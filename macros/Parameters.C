{

	// ****************************************************************************************************
	// Load parameters here instead of in each other macro.
	// Include neutrino and anti-neutrino data/MC.
	// These can be combined to increase sample size.
	// Can determine the binning to use by running Binning.C
        // ****************************************************************************************************

	bool useAnti = true;

	// Environment detailing file locations
        char* mcfile = getenv("TPCECALMCFILE");
        char* datafile = getenv("TPCECALDATAFILE");
	char* amcfile = getenv("TPCECALANTIMCFILE");
	char* adatafile = getenv("TPCECALANTIDATAFILE");

	if(!mcfile || !datafile || !amcfile || !adatafile)
        {
		if (!mcfile) std::cerr << "Must set $TPCECALMCFILE" << std::endl;
		if (!datafile) std::cerr << "Must set $TPCECALDATAFILE" << std::endl;
		if (!amcfile) std::cerr << "Must set $TPCECALANTIMCFILE" << std::endl;
		if (!adatafile) std::cerr << "Must set $TPCECALANTIDATAFILE" << std::endl;
                exit(1);
        }

	// Read in the files.
        DataSample data(datafile);
        DataSample mc(mcfile);
	DataSample anti_data(adatafile);
	DataSample anti_mc(amcfile);
        DrawingToolsTpcEcal draw(mcfile);

	// Ratio of data POT to MC POT.
	if(data.GetTree()->GetEntries() == 0 || mc.GetTree()->GetEntries() == 0 || anti_data.GetTree()->GetEntries() == 0 || anti_mc.GetTree()->GetEntries() == 0)
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

	const int nds_mom = 5;
	double ds_bins_mom[6] = {0,400,800,1200,2500,5000};
	const int nbr_mom = 5;
	double br_bins_mom[6] = {0,400,800,1200,2500,5000};
	const int nds_ang = 3;
	double ds_bins_ang[4] = {0.7,0.85,0.95,1.0};
	const int nbr_ang = 4;
	double br_bins_ang[5] = {0,0.2,0.4,0.6,1.0};

	std::string isBr = "accum_level[][1]>4"; // Appears to enter barrel based on cuts
	std::string isDS = "accum_level[][0]>4";
	std::string recoBr = "ecaldet<9 && ecaldet>4"; // Reconstructed as entering the barrel
	std::string recoDS = "ecaldet==9";
	std::string isEl = "&& pullelec>-1 && pullelec<2.5 && (pullmuon>2.5 || pullmuon<-2.5) && (pullpion>2 || pullpion<-2)";
	std::string isMu = "&& pullmuon>-2 && pullmuon<2 && (pullelec>2 || pullelec<-1)";
	std::string isProt = "&& pullprot>-2 && pullprot<2 && (pullelec>2 || pullelec<-1)";

	std::string momentum = "track_mom";
	std::string angle = "tpcBackDir[2]";
}