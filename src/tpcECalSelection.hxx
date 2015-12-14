#ifndef tpcECalSelection_h
#define tpcECalSelection_h

#include "SelectionBase.hxx"
#include "BasicUtils.hxx"

#include "Parameters.hxx"
#include "baseAnalysis.hxx"

class tpcECalSelection: public SelectionBase
{
   public:
   tpcECalSelection(bool forceBreak=true);
   virtual ~tpcECalSelection(){}

   //---- These are mandatory functions
   void DefineSteps();
   inline ToyBoxB* MakeToyBox();
   void InitializeEvent(AnaEventB& event);
   bool FillEventSummary(AnaEventB& event, Int_t allCutsPassed[]);
   nd280Samples::SampleEnum GetSampleEnum(){ return nd280Samples::kUnassigned; }

   bool CheckRedoSelection(const AnaEventB& event,
      const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
   bool IsRelevantTrackForSystematic(const AnaEventB& event, AnaTrackB* track,
      Int_t syst_index, Int_t branch) const;
   bool IsRelevantTrueTrackForSystematic(const AnaEventB& event,
      AnaTrueTrackB* trueTrack, Int_t syst_index, Int_t branch) const;
   bool IsRelevantSystematic(const AnaEventB& event, const ToyBoxB& box,
      Int_t syst_index, Int_t branch) const;
};


/// This class stores the objects used by the package.
/// E.g. Selected tracks, vertex, veto track etc...
class ToyBoxTpcECal: public ToyBoxB
{
   public:
   ToyBoxTpcECal()
   {
      TrackToUse = NULL;
      EnterDs = false;
      EnterBarrel = false;
   }

   virtual ~ToyBoxTpcECal(){}

   void Reset(){
   //      ToyBoxB::Reset();
   FgdFvTracks.clear();
   DsPosTracks.clear();
   BarrelPosTracks.clear();
   DsPosDirTracks.clear();
   BarrelPosDirTracks.clear();
   TrackToUse = NULL;
   EnterDs = false;
   EnterBarrel = false;
   }  

   /// Access to the singleton.
   static ToyBoxTpcECal& Get(void);

   /// Track with TPC that start in the FGD FV.
   std::vector<AnaTrackB*> FgdFvTracks;

   /// Tracks with TPC that finish near the DS.
   std::vector<AnaTrackB*> DsPosTracks;

   /// Tracks with TPC that finish near the Barrel.
   std::vector<AnaTrackB*> BarrelPosTracks;

   /// Tracks with TPC that finish near the DS and pass direction cuts.
   std::vector<AnaTrackB*> DsPosDirTracks;

   /// Tracks with TPC that finish near the Barrel and pass direction cuts.
   std::vector<AnaTrackB*> BarrelPosDirTracks;

   /// The track chosen in a bunch. There is a slim possibility that more than
   /// one track will meet the criteria. If this is the case, we just select
   /// one of them.
   AnaTrackB* TrackToUse;

   /// Whether TrackToUse was selected as entering the DsECal.
   bool EnterDs;

   /// Whether TrackToUse was selected as entering the Barrel.
   bool EnterBarrel;
};

inline ToyBoxB* tpcECalSelection::MakeToyBox() { return new ToyBoxTpcECal(); }

class FillSummaryAction_tpcECal: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;  
   StepBase* MakeClone(){ return new FillSummaryAction_tpcECal(); }
};

class FindLeadingTracksAction: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new FindLeadingTracksAction(); }
};

class FindTracksFgdFvAction: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new FindTracksFgdFvAction(); }
};

class FindTracksDsPosAction: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new FindTracksDsPosAction(); }
};

class FindTracksDsPosDirAction: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new FindTracksDsPosDirAction(); }
};

class FindTracksBarrelPosAction: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new FindTracksBarrelPosAction(); }
};

class FindTracksBarrelPosDirAction: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new FindTracksBarrelPosDirAction(); }
};

class ChooseTrackAction: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new ChooseTrackAction(); }
};

class HasTpcTracksCut: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new HasTpcTracksCut(); }
};

class HasFgdFvTracksCut: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new HasFgdFvTracksCut(); }
};

class DsPosCut: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new DsPosCut(); }
};

class DsPosDirCut: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new DsPosDirCut(); }
};

class BarrelPosCut: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new BarrelPosCut(); }
};

class BarrelPosDirCut: public StepBase
{
   public:
   using StepBase::Apply;
   bool Apply(AnaEventB& event, ToyBoxB& box) const;
   StepBase* MakeClone(){ return new BarrelPosDirCut(); }
};

class SelectionCriteria
{
   public:
   /**
      Checks whether a track has more than 18 TPC nodes in the most downstream
      TPC.
      \param track   The TPC track to be checked.
      \return  True if the track has more than 18 TPC nodes, false otherwise.
   */
   static bool HasSufficientTpcNodes(const AnaTpcTrack& tpc);

   /**
      Checks whether tracks entering the DS ECal have emerged from a specified
      volume within the most downstream TPC.
      \param pos  The position to be checked.
      \return  True if the track exited the most downstream TPC from within the
               specified volume.
   */
   static bool IsPositionOkDSECal(const TVector3& pos);

   /**
      Checks whether tracks entering the DS ECal have emerged from within a
      specified cone within the most downstream TPC.
      \param dir  The direction to be checked.
      \return  True if the track exited the most downstream TPC from within the
               specified cone.
   */
   static bool IsDirectionOkDSECal(const TVector3& dir);

   /**
      Checks whether tracks entering the Barrel ECal have emerged from a
      specified volume within the most downstream TPC.
      \param pos  The position to be checked.
      \return  True if the track exited the most downstream TPC from within the
               specified volume.
   */
   static bool IsPositionOkBarrelECal(const TVector3& pos);

   /**
      Checks whether tracks entering the Barrel ECal have emerged from within a
      specified cone within the most downstream TPC.
      \param dir  The direction to be checked.
      \return  True if the track exited the most downstream TPC from within the
               specified cone.
   */
   static bool IsDirectionOkBarrelECal(const TVector3& dir);
   
   private:
   SelectionCriteria(){ };

   static const int NumTpcNodes = 19;
   static const float DSTpcXMin = -920;
   static const float DSTpcXMax = +920;
   static const float DSTpcYMin = -910;
   static const float DSTpcYMax = +930;
   static const float DSTpcZMin = +2665;
   static const float DSTpcAngleMax = +40;
   static const float BarrelTpcXMin = -890;
   static const float BarrelTpcXMax = +890;
   static const float BarrelTpcYMin = -980;
   static const float BarrelTpcYMax = +1085;
   static const float BarrelTpcZMin = +600;
   static const float BarrelTpcZMax = +2600;
   static const float BarrelTpcAzimuthAbs = +160;
   static const float BarrelTpcAngleMin = +35;
};

#endif
