{
   /****************************************************************************
      Output parameter file to be read in by psyche.
      This doesn't use the combined neutrino / anti-neutrino method as the
      method has not been peer reviewed.
      3D-binned in neutrino flavour, ECal, momentum.
   ****************************************************************************/
   // Load all the parameters used for the analysis.
   // Store them in one file so they are the same for all macros.
   gROOT->ProcessLine(".x Parameters.C");

   ofstream out("TPCECALMatchEff_p6B.dat");
   out.setf(ios::fixed, ios::floatfield);
   out.precision(3);

   // Four neutrino types
   int nnu = 4;
   double nuBins[nnu + 1] = {-14.5, -13, 0, 13, 14.5};

   // Barrel and DS
   int ndet = 2;
   double detBins[ndet + 1] = {3.5, 8.5, 9.5};

   int nx;
   double *xbins;

   string cut[] = {recoBr, recoDS};

   double dummy = 1;

   for(int inu=0; inu<nnu; inu++)
   {  // Loop over neutrino type.
      for(int idet=0; idet<ndet; idet++) 
      {
         // Must be a neater way to do this....
         string signal;

         if(idet == 0)
         {
            signal += isBr;
            nx = nbr_mom;
            xbins = br_bins_mom;
         }
         else
         {
            signal += isDS;
            nx = nds_mom;
            xbins = ds_bins_mom;
         }

         if(inu == 1 || inu == 2)
         {
            signal += isEl;
         }
         else
         {
            signal += isMu;
         }

         vector<double> mc_lerr(nx);
         vector<double> mc_herr(nx);
         vector<double> mc_eff = draw.GetEfficiencyVsBin(mc, momentum, signal,
            cut[idet], nx, xbins, &mc_lerr, &mc_herr);

         vector<double> data_lerr(nx);
         vector<double> data_herr(nx);
         vector<double> data_eff = draw.GetEfficiencyVsBin(data, momentum,
            signal, cut[idet], nx, xbins, &data_lerr, &data_herr);

         for(int imom=0; imom<nx; imom++)
         {
            out << nuBins[inu] << "\t" << nuBins[inu+1] << "\t" << detBins[idet] << "\t" << detBins[idet+1] << "\t" << xbins[imom] << "\t" << xbins[imom+1] << "\t";
            out << dummy << "\t" << mc_eff[imom] << "\t" << mc_lerr[imom] << "\t" << mc_herr[imom] << "\t";
            out << data_eff[imom] << "\t" << data_lerr[imom] << "\t" << data_herr[imom] << endl;
         }
      }
   }
}
