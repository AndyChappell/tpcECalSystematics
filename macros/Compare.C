{
   // Load all the parameters used for the analysis.
   // Store them in one file so they are the same for all macros.
   gROOT->ProcessLine(".x Parameters.C");

   // Make the folders to save plots into.
   system("mkdir -p Plots");
   system("mkdir -p Plots/Nu");
   system("mkdir -p Plots/BarNu");
   system("mkdir -p Plots/Data");
   system("mkdir -p Plots/MC");
   system("mkdir -p Plots/Comparison");

   /****************************************************************************
      Check that the neutrino data and anti-neutrino data behave the same
      so that they can be combined together to increase the sample size.
      The analysis is statistically limited so this is potentially important.
      If the efficiencies are similar then the samples can be combined.
   ****************************************************************************/

   /****************************************************************************
      Check data/mc agreement for neutrino mode.
   ****************************************************************************/

   //draw.SetLegendSize(0.25, 0.35);
   //draw.SetLegendPos(0.7, 0.6);

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Counts/Bin");
   draw.Draw(data, mc, momentum, nds_mom, ds_bins_mom, "particle", isDS);
   c1->Print("Plots/Nu/ds_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(data, mc, angle, nds_ang, ds_bins_ang, "particle", isDS);
   c1->Print("Plots/Nu/ds_ang.pdf");

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.Draw(data, mc, momentum, nbr_mom, br_bins_mom, "particle", isBr);
   c1->Print("Plots/Nu/br_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(data, mc, angle, nbr_ang, br_bins_ang, "particle", isBr);
   c1->Print("Plots/Nu/br_ang.pdf");

   draw.SetLegendSize(0.15, 0.1);

   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Matching Efficiency");
   draw.DrawEfficiency(data, momentum,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_mom,
      ds_bins_mom, "", "#bar{#nu} Data");
   draw.DrawEfficiency(mc, momentum,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_mom,
      ds_bins_mom, "same", "#bar{#nu} MC");
   c1.Print("Plots/Nu/eff_ds_mom.pdf");

   draw.SetLegendPos("br");
   draw.DrawEfficiency(data, momentum,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_mom,
      br_bins_mom, "", "#bar{#nu} Data");
   draw.DrawEfficiency(mc, momentum,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_mom,
      br_bins_mom, "same", "#bar{#nu} MC");
   c1.Print("Plots/Nu/eff_br_mom.pdf");

   /****************************************************************************
      Check data/mc agreement for anti-neutrino.
   ****************************************************************************/

   draw.SetLegendSize(0.3, 0.4);

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Counts/Bin");
   draw.Draw(anti_data, anti_mc, momentum, nds_mom, ds_bins_mom, "particle",
      isDS);
   c1->Print("Plots/BarNu/ds_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(anti_data, anti_mc, angle, nds_ang, ds_bins_ang, "particle", isDS);
   c1->Print("Plots/BarNu/ds_ang.pdf");

   draw.SetLegendPos("tr");	
   draw.SetTitleX("Track Momentum (MeV)");
   draw.Draw(anti_data, anti_mc, momentum, nbr_mom, br_bins_mom, "particle",
      isBr);
   c1->Print("Plots/BarNu/br_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(anti_data, anti_mc, angle, nbr_ang, br_bins_ang, "particle", isBr);
   c1->Print("Plots/BarNu/br_ang.pdf");

   draw.SetLegendSize(0.15, 0.1);

   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Matching Efficiency");
   draw.DrawEfficiency(anti_data, momentum,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_mom,
      ds_bins_mom, "", "#bar{#nu} Data");
   draw.DrawEfficiency(anti_mc, momentum,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_mom,
      ds_bins_mom, "same", "#bar{#nu} MC");
   c1.Print("Plots/BarNu/eff_ds_mom.pdf");

   draw.SetLegendPos("br");
   draw.DrawEfficiency(anti_data, momentum,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_mom,
      br_bins_mom, "", "#bar{#nu} Data");
   draw.DrawEfficiency(anti_mc, momentum,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_mom,
      br_bins_mom, "same", "#bar{#nu} MC");
   c1.Print("Plots/BarNu/eff_br_mom.pdf");

   /****************************************************************************
      Check neutrino MC against anti-neutrino MC
   ****************************************************************************/

   draw.SetLegendSize(0.3, 0.4);

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Counts/Bin");
   draw.Draw(anti_mc, mc, momentum, nds_mom, ds_bins_mom, "particle", isDS, 1,
      "", "AREA");
   c1->Print("Plots/MC/ds_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(anti_mc, mc, angle, nds_ang, ds_bins_ang, "particle", isDS, 1, "",
      "AREA");
   c1->Print("Plots/MC/ds_ang.pdf");

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Counts/Bin");
   draw.Draw(mc, anti_mc, momentum, nbr_mom, br_bins_mom, "particle", isBr, 1,
      "", "AREA");
   c1->Print("Plots/MC/br_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(mc, anti_mc, angle, nbr_ang, br_bins_ang, "particle", isBr, 1, "",
      "AREA");
   c1->Print("Plots/MC/br_ang.pdf");

   draw.SetLegendSize(0.15, 0.1);

   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Matching Efficiency");
   draw.DrawEfficiency(mc, momentum,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_mom,
      br_bins_mom, "", "#nu MC");
   draw.DrawEfficiency(anti_mc, momentum,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_mom,
      br_bins_mom, "same", "#bar{#nu} MC");
   c1.Print("Plots/MC/eff_br_mom.pdf");

   draw.SetLegendPos("br");
   draw.DrawEfficiency(mc, momentum,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_mom,
      ds_bins_mom, "", "#nu MC");
   draw.DrawEfficiency(anti_mc, momentum,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_mom,
      ds_bins_mom, "same", "#bar{#nu} MC");
   c1.Print("Plots/MC/eff_ds_mom.pdf");

   /****************************************************************************
      Check Neutrino Data against anti-neutrino Data
   ****************************************************************************/

   draw.SetLegendSize(0.3, 0.4);

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Counts/Bin");
   draw.Draw(anti_data, data, momentum, nds_mom, ds_bins_mom, "particle", isDS,
      1, "", "AREA");
   c1->Print("Plots/Data/ds_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(data, anti_data, angle, nds_ang, ds_bins_ang, "particle", isDS, 1,
      "", "AREA");
   c1->Print("Plots/Data/ds_ang.pdf");

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.Draw(data, anti_data, momentum, nbr_mom, br_bins_mom, "particle", isBr,
      1, "", "AREA");
   c1->Print("Plots/Data/br_mom.pdf");

   draw.SetLegendPos("tl");
   draw.SetTitleX("cosine(Track Angle)");
   draw.Draw(data, anti_data, angle, nbr_ang, br_bins_ang, "particle", isBr, 1,
      "", "AREA");
   c1->Print("Plots/Data/br_ang.pdf");

   draw.SetLegendSize(0.15, 0.1);

   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Matching Efficiency");
   draw.DrawEfficiency(data, momentum,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_mom,
      ds_bins_mom, "", "#nu Data");
   draw.DrawEfficiency(anti_data, momentum,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_mom,
      ds_bins_mom, "same", "#bar{#nu} Data");
   c1.Print("Plots/Data/eff_ds_mom.pdf");

   draw.SetLegendPos("br");
   draw.DrawEfficiency(data, momentum,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_mom,
      br_bins_mom, "", "#nu Data");
   draw.DrawEfficiency(anti_data, momentum,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_mom,
      br_bins_mom, "same", "#bar{#nu} Data");
   c1.Print("Plots/Data/eff_br_mom.pdf");

   /****************************************************************************
      All four samples. With the results of combining them
   ****************************************************************************/

   draw.SetLegendSize(0.15, 0.2);

   // Efficiencies of all four samples together.
   draw.SetLegendPos("br");
   draw.DrawEfficiency(data, momentum,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_mom,
      ds_bins_mom, "", "#nu Data");
   draw.DrawEfficiency(mc, momentum,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_mom,
      ds_bins_mom, "same", "#nu MC");
   draw.DrawEfficiency(anti_data, momentum,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_mom,
      ds_bins_mom, "same", "#bar{#nu} Data");
   draw.DrawEfficiency(anti_mc, momentum,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_mom,
      ds_bins_mom, "same", "#bar{#nu} MC");
   c1->Print("Plots/Comparison/eff_ds_mom.pdf");

   draw.SetLegendPos("br");
   draw.DrawEfficiency(data, momentum,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_mom,
      br_bins_mom, "", "#nu Data");
   draw.DrawEfficiency(mc, momentum,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_mom,
      br_bins_mom, "same", "#nu MC");
   draw.DrawEfficiency(anti_data, momentum,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_mom,
      br_bins_mom, "same", "#bar{#nu} Data");
   draw.DrawEfficiency(anti_mc, momentum,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_mom,
      br_bins_mom, "same", "#bar{#nu} MC");
   c1->Print("Plots/Comparison/eff_br_mom.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("cosine(Track Angle)");
   draw.DrawEfficiency(data, angle,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_ang,
      ds_bins_ang, "", "#nu Data");
   draw.DrawEfficiency(mc, angle,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_ang,
      ds_bins_ang, "same", "#nu MC");
   draw.DrawEfficiency(anti_data, angle,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_ang,
      ds_bins_ang, "same", "#bar{#nu} Data");
   draw.DrawEfficiency(anti_mc, angle,
      "(accum_level[][0]>4 && !EnterBarrel && EnterDs)", recoDS, nds_ang,
      ds_bins_ang, "same", "#bar{#nu} MC");
   c1->Print("Plots/Comparison/eff_ds_ang.pdf");

   draw.SetLegendPos("br");
   draw.DrawEfficiency(data, angle,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_ang,
      br_bins_ang, "", "#nu Data");
   draw.DrawEfficiency(mc, angle,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_ang,
      br_bins_ang, "same", "#nu MC");
   draw.DrawEfficiency(anti_data, angle,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_ang,
      br_bins_ang, "same", "#bar{#nu} Data");
   draw.DrawEfficiency(anti_mc, angle,
      "(accum_level[][1]>4 && EnterBarrel && !EnterDs)", recoBr, nbr_ang,
      br_bins_ang, "same", "#bar{#nu} MC");
   c1->Print("Plots/Comparison/eff_br_ang.pdf");

   // The efficiencies when neutrino and anti-neutrino are combined.
   draw.SetLegendSize(0.15, 0.1);
   
   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.DrawCombinedEfficiency(data, anti_data, momentum,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs",  recoDS, nds_mom,
      ds_bins_mom, "e1", "#nu + #bar{#nu} Data");
   draw.DrawCombinedEfficiency(mc, anti_mc, momentum,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs",  recoDS, nds_mom,
      ds_bins_mom, "e1same", "#nu + #bar{#nu} MC");
   c1->Print("Plots/Comparison/ceff_ds_mom.pdf");

   draw.SetLegendPos("br");
   draw.DrawCombinedEfficiency(data, anti_data, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_mom,
      br_bins_mom, "e1", "#nu + #bar{#nu} Data");
   draw.DrawCombinedEfficiency(mc, anti_mc, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs",  recoBr, nbr_mom,
      br_bins_mom, "e1same", "#nu + #bar{#nu} MC");
   c1->Print("Plots/Comparison/ceff_br_mom.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("cosine(Track Angle)");
   draw.DrawCombinedEfficiency(data, anti_data, angle,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs",  recoDS, nds_ang,
      ds_bins_ang, "e1", "#nu + #bar{#nu} Data");
   draw.DrawCombinedEfficiency(mc, anti_mc, angle,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs",  recoDS, nds_ang,
      ds_bins_ang, "e1same", "#nu + #bar{#nu} MC");
   c1->Print("Plots/Comparison/ceff_ds_ang.pdf");

   draw.SetLegendPos("br");
   draw.DrawCombinedEfficiency(data, anti_data, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_ang,
      br_bins_ang, "e1", "#nu + #bar{#nu} Data");
   draw.DrawCombinedEfficiency(mc, anti_mc, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs",  recoBr, nbr_ang,
      br_bins_ang, "e1same", "#nu + #bar{#nu} MC");
   c1->Print("Plots/Comparison/ceff_br_ang.pdf");


   // The systematic uncertainties for combined samples, compared with uncombined.
   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Uncertainty");
   draw.CalculateSystematic(data, mc, momentum,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_mom,
      ds_bins_mom, "e1", "#nu");
   draw.CalculateSystematic(data, anti_data, mc, anti_mc, momentum,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_mom,
      ds_bins_mom, "e1same", "#nu + #bar{#nu}");
   c1.Print("Plots/Comparison/syst_ds_mom.pdf");

   draw.SetLegendPos("br");
   draw.CalculateSystematic(data, mc, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_mom,
      br_bins_mom, "e1", "#nu");
   draw.CalculateSystematic(data, anti_data, mc, anti_mc, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_mom,
      br_bins_mom, "e1same", "#nu + #bar{#nu}");
   c1.Print("Plots/Comparison/syst_br_mom.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("cosine(Track Angle)");
   draw.CalculateSystematic(data, mc, angle,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_ang,
      ds_bins_ang, "e1", "#nu");
   draw.CalculateSystematic(data, anti_data, mc, anti_mc, angle,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_ang,
      ds_bins_ang, "e1same", "#nu + #bar{#nu}");
   c1.Print("Plots/Comparison/syst_ds_ang.pdf");

   draw.SetLegendPos("br");
   draw.CalculateSystematic(data, mc, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_ang,
      br_bins_ang, "e1", "#nu");
   draw.CalculateSystematic(data, anti_data, mc, anti_mc, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_ang,
      br_bins_ang, "e1same", "#nu + #bar{#nu}");
   c1.Print("Plots/Comparison/syst_br_ang.pdf");

   // Systematic uncertainty without the affect of statistics.
   // i.e. (data_eff - mc_eff)*(data_eff - mc_eff)
   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SystematicBinning(data, mc, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_mom,
      br_bins_mom, "e1", "#nu");
   draw.SystematicBinning(anti_data, anti_mc, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_mom,
      br_bins_mom, "e1same", "#bar{#nu}");
   c1->Print("Plots/Comparison/comp_br_mom.pdf");

   draw.SetLegendPos("tr");
   draw.SystematicBinning(data, mc, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nds_mom,
      ds_bins_mom, "e1", "#nu");
   draw.SystematicBinning(anti_data, anti_mc, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nds_mom,
      ds_bins_mom, "e1same", "#bar{#nu}");
   c1->Print("Plots/Comparison/comp_ds_mom.pdf");

   draw.SetLegendPos("tr");
   draw.SetTitleX("cosine(Track Angle)");
   draw.SystematicBinning(data, mc, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_ang,
      br_bins_ang, "e1", "#nu");
   draw.SystematicBinning(anti_data, anti_mc, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_ang,
      br_bins_ang, "e1same", "#bar{#nu}");
   c1->Print("Plots/Comparison/comp_br_ang.pdf");

   draw.SetLegendPos("tr");
   draw.SystematicBinning(data, mc, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nds_ang,
      ds_bins_ang, "e1", "#nu");
   draw.SystematicBinning(anti_data, anti_mc, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nds_ang,
      ds_bins_ang, "e1same", "#bar{#nu}");
   c1->Print("Plots/Comparison/comp_ds_ang.pdf");

   c1->Close();
}
