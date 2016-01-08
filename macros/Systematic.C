{
   /****************************************************************************
      Output all the plots for selections, efficiencies and systematics.
      The "all" plots just use neutrino data/MC.
      When breaking down into particle type, statistics start to become a
      problem. So the specific particle plots use combined neutrino and
      anti-neutrino data.
   ****************************************************************************/

   // Load all the parameters used for the analysis.
   // Store them in one file so they are the same for all macros.
   gROOT->ProcessLine(".x Parameters.C");

   // Make the folders to save plots into.
   system("mkdir -p Plots");
   system("mkdir -p Plots/Selections");
   system("mkdir -p Plots/Systematics");
   system("mkdir -p Plots/Others");

   /****************************************************************************
      Total selections.
   ****************************************************************************/

   draw.SetLegendSize(0.2, 0.4);

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Counts/Bin");
   draw.Draw(data, mc, momentum, nds_mom, ds_bins_mom, "particle", isDS);
   c1->Print("Plots/Selections/sel_ds_all_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(data, mc, angle, nds_ang, ds_bins_ang, "particle", isDS);
   c1->Print("Plots/Selections/sel_ds_all_ang.pdf");

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.Draw(data, mc, momentum, nbr_mom, br_bins_mom, "particle", isBr);
   c1->Print("Plots/Selections/sel_br_all_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(data, mc, angle, nbr_ang, br_bins_ang, "particle", isBr);
   c1->Print("Plots/Selections/sel_br_all_ang.pdf");

   /****************************************************************************
      Electron selections.
   ****************************************************************************/

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.Draw(data, mc, momentum, nds_mom, ds_bins_mom, "particle", isDS + isEl);
   c1->Print("Plots/Selections/sel_ds_ele_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(data, mc, angle, nds_ang, ds_bins_ang, "particle", isDS + isEl);
   c1->Print("Plots/Selections/sel_ds_ele_ang.pdf");

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.Draw(data, mc, momentum, nbr_mom, br_bins_mom, "particle", isBr + isEl);
   c1->Print("Plots/Selections/sel_br_ele_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(data, mc, angle, nbr_ang, br_bins_ang, "particle", isBr + isEl);
   c1->Print("Plots/Selections/sel_br_ele_ang.pdf");

   /****************************************************************************
      Muon selections.
   ****************************************************************************/

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.Draw(data, mc, momentum, nds_mom, ds_bins_mom, "particle", isDS + isMu);
   c1->Print("Plots/Selections/sel_ds_mu_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(data, mc, angle, nds_ang, ds_bins_ang, "particle", isDS + isMu);
   c1->Print("Plots/Selections/sel_ds_mu_ang.pdf");

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.Draw(data, mc, momentum, nbr_mom, br_bins_mom, "particle", isBr + isMu);
   c1->Print("Plots/Selections/sel_br_mu_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(data, mc, angle, nbr_ang, br_bins_ang, "particle", isBr + isMu);
   c1->Print("Plots/Selections/sel_br_mu_ang.pdf");

   /****************************************************************************
      1D Efficiency plots, all particles.
   ****************************************************************************/

   draw.SetLegendSize(0.15, 0.1);
   draw.SetLegendPos("br");
   draw.SetTitleY("Matching Efficiency");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.DrawEfficiency(data, momentum, isDS + " && !EnterBarrel && EnterDs",
      recoDS, nds_mom, ds_bins_mom, "", "#nu Data");
   draw.DrawEfficiency(mc, momentum, isDS + " && !EnterBarrel && EnterDs",
      recoDS, nds_mom, ds_bins_mom, "same", "#nu MC");
   c1->Print("Plots/Systematics/eff_ds_all_mom.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("cosine(Track Angle)");
   draw.DrawEfficiency(data, angle, isDS + " && !EnterBarrel && EnterDs",
      recoDS, nds_ang, ds_bins_ang, "", "#nu Data");
   draw.DrawEfficiency(mc, angle, isDS + " && !EnterBarrel && EnterDs", recoDS,
      nds_ang, ds_bins_ang, "same", "#nu MC");
   c1->Print("Plots/Systematics/eff_ds_all_ang.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.DrawEfficiency(data, momentum, isBr + " && EnterBarrel && !EnterDs",
      recoBr, nbr_mom, br_bins_mom, "", "#nu Data");
   draw.DrawEfficiency(mc, momentum, isBr + " && EnterBarrel && !EnterDs",
      recoBr, nbr_mom, br_bins_mom, "same", "#nu MC");
   c1->Print("Plots/Systematics/eff_br_all_mom.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("cosine(Track Angle)");
   draw.DrawEfficiency(data, angle, isBr + " && EnterBarrel && !EnterDs",
      recoBr, nbr_ang, br_bins_ang, "", "#nu Data");
   draw.DrawEfficiency(mc, angle, isBr + " && EnterBarrel && !EnterDs", recoBr,
      nbr_ang, br_bins_ang, "same", "#nu MC");
   c1->Print("Plots/Systematics/eff_br_all_ang.pdf");

   draw.SetLegendSize(0.15, 0.2);
   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.DrawEfficiency(data, momentum, isDS + " && !EnterBarrel && EnterDs",
      recoDS, nds_mom, ds_bins_mom, "", "#nu Data");
   draw.DrawEfficiency(mc, momentum, isDS + " && !EnterBarrel && EnterDs",
      recoDS, nds_mom, ds_bins_mom, "same", "#nu MC");
   draw.DrawEfficiency(anti_data, momentum, isDS +
      " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom, "same",
      "#bar{#nu} Data");
   draw.DrawEfficiency(anti_mc, momentum, isDS + " && !EnterBarrel && EnterDs",
      recoDS, nds_mom, ds_bins_mom, "same", "#bar{#nu} MC");
   c1->Print("Plots/Systematics/eff_ds_all_mom_all.pdf");

   draw.SetLegendPos("br");
   draw.DrawEfficiency(data, momentum, isBr + " && EnterBarrel && !EnterDs",
      recoBr, nbr_mom, br_bins_mom, "", "#nu Data");
   draw.DrawEfficiency(mc, momentum, isBr + " && EnterBarrel && !EnterDs",
      recoBr, nbr_mom, br_bins_mom, "same", "#nu MC");
   draw.DrawEfficiency(anti_data, momentum, isBr +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, "same",
      "#bar{#nu} Data");
   draw.DrawEfficiency(anti_mc, momentum, isBr + " && EnterBarrel && !EnterDs",
      recoBr, nbr_mom, br_bins_mom, "same", "#bar{#nu} MC");
   c1->Print("Plots/Systematics/eff_br_all_mom_all.pdf");

   /****************************************************************************
      1D systematics plots, all particles.
   ****************************************************************************/

   draw.SetLegendSize(0.15, 0.1);
   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Systematic Uncertainty");
   draw.CalculateSystematic(data, mc, momentum, isDS +
      " && !EnterBarrel && EnterDs", "ecaldet == 9", nds_mom, ds_bins_mom, "e1",
      "#nu");
   draw.CalculateSystematic(data, anti_data, mc, anti_mc, momentum, isDS +
      " && !EnterBarrel && EnterDs", "ecaldet == 9", nds_mom, ds_bins_mom,
      "e1same", "#nu + #bar{#nu}");
   c1.Print("Plots/Systematics/syst_ds_all_mom.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("cosine(Track Angle)");
   draw.CalculateSystematic(data, mc, angle, isDS +
      " && !EnterBarrel && EnterDs", "ecaldet == 9", nds_ang, ds_bins_ang,
      "e1", "#nu");
   draw.CalculateSystematic(data, anti_data, mc, anti_mc, angle, isDS +
      " && !EnterBarrel && EnterDs", "ecaldet == 9", nds_ang, ds_bins_ang,
      "e1same", "#nu + #bar{#nu}");
   c1.Print("Plots/Systematics/syst_ds_all_ang.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.CalculateSystematic(data, mc, momentum, isBr +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, "e1", "#nu");
   draw.CalculateSystematic(data, anti_data, mc, anti_mc, momentum, isBr +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, "e1same",
      "#nu + #bar{#nu}");
   c1.Print("Plots/Systematics/syst_br_all_mom.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("cosine(Track Angle)");
   draw.CalculateSystematic(data, mc, angle, isBr +
      " && EnterBarrel && !EnterDs", recoBr, nbr_ang, br_bins_ang, "e1", "#nu");
   draw.CalculateSystematic(data, anti_data, mc, anti_mc, angle, isBr +
      " && EnterBarrel && !EnterDs", recoBr, nbr_ang, br_bins_ang, "e1same",
      "#nu + #bar{#nu}");
   c1.Print("Plots/Systematics/syst_br_all_ang.pdf");

   /****************************************************************************
      2D Efficiency plots.
   ****************************************************************************/

   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("cosine(Track Angle)");
   draw.DrawCombinedEfficiency(mc, anti_mc, momentum, angle, isDS +
      " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom, nds_ang,
      ds_bins_ang, "COLZTEXT");
   c1->Print("Plots/Systematics/eff_ds_all_mc_2d.pdf");

   draw.DrawCombinedEfficiency(data, anti_data, momentum, angle, isDS +
      " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom, nds_ang,
      ds_bins_ang, "COLZTEXT");
   c1->Print("Plots/Systematics/eff_ds_all_data_2d.pdf");

   draw.DrawCombinedEfficiency(mc, anti_mc, momentum, angle, isBr +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, nbr_ang,
      br_bins_ang, "COLZTEXT");
   c1->Print("Plots/Systematics/eff_br_all_mc_2d.pdf");

   draw.DrawCombinedEfficiency(data, anti_data, momentum, angle, isBr +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, nbr_ang,
      br_bins_ang, "COLZTEXT");
   c1->Print("Plots/Systematics/eff_br_all_data_2d.pdf");

   /****************************************************************************
      2D systematics plots.
   ****************************************************************************/

   draw.CalculateSystematic(data, mc, momentum, angle,
      isDS + " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom,
      nds_ang, ds_bins_ang, "COLZTEXT");
   c1->Print("Plots/Systematics/syst_neutonly_ds_all_2d.pdf");

   draw.CalculateSystematic(data, mc, momentum, angle,
      isBr + " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom,
      nbr_ang, br_bins_ang, "COLZTEXT");
   c1->Print("Plots/Systematics/syst_neutonly_br_all_2d.pdf");

   draw.CalculateSystematic(data, anti_data, mc, anti_mc, momentum, angle,
      isDS + " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom,
      nds_ang, ds_bins_ang, "COLZTEXT");
   c1->Print("Plots/Systematics/syst_ds_all_2d.pdf");

   draw.CalculateSystematic(data, anti_data, mc, anti_mc, momentum, angle,
      isBr + " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom,
      nbr_ang, br_bins_ang, "COLZTEXT");
   c1->Print("Plots/Systematics/syst_br_all_2d.pdf");

   /****************************************************************************
      Different particles.
   ****************************************************************************/

   // Electrons
   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Matching Efficiency");
   draw.DrawCombinedEfficiency(data, anti_data, momentum, isDS + isEl +
      " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom, "", "Data");
   draw.DrawCombinedEfficiency(mc, anti_mc, momentum, isDS + isEl +
      " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom, "same", "MC");
   c1->Print("Plots/Systematics/eff_ds_ele_mom.pdf");

   draw.SetLegendPos("br");
   draw.DrawCombinedEfficiency(data, anti_data, momentum, isBr + isEl +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, "", "Data");
   draw.DrawCombinedEfficiency(mc, anti_mc, momentum, isBr + isEl +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, "same", "MC");
   c1->Print("Plots/Systematics/eff_br_ele_mom.pdf");

   draw.SetTitleY("Systematic Uncertainty");
   draw.CalculateSystematic(data, anti_data, mc, anti_mc, momentum, isDS +
      isEl +  " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom);
   c1->Print("Plots/Systematics/syst_ds_ele_mom.pdf");

   draw.CalculateSystematic(data, anti_data, mc, anti_mc, momentum, isBr +
      isEl +  " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom);
   c1->Print("Plots/Systematics/syst_br_ele_mom.pdf");

   // Muons
   draw.SetLegendPos("br");
   draw.SetTitleY("Matching Efficiency");
   draw.DrawCombinedEfficiency(data, anti_data, momentum, isDS + isMu +
      " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom, "", "Data");
   draw.DrawCombinedEfficiency(mc, anti_mc, momentum, isDS + isMu +
      " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom, "same", "MC");
   c1->Print("Plots/Systematics/eff_ds_mu_mom.pdf");

   draw.SetLegendPos("br");
   draw.DrawCombinedEfficiency(data, anti_data, momentum, isBr + isMu +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, "", "Data");
   draw.DrawCombinedEfficiency(mc, anti_mc, momentum, isBr + isMu +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, "same", "MC");
   c1->Print("Plots/Systematics/eff_br_mu_mom.pdf");

   draw.SetTitleY("Systematic Uncertainty");
   draw.CalculateSystematic(data, anti_data, mc, anti_mc, momentum, isDS +
      isMu +  " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom);
   c1->Print("Plots/Systematics/syst_ds_mu_mom.pdf");

   draw.CalculateSystematic(data, anti_data, mc, anti_mc, momentum, isBr +
      isMu +  " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom);
   c1->Print("Plots/Systematics/syst_br_mu_mom.pdf");

   // Protons
   draw.SetLegendPos("br");
   draw.DrawCombinedEfficiency(data, anti_data, momentum, isDS + isProt +
      " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom, "", "Data");
   draw.DrawCombinedEfficiency(mc, anti_mc, momentum, isDS + isProt +
      " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom, "same", "MC");
   c1->Print("Plots/Systematics/eff_ds_prot_mom.pdf");

   draw.SetLegendPos("br");
   draw.DrawCombinedEfficiency(data, anti_data, momentum, isBr + isProt +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, "", "Data");
   draw.DrawCombinedEfficiency(mc, anti_mc, momentum, isBr + isProt +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, "same", "MC");
   c1->Print("Plots/Systematics/eff_br_prot_mom.pdf");

   draw.SetTitleY("Systematic Uncertainty");
   draw.CalculateSystematic(data, anti_data, mc, anti_mc, momentum, isDS +
      isProt +  " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom);
   c1->Print("Plots/Systematics/syst_ds_prot_mom.pdf");

   draw.CalculateSystematic(data, anti_data, mc, anti_mc, momentum, isBr +
      isProt +  " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom);
   c1->Print("Plots/Systematics/syst_br_prot_mom.pdf");

   /****************************************************************************
      True matching.
      What was "cut" is now signal. Confusing.
   ****************************************************************************/

   draw.SetLegendPos("br");
   draw.SetTitleY("Matching Efficiency");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.DrawEfficiency(mc, momentum, isDS + "&& TrueDs", isDS +
      " && ecaldet==9", nds_mom, ds_bins_mom, "", "#nu MC");
   draw.DrawEfficiency(anti_mc, momentum, isDS + "&& TrueDs", isDS +
      "&& ecaldet==9", nds_mom, ds_bins_mom, "same", "#bar{#nu} MC");
   c1->Print("Plots/Others/true_eff_ds_all_mom.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("cosine(Track Angle)");
   draw.DrawEfficiency(mc, angle, isDS + "&& TrueDs", isDS + " && ecaldet==9",
      nds_ang, ds_bins_ang, "", "#nu MC");
   draw.DrawEfficiency(anti_mc, angle, isDS + "&& TrueDs", isDS +
      " && ecaldet==9", nds_ang, ds_bins_ang, "same", "#bar{#nu} MC");
   c1->Print("Plots/Others/true_eff_ds_all_ang.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.DrawEfficiency(mc, momentum, isBr + "&& TrueBarrel", isBr +
      " && ecaldet<9 && ecaldet>4", nbr_mom, br_bins_mom, "", "#nu MC");
   draw.DrawEfficiency(anti_mc, momentum, isBr + "&& TrueBarrel", isBr +
      "&& ecaldet<9 && ecaldet>4", nbr_mom, br_bins_mom, "same", "#bar{#nu} MC");
   c1->Print("Plots/Others/true_eff_br_all_mom.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("cosine(Track Angle)");
   draw.DrawEfficiency(mc, angle, isBr + "&& TrueBarrel", isBr +
      "&& ecaldet<9 && ecaldet>4", nbr_ang, br_bins_ang, "", "#nu MC");
   draw.DrawEfficiency(anti_mc, angle, isBr + "&& TrueBarrel", isBr +
      "&& ecaldet<9 && ecaldet>4", nbr_ang, br_bins_ang, "same", "#bar{#nu} MC");
   c1->Print("Plots/Others/true_eff_br_all_ang.pdf");

   /****************************************************************************
      Event weights. For combining histograms.
   ****************************************************************************/

   draw.SetLegendPos("br");
   draw.SetTitleY("Event Weight");
   draw.DrawRatio(data, anti_data, momentum, nds_mom, ds_bins_mom, isDS, isDS,
      1, "", "e1", "Data");      
   draw.DrawRatio(mc, anti_mc, momentum, nds_mom, ds_bins_mom, isDS, isDS,
      1, "same", "e1", "MC");
   c1->Print("Plots/Others/ratio_ds_all_mom.pdf");

   draw.SetLegendPos("br");
   draw.DrawRatio(data, anti_data, momentum, nbr_mom, br_bins_mom, isBr, isBr,
      1, "", "e1", "Data");
   draw.DrawRatio(mc, anti_mc, momentum, nbr_mom, br_bins_mom, isBr, isBr,
      1, "same", "e1", "MC");
   c1->Print("Plots/Others/ratio_br_all_mom.pdf");

   c1->Close();

   /****************************************************************************
      Table of numbers.
   ****************************************************************************/

   double xbins[2] = {-100000, 100000};
   vector<double> temp(1);

   std::cout.setf(ios::fixed,ios::floatfield);
   std::cout.precision(3);

   double dataeff = draw.GetEfficiencyVsBin(data, momentum, isBr +
      " && EnterBarrel && !EnterDs", recoBr, 1, xbins, &temp).at(0);
   double dataerr = temp.at(0);
   double mceff = draw.GetEfficiencyVsBin(mc, momentum, isBr +
      " && EnterBarrel && !EnterDs", recoBr, 1, xbins, &temp).at(0);
   double mcerr = temp.at(0);
   double syst = sqrt((dataeff - mceff) * (dataeff - mceff) + mcerr * mcerr +
      dataerr*dataerr);

   std::cout << "All (neutrino only), Barrel: " << std::endl;
   std::cout << "Data eff = " << dataeff << " +/- " << dataerr << std::endl;
   std::cout << "MC eff = " << mceff << " +/- " << mcerr << std::endl;
   std::cout << "Syst = " << syst << std::endl << std::endl;

   double dataeff = draw.GetEfficiencyVsBin(data, momentum, isDS +
      " && !EnterBarrel && EnterDs", recoDS, 1, xbins, &temp).at(0);
   double dataerr = temp.at(0);
   double mceff = draw.GetEfficiencyVsBin(mc, momentum, isDS +
      " && !EnterBarrel && EnterDs", recoDS, 1, xbins, &temp).at(0);
   double mcerr = temp.at(0);
   double syst = sqrt((dataeff - mceff) * (dataeff - mceff) + mcerr * mcerr +
      dataerr*dataerr);

   std::cout << "All (neutrino only), DS: " << std::endl;
   std::cout << "Data eff = " << dataeff << " +/- " << dataerr << std::endl;
   std::cout << "MC eff = " << mceff << " +/- " << mcerr << std::endl;
   std::cout << "Syst = " << syst << std::endl << std::endl;

   double dataeff = draw.GetCombinedEfficiency(data, anti_data, momentum, isBr +
      " && EnterBarrel && !EnterDs", recoBr, 1, xbins, &temp).at(0);
   double dataerr = temp.at(0);
   double mceff = draw.GetCombinedEfficiency(mc, anti_mc, momentum, isBr +
      " && EnterBarrel && !EnterDs", recoBr, 1, xbins, &temp).at(0);
   double mcerr = temp.at(0);
   double syst = sqrt((dataeff - mceff) * (dataeff - mceff) + mcerr * mcerr +
      dataerr*dataerr);

   std::cout << "All, Barrel: " << std::endl;
   std::cout << "Data eff = " << dataeff << " +/- " << dataerr << std::endl;
   std::cout << "MC eff = " << mceff << " +/- " << mcerr << std::endl;
   std::cout << "Syst = " << syst << std::endl << std::endl;

   double dataeff = draw.GetCombinedEfficiency(data, anti_data, momentum, isDS +
      " && !EnterBarrel && EnterDs", recoDS, 1, xbins, &temp).at(0);
   double dataerr = temp.at(0);
   double mceff = draw.GetCombinedEfficiency(mc, anti_mc, momentum, isDS +
      " && !EnterBarrel && EnterDs", recoDS, 1, xbins, &temp).at(0);
   double mcerr = temp.at(0);
   double syst = sqrt((dataeff - mceff) * (dataeff - mceff) + mcerr * mcerr +
      dataerr*dataerr);

   std::cout << "All, DS: " << std::endl;
   std::cout << "Data eff = " << dataeff << " +/- " << dataerr << std::endl;
   std::cout << "MC eff = " << mceff << " +/- " << mcerr << std::endl;
   std::cout << "Syst = " << syst << std::endl << std::endl;

   double dataeff = draw.GetCombinedEfficiency(data, anti_data, momentum, isBr +
      isEl + " && EnterBarrel && !EnterDs", recoBr, 1, xbins, &temp).at(0);
   double dataerr = temp.at(0);
   double mceff = draw.GetCombinedEfficiency(mc, anti_mc, momentum, isBr +
      isEl + " && EnterBarrel && !EnterDs", recoBr, 1, xbins, &temp).at(0);
   double mcerr = temp.at(0);
   double syst = sqrt((dataeff - mceff) * (dataeff - mceff) + mcerr * mcerr +
      dataerr*dataerr);

   std::cout << "Electrons, Barrel: " << std::endl;
   std::cout << "Data eff = " << dataeff << " +/- " << dataerr << std::endl;
   std::cout << "MC eff = " << mceff << " +/- " << mcerr << std::endl;
   std::cout << "Syst = " << syst << std::endl << std::endl;

   double dataeff = draw.GetCombinedEfficiency(data, anti_data, momentum, isDS +
      isEl + " && !EnterBarrel && EnterDs", recoDS, 1, xbins, &temp).at(0);
   double dataerr = temp.at(0);
   double mceff = draw.GetCombinedEfficiency(mc, anti_mc, momentum, isDS +
      isEl + " && !EnterBarrel && EnterDs", recoDS, 1, xbins, &temp).at(0);
   double mcerr = temp.at(0);
   double syst = sqrt((dataeff - mceff) * (dataeff - mceff) + mcerr * mcerr +
      dataerr * dataerr);

   std::cout << "Electrons, DS: " << std::endl;
   std::cout << "Data eff = " << dataeff << " +/- " << dataerr << std::endl;
   std::cout << "MC eff = " << mceff << " +/- " << mcerr << std::endl;
   std::cout << "Syst = " << syst << std::endl << std::endl;

   double dataeff = draw.GetCombinedEfficiency(data, anti_data, momentum, isBr +
      isMu + " && EnterBarrel && !EnterDs", recoBr, 1, xbins, &temp).at(0);
   double dataerr = temp.at(0);
   double mceff = draw.GetCombinedEfficiency(mc, anti_mc, momentum, isBr +
      isMu + " && EnterBarrel && !EnterDs", recoBr, 1, xbins, &temp).at(0);
   double mcerr = temp.at(0);
   double syst = sqrt((dataeff - mceff) * (dataeff - mceff) + mcerr * mcerr +
      dataerr * dataerr);

   std::cout << "Muons, Barrel: " << std::endl;
   std::cout << "Data eff = " << dataeff << " +/- " << dataerr << std::endl;
   std::cout << "MC eff = " << mceff << " +/- " << mcerr << std::endl;
   std::cout << "Syst = " << syst << std::endl << std::endl;

   double dataeff = draw.GetCombinedEfficiency(data, anti_data, momentum, isDS +
      isMu + " && !EnterBarrel && EnterDs", recoDS, 1, xbins, &temp).at(0);
   double dataerr = temp.at(0);
   double mceff = draw.GetCombinedEfficiency(mc, anti_mc, momentum, isDS +
      isMu + " && !EnterBarrel && EnterDs", recoDS, 1, xbins, &temp).at(0);
   double mcerr = temp.at(0);
   double syst = sqrt((dataeff - mceff) * (dataeff - mceff) + mcerr * mcerr +
      dataerr * dataerr);

   std::cout << "Muons, DS: " << std::endl;
   std::cout << "Data eff = " << dataeff << " +/- " << dataerr << std::endl;
   std::cout << "MC eff = " << mceff << " +/- " << mcerr << std::endl;
   std::cout << "Syst = " << syst << std::endl << std::endl;

   /****************************************************************************
      Numbers in terms of momentum and angle.
   ****************************************************************************/

   std::cout << std::endl << std::endl;

   std::cout.setf(ios::fixed, ios::floatfield);
   std::cout.precision(3);
   double syst;

   vector<double> MC_Eff_Err(nds_mom);
   vector<double> MC_Eff = draw.GetEfficiencyVsBin(mc, momentum, isDS +
      " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom, &MC_Eff_Err);
   vector<double> Data_Eff_Err(nds_mom);
   vector<double> Data_Eff = draw.GetEfficiencyVsBin(data, momentum, isDS +
      " && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom, &Data_Eff_Err);

   for(int i=0; i<nds_mom; i++)
   {
      std::cout << "For bin: " << ds_bins_mom[i] << " - " << ds_bins_mom[i+1] <<
         std::endl;
      std::cout << "Data eff = " << Data_Eff[i] << " +/- " << Data_Eff_Err[i] <<
         std::endl;
      std::cout << "MC eff = " << MC_Eff[i] << " +/- " << MC_Eff_Err[i] <<
         std::endl;
      syst = sqrt((Data_Eff[i] - MC_Eff[i]) * (Data_Eff[i] - MC_Eff[i]) +
         MC_Eff_Err[i] * MC_Eff_Err[i] + Data_Eff_Err[i] * Data_Eff_Err[i]);
      std::cout << "Syst = " << syst << std::endl << std::endl;
   }

   MC_Eff_Err.resize(nds_ang);
   MC_Eff = draw.GetEfficiencyVsBin(mc, angle, isDS +
      " && !EnterBarrel && EnterDs", recoDS, nds_ang, ds_bins_ang, &MC_Eff_Err);
   Data_Eff_Err.resize(nds_ang);
   Data_Eff = draw.GetEfficiencyVsBin(data, angle, isDS +
      " && !EnterBarrel && EnterDs", recoDS, nds_ang, ds_bins_ang, &Data_Eff_Err);

   for(int i=0; i<nds_ang; i++)
   {
      std::cout << "For bin: " << ds_bins_ang[i] << " - " << ds_bins_ang[i+1] <<
         std::endl;
      std::cout << "Data eff = " << Data_Eff[i] << " +/- " << Data_Eff_Err[i] <<
         std::endl;
      std::cout << "MC eff = " << MC_Eff[i] << " +/- " << MC_Eff_Err[i] <<
         std::endl;
      syst = sqrt((Data_Eff[i] - MC_Eff[i]) * (Data_Eff[i] - MC_Eff[i]) +
         MC_Eff_Err[i] * MC_Eff_Err[i] + Data_Eff_Err[i] * Data_Eff_Err[i]);
      std::cout << "Syst = " << syst << std::endl << std::endl;
   }

   MC_Eff_Err.resize(nbr_mom);
   MC_Eff = draw.GetEfficiencyVsBin(mc, momentum, isBr +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, &MC_Eff_Err);
   Data_Eff_Err.resize(nbr_mom);
   Data_Eff = draw.GetEfficiencyVsBin(data, momentum, isBr +
      " && EnterBarrel && !EnterDs", recoBr, nbr_mom, br_bins_mom, &Data_Eff_Err);

   for(int i=0; i<nbr_mom; i++)
   {
      std::cout << "For bin: " << br_bins_mom[i] << " - " << br_bins_mom[i+1] <<
         std::endl;
      std::cout << "Data eff = " << Data_Eff[i] << " +/- " << Data_Eff_Err[i] <<
         std::endl;
      std::cout << "MC eff = " << MC_Eff[i] << " +/- " << MC_Eff_Err[i] <<
         std::endl;
      syst = sqrt((Data_Eff[i] - MC_Eff[i]) * (Data_Eff[i] - MC_Eff[i]) +
         MC_Eff_Err[i] * MC_Eff_Err[i] + Data_Eff_Err[i] * Data_Eff_Err[i]);
      std::cout << "Syst = " << syst << std::endl << std::endl;
   }

   MC_Eff_Err.resize(nbr_ang);
   MC_Eff = draw.GetEfficiencyVsBin(mc, angle, isBr +
      " && EnterBarrel && !EnterDs", recoBr, nbr_ang, br_bins_ang, &MC_Eff_Err);
   Data_Eff_Err.resize(nbr_ang);
   Data_Eff = draw.GetEfficiencyVsBin(data, angle, isBr +
      " && EnterBarrel && !EnterDs", recoBr, nbr_ang, br_bins_ang, &Data_Eff_Err);

   for(int i=0; i<nbr_ang; i++)
   {
      std::cout << "For bin: " << br_bins_ang[i] << " - " << br_bins_ang[i+1] <<
         std::endl;
      std::cout << "Data eff = " << Data_Eff[i] << " +/- " << Data_Eff_Err[i] <<
         std::endl;
      std::cout << "MC eff = " << MC_Eff[i] << " +/- " << MC_Eff_Err[i] <<
         std::endl;
      syst = sqrt((Data_Eff[i] - MC_Eff[i]) * (Data_Eff[i] - MC_Eff[i]) +
         MC_Eff_Err[i] * MC_Eff_Err[i] + Data_Eff_Err[i] * Data_Eff_Err[i]);
      std::cout << "Syst = " << syst << std::endl << std::endl;
   }
}
