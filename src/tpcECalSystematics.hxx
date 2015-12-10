#ifndef tpcECalSystematics_h
#define tpcECalSystematics_h

#include "baseAnalysis.hxx"
#include "BaseDataClasses.hxx"
#include "AnalysisUtils.hxx"
#include "tpcECalSelection.hxx"

class tpcECalSystematics: public baseAnalysis {
 public:

  tpcECalSystematics(AnalysisAlgorithm* ana=NULL);
  virtual ~tpcECalSystematics(){}

  void DefineSelections();
  void DefineConfigurations();
  void DefineCorrections();
  void DefineSystematics();
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();
  void FillMicroTrees(bool addBase=true);
  bool CheckFillTruthTree(const AnaTrueVertex& vtx);
  bool CheckFillRooTracker(const AnaTrueVertex& vtx);
  void FillToyVarsInMicroTrees(bool addBase=false);
  void FillTruthTree(const AnaTrueVertex& vtx);
  void FillCategories();

  const ToyBoxTpcECal& tpcECalbox(){return *static_cast<const ToyBoxTpcECal*>(&box());}

 public:

  enum enumStandardMicroTrees_XsecRatioAnalysis{

   // Reconstructed TPC variables
   track_charge = enumStandardMicroTreesLast_baseAnalysis+1,
   track_mom,
   pullelec,
   pullmuon,
   pullpion,
   pullprot,
   tpcBackPos,
   tpcBackDir,
   IsForwards,
   tpcNHits,
   ecaldet,
   EnterDs,
   EnterBarrel,
   IsHighestMom,

   // Reconstructed ECal variables.
   NearestEcalPos,
   NearestEcalDir,
   NearestEcalShowerLike,
   NearestEcalDiff,
   NearestEcalZDiff,
   NearestEcalNHits,
   NearestEcalEmEne,

   // True variables
   TrueBarrel,
   TrueDs
  };

};



AnaEcalTrackB* GetNearestEcal(std::vector<AnaTrackB*> tracks, TLorentzVector pos) {

  AnaEcalTrackB* selected = NULL;
  double min = 1e6;

  for (unsigned int i = 0; i < tracks.size(); i++) {
    AnaTrackB* track = tracks[i];

    for (int j = 0; j < track->nECALSegments; j++) {
      AnaEcalTrackB* ecal = track->ECALSegments[j];
      double diff = (utils::ArrayToTLorentzVector(ecal->PositionStart) - pos).Vect().Mag();

      if (diff < min) {
        min = diff;
        selected = ecal;
      }
    }
  }

  return selected;
}

#endif
