#include "tpcECalSelection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "Parameters.hxx"
#include "nd280Samples.hxx"
#include "EventBoxUtils.hxx"
#include "BasicUtils.hxx"

#include "baseAnalysis.hxx"

#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "Parameters.hxx"


tpcECalSelection::tpcECalSelection(bool forceBreak): SelectionBase(forceBreak)
{
   // Define the steps
   DefineSteps();
}

void tpcECalSelection::DefineSteps()
{
   AddStep(StepBase::kCut, "event quality", new EventQualityCut(), true);

   // Actions
   AddStep(StepBase::kAction, "Leading tracks", new FindLeadingTracksAction());
   AddStep(StepBase::kAction, "FGD FV Action", new FindTracksFgdFvAction());
   AddStep(StepBase::kAction, "DS Position Action",
      new FindTracksDsPosAction());
   AddStep(StepBase::kAction, "DS Direction Action",
      new FindTracksDsPosDirAction());
   AddStep(StepBase::kAction, "Barrel Position Action",
      new FindTracksBarrelPosAction());
   AddStep(StepBase::kAction, "Barrel Direction Action",
      new FindTracksBarrelPosDirAction());
   AddStep(StepBase::kAction, "Choose the track Action",
      new ChooseTrackAction());

   // Cuts
   AddStep(StepBase::kCut,  "Has TPC Tracks", new HasTpcTracksCut());
   AddStep(StepBase::kCut,  "Has Tracks in FGD FV", new HasFgdFvTracksCut());

   // Add a split to the trunk with 2 branches 
   AddSplit(2);

   AddStep(0, StepBase::kCut, "DS Position", new DsPosCut());
   AddStep(0, StepBase::kCut, "DS Direction", new DsPosDirCut());
   AddStep(1, StepBase::kCut, "Barrel Position", new BarrelPosCut());
   AddStep(1, StepBase::kCut, "Barrel Direction", new BarrelPosDirCut());

   SetBranchAlias(0,"dsECal", 0);
   SetBranchAlias(1,"brECal", 1);

   // By default the preselection correspond to cuts 0-2
   SetPreSelectionAccumLevel(2);
}

bool tpcECalSelection::FillEventSummary(AnaEventB& event, Int_t allCutsPassed[])
{
   (void) event;
   (void) allCutsPassed;

   return true; 
}

bool FillSummaryAction_tpcECal::Apply(AnaEventB& event, ToyBoxB& box) const
{
   (void) event;
   (void) box;

   return true;
}


bool tpcECalSelection::IsRelevantTrackForSystematic(const AnaEventB& event,
   AnaTrackB* track, Int_t syst_index, Int_t branch) const
{
   (void) syst_index;
   (void) track;
   (void) branch;
   (void) event;

   return true;
}

bool tpcECalSelection::IsRelevantTrueTrackForSystematic(const AnaEventB& event,
   AnaTrueTrackB* trueTrack, Int_t syst_index, Int_t branch) const
{
   (void) syst_index;
   (void) trueTrack;
   (void) branch;
   (void) event;

   return true;
}

bool tpcECalSelection::IsRelevantSystematic(const AnaEventB& event,
   const ToyBoxB& box, Int_t syst_index, Int_t branch) const
{
   (void) syst_index;
   (void) box;
   (void) branch;
   (void) event;

   return true;
}

bool tpcECalSelection::CheckRedoSelection(const AnaEventB& event,
   const ToyBoxB& PreviousToyBox, Int_t& redoFromStep)
{
   (void) PreviousToyBox;
   (void) event;
   (void) redoFromStep;

   return true;
}

void tpcECalSelection::InitializeEvent(AnaEventB& event)
{
   // Create the appropriate EventBox if it does not exist yet
   if (!event.EventBoxes[AnaEventB::kEventBoxTracker])
   {
      event.EventBoxes[AnaEventB::kEventBoxTracker] = new EventBoxTracker();
   }

   boxUtils::FillTracksWithTPC(event, SubDetId::kFGD);
   boxUtils::FillTracksWithFGD(event, SubDetId::kFGD);
   boxUtils::FillTrajsChargedInTPC(event);
   boxUtils::FillTrajsChargedInFGDAndNoTPC(event, SubDetId::kFGD);
}

bool FindLeadingTracksAction::Apply(AnaEventB& event, ToyBoxB& box) const
{
   cutUtils::FindLeadingTracksOld(event, box, false, SubDetId::kFGD);

   return true;
}

