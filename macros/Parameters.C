{
   /****************************************************************************
      Load parameters here instead of in each other macro.
      Include neutrino and anti-neutrino data/MC.
      These can be combined to increase sample size.
      Can determine the binning to use by running Binning.C
   ****************************************************************************/

   bool useAnti = true;

   // Environment detailing file locations
   char* mcfile = getenv("TPCECALMCFILE");
   char* datafile = getenv("TPCECALDATAFILE");
   char* amcfile = getenv("TPCECALANTIMCFILE");
   char* adatafile = getenv("TPCECALANTIDATAFILE");

   if(!mcfile || !datafile || !amcfile || !adatafile)
   {
      if (!mcfile)
      {
         std::cerr << "Must set $TPCECALMCFILE" << std::endl;
      }
      if (!datafile)
      {
         std::cerr << "Must set $TPCECALDATAFILE" << std::endl;
      }
      if (!amcfile)
      {
         std::cerr << "Must set $TPCECALANTIMCFILE" << std::endl;
      }
      if (!adatafile)
      {
         std::cerr << "Must set $TPCECALANTIDATAFILE" << std::endl;
      }
      exit(1);
   }

   // Read in the files.
   DataSample data(datafile);
   DataSample mc(mcfile);
   DataSample anti_data(adatafile);
   DataSample anti_mc(amcfile);
   DrawingToolsTpcEcal draw(mcfile);

   // Ratio of data POT to MC POT.
   if(data.GetTree()->GetEntries() == 0 || mc.GetTree()->GetEntries() == 0 ||
      anti_data.GetTree()->GetEntries() == 0 ||
      anti_mc.GetTree()->GetEntries() == 0)
   {
      std::cout << "Warning! Tree has no entries. Aborting..." << std::endl;
      exit(1);
   }

   // Print out the POT in each file.
   std::cout << std::endl << "Neutrino Data POT" << std::endl;
   draw.DumpPOT(data);
   std::cout << std::endl << "Neutrino MC POT" << std::endl;
   draw.DumpPOT(mc);
   std::cout << std::endl << "Anti-neutrino Data POT" << std::endl;
   draw.DumpPOT(anti_data);
   std::cout << std::endl << "Anti-neutrino MC POT" << std::endl;
   draw.DumpPOT(anti_mc);
   std::cout << std::endl;

   draw.SetDifferentStackFillStyles();
   draw.ApplyRange(false);
   gStyle->SetOptStat(0);

/*   const int nds_mom = 4;
   double ds_bins_mom[5] = {0,500,700,1800,5000};
   const int nbr_mom = 5;
   double br_bins_mom[6] = {0,400,800,2000,3000,5000};
   const int nds_ang = 5;
   double ds_bins_ang[6] = {0.75,0.8,0.85,0.925,0.975,1.0};
   const int nbr_ang = 7;
   double br_bins_ang[8] = {-0.5,-0.15,0,0.25,0.35,0.65,0.75,0.85};*/

   const int nds_mom = 8;
   double ds_bins_mom[9] = {0,200,400,600,800,1250,1750,4000,5000};
   const int nds_ang = 4;
   double ds_bins_ang[5] = {0.75,0.8,0.85,0.925,1.0};

   // Subject to change, very low stats at mid-to-high momentum
   const int nbr_mom = 6;
   double br_bins_mom[7] = {0,200,400,600,1800,3200,5000};
   const int nbr_ang = 7;
   double br_bins_ang[8] = {-0.3,-0.1,0.1,0.3,0.4,0.7,0.775,0.825};

   // Appears to enter barrel based on cuts
   std::string isBr = "accum_level[][1]>4";
   std::string isDS = "accum_level[][0]>4";
   // Reconstructed as entering the barrel
   std::string recoBr = "ecaldet<9 && ecaldet>4";
   std::string recoDS = "ecaldet==9";
   std::string isEl = "&& pullelec>-1 && pullelec<2.5 && (pullmuon>2.5 || pullmuon<-2.5) && (pullpion>2 || pullpion<-2)";
   std::string isMu = "&& pullmuon>-2 && pullmuon<2 && (pullelec>2 || pullelec<-1)";
   std::string isProt = "&& pullprot>-2 && pullprot<2 && (pullelec>2 || pullelec<-1)";

   std::string momentum = "track_mom";
   std::string angle = "tpcBackDir[2]";
}
