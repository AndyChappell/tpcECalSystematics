#ifndef tpcECalSelection_h
#define tpcECalSelection_h

#include "SelectionBase.hxx"
#include "BasicUtils.hxx"

#include "Parameters.hxx"

class tpcECalSelection: public SelectionBase{
 public:
  tpcECalSelection(bool forceBreak=true);
  virtual ~tpcECalSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  inline ToyBoxB* MakeToyBox();
  void InitializeEvent(AnaEventB& event);
  bool FillEventSummary(AnaEventB& event, Int_t allCutsPassed[]);
  nd280Samples::SampleEnum GetSampleEnum(){return nd280Samples::kUnassigned;}

  bool CheckRedoSelection(const AnaEventB& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
  bool IsRelevantTrackForSystematic(const AnaEventB& event, AnaTrackB* track, Int_t syst_index, Int_t branch) const;
  bool IsRelevantTrueTrackForSystematic(const AnaEventB& event, AnaTrueTrackB* trueTrack, Int_t syst_index, Int_t branch) const;
  bool IsRelevantSystematic(const AnaEventB& event, const ToyBoxB& box, Int_t syst_index, Int_t branch) const;
};


/// This class stores the objects used by the package.
/// E.g. Selected tracks, vertex, veto track etc...
class ToyBoxTpcECal: public ToyBoxB{

  public:
    ToyBoxTpcECal(){
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

inline ToyBoxB* tpcECalSelection::MakeToyBox() {return new ToyBoxTpcECal();}


class FillSummaryAction_tpcECal: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_tpcECal();}
};

class FindLeadingTracksAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindLeadingTracksAction();}
};

class FindTracksFgdFvAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindTracksFgdFvAction();}
};

class FindTracksDsPosAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindTracksDsPosAction();}
};

class FindTracksDsPosDirAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindTracksDsPosDirAction();}
};

class FindTracksBarrelPosAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindTracksBarrelPosAction();}
};

class FindTracksBarrelPosDirAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindTracksBarrelPosDirAction();}
};

class ChooseTrackAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ChooseTrackAction();}
};

class HasTpcTracksCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new HasTpcTracksCut();}
};

class HasFgdFvTracksCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new HasFgdFvTracksCut();}
};

class DsPosCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new DsPosCut();}
};

class DsPosDirCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new DsPosDirCut();}
};

class BarrelPosCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new BarrelPosCut();}
};

class BarrelPosDirCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventB& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new BarrelPosDirCut();}
};

#endif
