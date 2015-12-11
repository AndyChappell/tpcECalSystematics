#ifndef tpcECalSystematics_h
#define tpcECalSystematics_h

#include "baseAnalysis.hxx"
#include "BaseDataClasses.hxx"
#include "AnalysisUtils.hxx"
#include "tpcECalSelection.hxx"

class tpcECalSystematics: public baseAnalysis
{
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

   const ToyBoxTpcECal& tpcECalbox()
   {
      return *static_cast<const ToyBoxTpcECal*>(&box());
   }

   enum enumStandardMicroTrees_XsecRatioAnalysis
   {
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

   private:
   /**
      Extracts the bits that identify parts of the barrel ECal and checks if
      they are set.
      \param detector   The bitfield containing all of the subdetectors
                        intersected by a track. This value comes from the field
                        AnaTrackB::Detector.
      \return  True if the track intersects part of the barrel ECal, False
               otherwise.
   */
   bool IsBarrelECal(const unsigned long detector);

   /**
      Extracts the bit that identifies the DS ECal and checks if it is set.
      \param detector   The bitfield containing all of the subdetectors
                        intersected by a track. This value comes from the field
                        AnaTrackB::Detector.
      \return  True if the track intersects the DS ECal, False otherwise.
   */
   bool IsDSECal(const unsigned long detector);
};

AnaEcalTrackB* GetNearestEcal(std::vector<AnaTrackB*> tracks,
   TLorentzVector pos)
{
   AnaEcalTrackB* selected = NULL;
   double min = 1e6;

   for (unsigned int i = 0; i < tracks.size(); i++)
   {
      AnaTrackB* track = tracks[i];

      for (int j = 0; j < track->nECALSegments; j++)
      {
         AnaEcalTrackB* ecal = track->ECALSegments[j];
         double diff = (utils::ArrayToTLorentzVector(ecal->PositionStart) -
            pos).Vect().Mag();

         if (diff < min)
         {
            min = diff;
            selected = ecal;
         }
      }
   }

   return selected;
}

#endif
