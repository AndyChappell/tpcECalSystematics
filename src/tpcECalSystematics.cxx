#include "tpcECalSystematics.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "UseGlobalAltMomCorrection.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "CutUtils.hxx"
#include "SubDetId.hxx"

/*
TODO
The tpcECalDrawingTools has methods for combining the results of running
neutrino and anti-neutrino mode. This can be validated in the code
but it may not be necessary (maybe can run them together). This
should be checked. The reason the samples were combined is that 
the systematic study is limited by statistics and the cuts are 
charge independent.
*/

tpcECalSystematics::tpcECalSystematics(AnalysisAlgorithm* ana) :
   baseAnalysis(ana)
{
   // Minimum accum level to save event into the output tree
   SetMinAccumCutLevelToSave(2);

   ND::versioning().AddPackage("tpcECalSystematics",
      anaUtils::GetSoftwareVersionFromPath((std::string)getenv(
         "TPCECALSYSTEMATICSROOT")));
}

void tpcECalSystematics::DefineSelections()
{
   // ----- Inclusive CC -----------
   sel().AddSelection("tpcECal", "Selecting events that appear to enter an ECal",
      new tpcECalSelection(true));
}

void tpcECalSystematics::DefineConfigurations()
{
   // Some configurations are defined in the baseAnalysis
   baseAnalysis::DefineConfigurations();
}

void tpcECalSystematics::DefineCorrections()
{
   // Some corrections are defined in baseAnalysis
   baseAnalysis::DefineCorrections();

   //------------ Corrections -----------
   // Change the main fit momentum by the muon alternate momentum, but only for P5
   #if !VERSION_HAS_EQUIVALENT_MAIN_AND_ALT_FITS
   if (ND::params().GetParameterI(
      "tpcECalSystematics.Corrections.EnableUseMuonAltMom") == 1)
   {
      corr().AddCorrection("altmom_corr", new UseGlobalAltMomCorrection(
         UseGlobalAltMomCorrection::kMuon));
   }
   #endif   
}

void tpcECalSystematics::DefineSystematics()
{
   // Some systematics are defined in baseAnalysis.
   baseAnalysis::DefineSystematics();
}

void tpcECalSystematics::DefineMicroTrees(bool addBase)
{
   // -------- Add variables to the analysis tree ----------------------

   // Variables from baseAnalysis (run, event, ...)
   baseAnalysis::DefineMicroTrees(addBase);

   // Reconstructed TPC track variables.
   AddVarI(output(), track_charge,
      "Reconstructed charge of the selected track");
   AddVarF(output(), track_mom,
      "Reconstructed momentum of selected track, from the refit assuming the "
      "electron hypothesis");
   AddVarF(output(), pullelec,
      "dE/dx pull of the track, assuming the electron hypothesis");
   AddVarF(output(), pullmuon,
      "dE/dx pull of the track, assuming the muon hypothesis");
   AddVarF(output(), pullpion,
      "dE/dx pull of the track, assuming the pion hypothesis");
   AddVarF(output(), pullprot,
      "dE/dx pull of the track, assuming the proton hypothesis");
   AddVar4VF(output(), tpcBackPos, "End position of the track");
   AddVar3VF(output(), tpcBackDir, "End direction of the track");
   AddVarI(output(), IsForwards,
      "Whether the track is reconstruced as being forwards or backwards going");
   AddVarI(output(), tpcNHits,
      "Number of hits the track has in the last TPC it passes through");
   AddVarI(output(), ecaldet,
      "Number identifying the part of the ECal the track appears to enter. "
      "9 == DS, 5 - 8 == BR");
   AddVarI(output(), EnterDs,
      "Whether the track appears to enter the DS ECal");
   AddVarI(output(), EnterBarrel,
      "Whether the track appears to enter the Barrel ECal");
   AddVarI(output(), IsHighestMom,
      "Whether the track is the highest momentum track in the event");

   // Reconstructed ECal variables.
   AddVar4VF(output(), NearestEcalPos,
      "Position of the nearest ECal segment to the end of the TPC track");
   AddVar3VF(output(), NearestEcalDir,
      "Direction of the nearest ECal segment to the end of the TPC track");
   AddVarI(output(), NearestEcalShowerLike,
      "Whether the ECal segment is shower-like");
   AddVarD(output(), NearestEcalDiff,
      "Difference in position of the end of the TPC track and the start of "
      "the ECal segment");
   AddVarD(output(), NearestEcalZDiff,
      "Difference in z position of the end of the TPC track and the start of "
      "the ECal segment");
   AddVarI(output(), NearestEcalNHits,
      "Number of ECal hits the ECal segment has");
   AddVarF(output(), NearestEcalEmEne,
      "Reconstructed Energy of the ECal segment");

   // True variables
   AddVarI(output(), TrueBarrel, "Whether the TPC enters the barrel ECal in "
      "truth");
   AddVarI(output(), TrueDs, "Whether the TPC enters the DS ECal in truth");
}