bool FindTracksFgdFvAction::Apply(AnaEventB& event, ToyBoxB& box) const
{
   ToyBoxTpcECal *tpcECalbox = static_cast<ToyBoxTpcECal*>(&box);

   AnaTrackB* tpcTracks[100];
   Int_t Ntpc = anaUtils::GetAllTracksUsingTPC(event, tpcTracks);

   for (int i = 0; i < Ntpc; i++)
   {
      if (cutUtils::FiducialCut(*(tpcTracks[i]), SubDetId::kFGD))
      {
         tpcECalbox->FgdFvTracks.push_back(tpcTracks[i]);
      }
   }

   return true;
}

bool FindTracksDsPosAction::Apply(AnaEventB& event, ToyBoxB& box) const
{
   (void) event;

   ToyBoxTpcECal *tpcECalbox = static_cast<ToyBoxTpcECal*>(&box);

   std::vector<AnaTrackB*> tpcTracks = tpcECalbox->FgdFvTracks;

   for (unsigned int i = 0; i < tpcTracks.size(); i++)
   {
      AnaTrackB* track = tpcTracks[i];
      AnaTpcTrack* backTpc = static_cast<AnaTpcTrack*>(
         anaUtils::GetTPCBackSegment(track));
      TVector3 endpos = utils::ArrayToTVector3(backTpc->PositionEnd);

      if (backTpc->NHits < 19)
      {
         continue;
      }
      if (endpos.X() != endpos.X())
      {
         continue;
      }
      if (fabs(endpos.X()) > 920.)
      {
         continue;
      }
      if (fabs(endpos.Y() + 10.) > 920.)
      {
         continue;
      }
      if (endpos.Z() < 2665.)
      {
         continue;
      }

      tpcECalbox->DsPosTracks.push_back(track);
   }

   return true;
}

bool FindTracksDsPosDirAction::Apply(AnaEventB& event, ToyBoxB& box) const
{
   (void) event;

   ToyBoxTpcECal *tpcECalbox = static_cast<ToyBoxTpcECal*>(&box);

   std::vector<AnaTrackB*> tpcTracks = tpcECalbox->DsPosTracks;

   for (unsigned int i = 0; i < tpcTracks.size(); i++)
   {
      AnaTrackB* track = tpcTracks[i];
      AnaTpcTrack* backTpc = static_cast<AnaTpcTrack*>(
         anaUtils::GetTPCBackSegment(track));
      TVector3 enddir = utils::ArrayToTVector3(backTpc->DirectionEnd);
      TVector3 zdir(0., 0., 1.);

      if (enddir.X() != enddir.X())
      {
         continue;
      }

      double tpcBackAngleFromZ = TMath::RadToDeg() * enddir.Unit().Angle(zdir);
      if (tpcBackAngleFromZ > 40)
      {
         continue;
      }

      tpcECalbox->DsPosDirTracks.push_back(track);
   }

   return true;
}

bool FindTracksBarrelPosAction::Apply(AnaEventB& event, ToyBoxB& box) const
{
   (void) event;

   ToyBoxTpcECal *tpcECalbox = static_cast<ToyBoxTpcECal*>(&box);

   std::vector<AnaTrackB*> tpcTracks = tpcECalbox->FgdFvTracks;

   double barrelLeft = 890.;
   double barrelRight = -890.;
   double barrelBottom = -980.;
   double barrelTop = 1085.;
   double barrelFront = 600.;
   double barrelBack = 2600.;

   for (unsigned int i = 0; i < tpcTracks.size(); i++)
   {
      AnaTrackB* track = tpcTracks[i];
      AnaTpcTrack* backTpc = static_cast<AnaTpcTrack*> (anaUtils::GetTPCBackSegment(track));
      TVector3 endpos = utils::ArrayToTVector3(backTpc->PositionEnd);

      if (backTpc->NHits < 19)
      {
         continue;
      }
      if (endpos.X() != endpos.X())
      {
         continue;
      }
      if (!(endpos.X() > barrelLeft || endpos.X() < barrelRight ||
         endpos.Y() < barrelBottom || endpos.Y() > barrelTop))
      {
         continue;
      }
      if(endpos.Z() < barrelFront || endpos.Z() > barrelBack)
      {
         continue;
      }

      tpcECalbox->BarrelPosTracks.push_back(track);
   }

   return true;
}

