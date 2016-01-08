{
   // Load all the parameters used for the analysis.
   // Store them in one file so they are the same for all macros.
   gROOT->ProcessLine(".x Parameters.C");

   // Make the folders to save plots into.
   system("mkdir -p Plots");
   system("mkdir -p Plots/Binning");
   system("mkdir -p Plots/Binning/BinsToUse");

   /****************************************************************************
      Find optimal binning for the analysis.
      Want biggest bins possible over which the systematic uncertainty does not
      change much.
      Without including the effects of statistics.
      Calculate the systematic as just (data_eff - mc_eff)*(data_eff - mc_eff) 
      and chose bins for the analysis where the numbers are roughly constant.
      Error bars now represent size of stastics.
   ****************************************************************************/

   draw.SetLegendSize(0.15, 0.1);

   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Efficiency");
   
   draw.DrawEfficiency(data, momentum,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_mom,
      ds_bins_mom, "e1", "Data Eff");
   draw.DrawEfficiency(mc, momentum,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_mom,
      ds_bins_mom, "e1same", "MC Eff");
   draw.SystematicBinning(data, mc, momentum,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_mom,
      ds_bins_mom, "e1same", "Uncertainty");
   c1->Print("Plots/Binning/DS_Momentum.pdf");

   draw.SetLegendPos("br");
   draw.DrawEfficiency(data, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_mom,
      br_bins_mom, "e1", "Data Eff");
   draw.DrawEfficiency(mc, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_mom,
      br_bins_mom, "e1same", "MC Eff");
   draw.SystematicBinning(data, mc, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_mom,
      br_bins_mom, "e1same", "Uncertainty");
   c1->Print("Plots/Binning/Barrel_Momentum.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("Track Angle");
   draw.DrawEfficiency(data, angle,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_ang,
      ds_bins_ang, "e1", "Data Eff");
   draw.DrawEfficiency(mc, angle,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_ang,
      ds_bins_ang, "e1same", "MC Eff");
   draw.SystematicBinning(data, mc, angle,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_ang,
      ds_bins_ang, "e1same", "Uncertainty");
   c1->Print("Plots/Binning/DS_Angle.pdf");

   draw.SetLegendPos("br");
   draw.DrawEfficiency(data, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_ang,
      br_bins_ang, "e1", "Data Eff");
   draw.DrawEfficiency(mc, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_ang,
      br_bins_ang, "e1same", "MC Eff");
   draw.SystematicBinning(data, mc, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_ang,
      br_bins_ang, "e1same", "Uncertainty");
   c1->Print("Plots/Binning/Barrel_Angle.pdf");

   draw.SetLegendPos("br");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SystematicBinning(data, mc, momentum,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_mom,
      ds_bins_mom, "e1", "DS");
   draw.SystematicBinning(data, mc, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_mom,
      br_bins_mom, "e1same", "Barrel");
   c1->Print("Plots/Binning/Momentum_Barrel.pdf");

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Angle");
   draw.SystematicBinning(data, mc, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_ang,
      br_bins_ang, "e1", "Barrel");
   draw.SystematicBinning(data, mc, angle,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_ang,
      ds_bins_ang, "e1same", "DS");
   c1->Print("Plots/Binning/Angle.pdf");

   draw.SetTitleX("Track Momentum (MeV)");
   draw.SetTitleY("Track Angle");
   draw.SetTitleZ("Systematic Uncertainty");
   draw.CalculateSystematic(data, mc, momentum, angle,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_mom,
      ds_bins_mom, nds_ang, ds_bins_ang, "COLZTEXT");
   c1->Print("Plots/Binning/DS.pdf");

   draw.SetLegendPos("br");
   draw.CalculateSystematic(data, mc, momentum, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_mom,
      br_bins_mom, nbr_ang, br_bins_ang, "COLZTEXT");
   c1->Print("Plots/Binning/Barrel.pdf");

   /****************************************************************************
      Choose the actual binning 
   ****************************************************************************/

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Angle");
   draw.SetTitleY("Uncertainty");
   draw.SystematicBinning(data, mc, angle,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_ang,
      br_bins_ang, "e1", "Barrel");
   draw.DrawCutLineVertical(0, false, "", 0.45);
   draw.DrawCutLineVertical(0.6, false, "", 0.45);
   draw.DrawCutLineVertical(1, false, "", 0.45);
   c1->Print("Plots/Binning/BinsToUse/Barrel_Angle.pdf");

   draw.SetLegendPos("tr");
   draw.SystematicBinning(data, mc, angle,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_ang,
      ds_bins_ang, "e1", "DS");
   draw.DrawCutLineVertical(0.7, false, "", 0.45);
   draw.DrawCutLineVertical(0.85, false, "", 0.45);
   draw.DrawCutLineVertical(0.95, false, "", 0.45);
   draw.DrawCutLineVertical(1, false, "", 0.45);
   c1->Print("Plots/Binning/BinsToUse/DS_Angle.pdf");

   draw.SetLegendPos("tr");
   draw.SetTitleX("Track Momentum (MeV)");
   draw.SystematicBinning(data, mc, momentum,
      "accum_level[][0]>4 && !EnterBarrel && EnterDs", recoDS, nds_mom, ds_bins_mom, "e1", "DS");
   draw.DrawCutLineVertical(0, false, "", 0.45);
   draw.DrawCutLineVertical(200, false, "", 0.45);
   draw.DrawCutLineVertical(600, false, "", 0.45);
   draw.DrawCutLineVertical(800, false, "", 0.45);
   draw.DrawCutLineVertical(3500, false, "", 0.45);
   draw.DrawCutLineVertical(5000, false, "", 0.45);
   c1->Print("Plots/Binning/BinsToUse/DS_Momentum.pdf");

   draw.SetLegendPos("tr");
   draw.SystematicBinning(data, mc, momentum,
      "accum_level[][1]>4 && EnterBarrel && !EnterDs", recoBr, nbr_mom,
      br_bins_mom, "e1", "Barrel");
   draw.DrawCutLineVertical(0, false, "", 0.45);
   draw.DrawCutLineVertical(200, false, "", 0.45);
   draw.DrawCutLineVertical(400, false, "", 0.45);
   draw.DrawCutLineVertical(600, false, "", 0.45);
   draw.DrawCutLineVertical(800, false, "", 0.45);
   draw.DrawCutLineVertical(1200, false, "", 0.45);
   draw.DrawCutLineVertical(3500, false, "", 0.45);
   draw.DrawCutLineVertical(5000, false, "", 0.45);
   c1->Print("Plots/Binning/BinsToUse/Barrel_Momentum.pdf");
}