void tpcECalSystematics::FillMicroTrees(bool addBase)
{
   // Variables from baseAnalysis (run, event, ...)
   if (addBase) baseAnalysis::FillMicroTreesBase(addBase);

   AnaTrackB* track = tpcECalbox().TrackToUse;
   if(!track) return;
   AnaTpcTrack* backTpc = static_cast<AnaTpcTrack*>(
      anaUtils::GetTPCBackSegment(track));

   output().FillVar(track_charge, track->Charge);
   output().FillVar(track_mom, backTpc->Momentum);
   output().FillVar(pullelec, backTpc->Pullele);
   output().FillVar(pullmuon, backTpc->Pullmu);
   output().FillVar(pullpion, backTpc->Pullpi);
   output().FillVar(pullprot, backTpc->Pullp);
   output().FillVectorVarFromArray(tpcBackPos, backTpc->PositionEnd, 4);
   output().FillVectorVarFromArray(tpcBackDir, backTpc->DirectionEnd, 3);
   output().FillVar(IsForwards, utils::ArrayToTLorentzVector(
      backTpc->PositionStart).Z() < utils::ArrayToTLorentzVector(
      backTpc->PositionEnd).Z());
   output().FillVar(tpcNHits, backTpc->NHits);
   output().FillVar(IsHighestMom, (int)(track == tpcECalbox().HMtrack));

   int det = -999;
   if(IsDSECal(track->Detector))
   {  // DS ECal
      det = 9;
   }
   else if(IsBarrelECal(track->Detector))
   {  // Barrel ECal
      det = 5;
   }
   else
   {
      det = -999;
   }

   output().FillVar(ecaldet, det);

   output().FillVar(EnterDs, (int) tpcECalbox().EnterDs);
   output().FillVar(EnterBarrel, (int) tpcECalbox().EnterBarrel);

   AnaEcalTrack* nearestEcal = static_cast<AnaEcalTrack*>(GetNearestEcal(
      tpcECalbox().FgdFvTracks, backTpc->PositionEnd));
   if(nearestEcal)
   {
      output().FillVar(NearestEcalNHits, nearestEcal->NHits);
      output().FillVar(NearestEcalEmEne, nearestEcal->EMEnergy);
      output().FillVar(NearestEcalShowerLike, nearestEcal->IsShowerLike);
      output().FillVectorVarFromArray(NearestEcalPos,
         nearestEcal->PositionStart, 4);
      output().FillVectorVarFromArray(NearestEcalDir,
         nearestEcal->DirectionStart, 3);
      output().FillVar(NearestEcalDiff, (utils::ArrayToTLorentzVector(
         nearestEcal->PositionStart) - utils::ArrayToTLorentzVector(
         backTpc->PositionEnd)).Vect().Mag());
      output().FillVar(NearestEcalZDiff, (utils::ArrayToTLorentzVector(
         nearestEcal->PositionStart) - utils::ArrayToTLorentzVector(
         backTpc->PositionEnd)).Z());
   }

   if(track->TrueTrack)
   {
      bool TrueBarrel = false;
      bool TrueDS = false;

      AnaDetCrossingB **DetCrossings = track->TrueTrack->DetCrossings;
      int nDet = track->TrueTrack->nDetCrossings;
      for(int i=0; i<nDet; i++)
      {
         if(IsBarrelECal(DetCrossings[i]->Detector))
         {
            TrueBarrel = true;
         }
         if(IsDSECal(DetCrossings[i]->Detector))
         {
            TrueDS = true;
         }
      }

      output().FillVar(TrueBarrel, TrueBarrel);
      output().FillVar(TrueDs, TrueDS);
   }
}

void tpcECalSystematics::DefineTruthTree()
{
   // Variables from baseAnalysis (run, event, ...)
   baseAnalysis::DefineTruthTree();
}

void tpcECalSystematics::FillTruthTree(const AnaTrueVertex& vtx)
{
   // Fill the common variables
   baseAnalysis::FillTruthTreeBase(vtx);
}

bool tpcECalSystematics::CheckFillTruthTree(const AnaTrueVertex& vtx)
{
   (void) vtx;

   // Don't need the truth tree for this package.
   return false;
}

bool tpcECalSystematics::CheckFillRooTracker(const AnaTrueVertex& vtx)
{
   (void) vtx;

   // Don't need the rootracker tree for this package.
   return false;
}

void tpcECalSystematics::FillToyVarsInMicroTrees(bool addBase)
{
   if(addBase)
   {
      baseAnalysis::FillToyVarsInMicroTreesBase(addBase);
   }
}

void tpcECalSystematics::FillCategories()
{
   // Fill the track categories for color drawing
   anaUtils::FillCategories(_event, static_cast<AnaTrack*>(
      tpcECalbox().TrackToUse),"");
}

bool tpcECalSystematics::IsBarrelECal(const unsigned long detector)
{
   bool barrelLeft = (detector & (1 << SubDetId::kLeftTECAL)) ? true : false;
   bool barrelRight = (detector & (1 << SubDetId::kRightTECAL)) ? true : false;
   bool barrelTop = (detector & (1 << SubDetId::kTopTECAL)) ? true : false;
   bool barrelBottom = (detector & (1 << SubDetId::kBottomTECAL)) ? true : false;
      
   return (barrelLeft || barrelRight || barrelTop || barrelBottom);
}

bool tpcECalSystematics::IsDSECal(const unsigned long detector)
{   
   return (detector & (1 << SubDetId::kDSECAL)) ? true : false;
}