bool FindTracksBarrelPosDirAction::Apply(AnaEventB& event, ToyBoxB& box) const
{
   (void) event;

   ToyBoxTpcECal *tpcECalbox = static_cast<ToyBoxTpcECal*>(&box);

   std::vector<AnaTrackB*> tpcTracks = tpcECalbox->BarrelPosTracks;

   for (unsigned int i = 0; i < tpcTracks.size(); i++)
   {
      AnaTrackB* track = tpcTracks[i];
      AnaTpcTrack* backTpc = static_cast<AnaTpcTrack*>(
         anaUtils::GetTPCBackSegment(track));
      TVector3 enddir = utils::ArrayToTVector3(backTpc->DirectionEnd);
      TVector3 zdir(0., 0., 1.);

      if (enddir.X() != enddir.X())
      {
         continue;
      }

      double tpcBackAzimuth = TMath::RadToDeg() * TMath::ATan(
         enddir.X() / enddir.Y());

      if (enddir.X() > 0 && enddir.Y() < 0)
      {
         tpcBackAzimuth += 180;
      }
      else if (enddir.X() < 0 && enddir.Y() < 0)
      {
         tpcBackAzimuth -= 180;
      }

      double tpcBackAngleFromZ = enddir.Unit().Dot(zdir);
      tpcBackAngleFromZ = TMath::RadToDeg() * TMath::ACos(tpcBackAngleFromZ);

      if (tpcBackAzimuth > 160 || tpcBackAzimuth < -160)
      {
         continue;
      }

      if (tpcBackAngleFromZ < 35.)
      {
         continue;
      }

      tpcECalbox->BarrelPosDirTracks.push_back(track);
   }

   return true;
}

bool ChooseTrackAction::Apply(AnaEventB& event, ToyBoxB& box) const
{
   (void) event;

   ToyBoxTpcECal *tpcECalbox = static_cast<ToyBoxTpcECal*>(&box);

   if (tpcECalbox->BarrelPosDirTracks.size() > 0)
   {
      tpcECalbox->TrackToUse = tpcECalbox->BarrelPosDirTracks.back();
      tpcECalbox->EnterBarrel = true;
   }
   else if (tpcECalbox->DsPosDirTracks.size() > 0)
   {
      tpcECalbox->TrackToUse = tpcECalbox->DsPosDirTracks.back();
      tpcECalbox->EnterDs = true;
   }

   if (!tpcECalbox->TrackToUse)
   {
      return false;
   }

   return true;
}

bool HasTpcTracksCut::Apply(AnaEventB& event, ToyBoxB& box) const
{
   (void) box;

   bool pass = false;

   AnaTrackB* tracks[100];
   Int_t Ntpc = anaUtils::GetAllTracksUsingTPC(event, tracks);

   for (int i = 0; i < Ntpc; i++)
   {
      AnaTpcTrack* backTpc = static_cast<AnaTpcTrack*>(
         anaUtils::GetTPCBackSegment(tracks[i]));
      if (backTpc->NHits > 19)
      {
         pass = true;
         break;
      }
   }

   return pass;
}

bool HasFgdFvTracksCut::Apply(AnaEventB& event, ToyBoxB& box) const
{
   (void) event;

   ToyBoxTpcECal *tpcECalbox = static_cast<ToyBoxTpcECal*>(&box);

   return (tpcECalbox->FgdFvTracks.size() > 0);
}

bool DsPosCut::Apply(AnaEventB& event, ToyBoxB& box) const
{
   (void) event;

   ToyBoxTpcECal *tpcECalbox = static_cast<ToyBoxTpcECal*>(&box);

   return (tpcECalbox->DsPosTracks.size() > 0);
}

bool DsPosDirCut::Apply(AnaEventB& event, ToyBoxB& box) const
{
   (void) event;

   ToyBoxTpcECal *tpcECalbox = static_cast<ToyBoxTpcECal*>(&box);

   return (tpcECalbox->DsPosDirTracks.size() > 0);
}

bool BarrelPosCut::Apply(AnaEventB& event, ToyBoxB& box) const
{
   (void) event;

   ToyBoxTpcECal *tpcECalbox = static_cast<ToyBoxTpcECal*>(&box);

   return (tpcECalbox->BarrelPosTracks.size() > 0);
}

bool BarrelPosDirCut::Apply(AnaEventB& event, ToyBoxB& box) const
{
   (void) event;

   ToyBoxTpcECal *tpcECalbox = static_cast<ToyBoxTpcECal*>(&box);

   return (tpcECalbox->BarrelPosDirTracks.size() > 0);
}
