#include "DrawingToolsTpcEcal.hxx"
#include <iomanip>
#include <iostream>
#include <fstream>

#include "TF1.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TSystem.h"
#include "TPaveText.h"

DrawingToolsTpcEcal::DrawingToolsTpcEcal(const string& file, bool useT2Kstyle):
   DrawingTools(file, useT2Kstyle)
{
   _treeForSystErrors = NULL;
   _range = false;
}

DrawingToolsTpcEcal::DrawingToolsTpcEcal(Experiment& exp, bool useT2Kstyle):
   DrawingTools(exp.GetFilePath(), useT2Kstyle)
{
   _treeForSystErrors = NULL;
   _range = false;
}

// *****************************************************************************
// Useful tools.
// *****************************************************************************

/// Calculate the uncertainty based on Binomial statistics.
double GetBinomialUncertainty(double numer, double denom)
{
   if(denom == 0)
   {
      return 0;
   }

   double frac = numer / denom;
   return sqrt(frac*(1 - frac) / denom);
}

/// Calculate the uncertainty based on Poisson statistics.
double GetPoissonUncertainty(double x, double dx, double y, double dy,
   double z = 0, double dz = 0)
{
   if(dx == 0 || dy == 0)
   {
      return 0;
   }
   if(dz == 0)
   {
      return sqrt(((dx*dx)/(x*x)) + ((dy*dy)/(y*y)));
   }
   
   return sqrt(((dx*dx)/(x*x)) + ((dy*dy)/(y*y)) + ((dz*dz)/(z*z)));
}

/// Fitting functions. Only one so far!
double Line(Double_t *v, Double_t *par)
{
   Double_t fitval = par[0] + v[0] * par[1];

   return fitval;
}

void DrawingToolsTpcEcal::ProcessPlot(TH1 *hist, const string opt,
   const string leg)
{
   bool same = (ToUpper(opt).find("SAME") != string::npos);

   hist->GetXaxis()->SetTitle(_titleX.c_str());
   hist->GetYaxis()->SetTitle(_titleY.c_str());
   hist->SetTitle(_title.c_str());
   if(_range)
   {
      hist->SetAxisRange(_min, _max, "Y");
   }

   if(same)
   {
      _same_level++;
   }
   else
   {
      _same_level = 0;
   }

   hist->SetLineColor(_auto_colors[_same_level]);
   hist->SetFillColor(_auto_colors[_same_level]);
   hist->SetMarkerColor(_auto_colors[_same_level]);
   hist->SetMarkerStyle(_auto_markers[_same_level]);
   gStyle->SetOptStat(0);

   hist->Draw(opt.c_str());

   if(leg != "")
   {
      if(!same)
      {
         CreateLegend();
      }
      _legends.back()->AddEntry(hist, leg.c_str(), "LE1P");
      _legends.back()->Draw();
   }
}

void DrawingToolsTpcEcal::ProcessPlot(TH2 *hist, const string opt,
   const string leg)
{
   bool same = (ToUpper(opt).find("SAME") != string::npos);

   hist->GetXaxis()->SetTitle(_titleX.c_str());
   hist->GetYaxis()->SetTitle(_titleY.c_str());
   hist->GetZaxis()->SetTitle(_titleZ.c_str());
   hist->SetTitle(_title.c_str());
   hist->SetStats(0);

   if(same)
   {
      _same_level++;
   }
   else
   {
      _same_level = 0;
   }

   if(ToUpper(opt).find("TEXT") != string::npos)
   {
      hist->SetMarkerSize(2);
      gStyle->SetPaintTextFormat("1.2f");
      if(ToUpper(opt).find("COLZ") != string::npos)
      {
         hist->SetMarkerColor(kWhite);
         hist->SetMarkerSize(2.5);
      }
   }

   hist->Draw(opt.c_str());

   if(leg != "")
   {
      if(!same)
      {
         CreateLegend();
      }
      _legends.back()->AddEntry(hist, leg.c_str(), "LE1P");
      _legends.back()->Draw();
   }
}

double DrawingToolsTpcEcal::GetPurity(DataSample& mc, const string signal,
   const string cut, double *error)
{
   string sel = cut + " && " + signal;

   double selec = mc.GetTree()->GetEntries(sel.c_str());
   double total = mc.GetTree()->GetEntries(cut.c_str());

   // Uncertainty in purity.
   if(error)
   {
      *error = GetBinomialUncertainty(selec, total);
   }

   if(total != 0)
   {
      return selec/total;
   }
   else
   {
      return 0;
   }
}

double DrawingToolsTpcEcal::GetEfficiency(DataSample& mc, const string signal,
   const string cut, double *error)
{
   string sel = cut + " && " + signal;

   double selec = mc.GetTree()->GetEntries(sel.c_str());
   double total = mc.GetTree()->GetEntries(signal.c_str());

   // Uncertainty in purity.
   if(error)
   {
      *error = GetBinomialUncertainty(selec, total);
   }

   if(total != 0)
   {
      return selec/total;
   }
   else
   {
      return 0;
   }
}

/* The pointer to the vector is used as an arguement instead of just the vector
   so that it can be initialised in the header. This allows it to be an optional
   argument. Vectors are used instead of arrays for greater stability and
   simplicity in the more complicated functions. */
vector<double> DrawingToolsTpcEcal::GetPurityVsBin(DataSample& data,
   const string var, const string& signal, const string& cut, int nx,
   double xmin, double xmax, vector<double> *purerrors)
{
   double xbins[NMAXBINS];

   return GetPurityVsBin(data, var, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx, xmin, xmax, xbins), purerrors);
}

vector<double> DrawingToolsTpcEcal::GetPurityVsBin(DataSample& data,
   const string var, const string& signal, const string& cut, int nx,
   double *xbins, vector<double> *purerrors)
{
   if(purerrors && (signed) purerrors->size() != nx)
   {
      cout << "Passed error vector has incorrect size! Resizing..." << endl;
      purerrors->resize(nx);
   }

   vector<double> purities(nx);
   double temp = 0;
   string sel = cut + " && " + signal;

   TH1F* selec = DrawingToolsBase::GetHisto(data.GetTree(), "selec", var, nx,
      xbins, sel, "", "", 1);
   TH1F* total = DrawingToolsBase::GetHisto(data.GetTree(), "total", var, nx,
      xbins, cut, "", "", 1);

   for(int i = 1; i < nx + 1; i++)
   {
      if(total->GetBinContent(i) != 0)
      {
         purities.at(i-1) = selec->GetBinContent(i) / total->GetBinContent(i);
      }

      // Sort out errors.
      if(!purerrors)
      {
         continue;
      }
      // Uncertainty in fraction
      temp = GetBinomialUncertainty(selec->GetBinContent(i),
         total->GetBinContent(i));
      // Statistical errors only
      purerrors->at(i-1) = temp; 
   }

   return purities;
}

// NOTE!!! When using these functions you must use accum_level with the brackets
// afterwards. Like accum_level[][0]>2 or you will get the wrong results.
vector<double> DrawingToolsTpcEcal::GetEfficiencyVsBin(DataSample& data,
   const string var, const string& signal, const string& cut, int nx,
   double xmin, double xmax, vector<double> *lerr, vector<double> *herr)
{
   double xbins[NMAXBINS];

   return GetEfficiencyVsBin(data, var, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx, xmin, xmax, xbins), lerr, herr);
}

vector<double> DrawingToolsTpcEcal::GetEfficiencyVsBin(DataSample& data,
   const string var, const string& signal, const string& cut, int nx,
   double *xbins, vector<double> *lerr, vector<double> *herr)
{
   if(lerr)
   {
      lerr->resize(nx);
   }
   if(herr)
   {
      herr->resize(nx);
   }

   vector<double> efficiencies(nx);
   double temp = 0;
   string sel = cut + " && " + signal;

   TH1F* selec = DrawingToolsBase::GetHisto(data.GetTree(), "selec", var, nx,
      xbins, sel, "", "", 1);
   TH1F* total = DrawingToolsBase::GetHisto(data.GetTree(), "total", var, nx,
      xbins, signal, "", "", 1);

   for(int i = 1; i < nx + 1; i++)
   {
      if(total->GetBinContent(i) != 0)
      {
         efficiencies.at(i-1) = selec->GetBinContent(i) /
            total->GetBinContent(i);
      }

      // Sort out errors.
      if(!lerr && !herr)
      {
         continue;
      }
      // Uncertainty in fraction
      temp = GetBinomialUncertainty(selec->GetBinContent(i),
         total->GetBinContent(i));

      if(lerr && !herr)
      {
         lerr->at(i-1) = temp;
         continue;
      }

      // Efficiency cannot be > 1 or < 0 so we limit the uncertainties.
      if(temp + efficiencies.at(i-1) > 1)
      {
         herr->at(i-1) = 1 - efficiencies.at(i-1);
      }
      else
      {
         herr->at(i-1) = temp;
      }

      if(temp - efficiencies.at(i-1) < 0)
      {
         lerr->at(i-1) = efficiencies.at(i-1);
      }
      else
      {
         lerr->at(i-1) = temp;
      }
   }

   return efficiencies;
}

vector<double> DrawingToolsTpcEcal::GetMisEfficiencyVsBin(DataSample& data,
   const string var, const string& signal, const string& cut, int nx,
   double xmin, double xmax, vector<double> *lerr, vector<double> *herr)
{
   double xbins[NMAXBINS];

   return GetMisEfficiencyVsBin(data, var, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx, xmin, xmax, xbins), lerr, herr);
}

vector<double> DrawingToolsTpcEcal::GetMisEfficiencyVsBin(DataSample& data,
   const string var, const string& signal, const string& cut, int nx,
   double *xbins, vector<double> *lerr, vector<double> *herr)
{
	// Get the efficiencies
	vector<double> eff = GetEfficiencyVsBin(data, var, signal, cut, nx, xbins,
      lerr, herr);
	// Change to 1 - eff
	for(unsigned int i=0; i<eff.size(); i++)
   {
      eff.at(i) = 1 - eff.at(i);
   }

	return eff;
}

vector<double> DrawingToolsTpcEcal::GetPurEffVsBin(DataSample& data,
   const string var, const string& signal, const string& cut, int nx,
   double xmin, double xmax, vector<double> *errors)
{
   double xbins[NMAXBINS];

   return GetPurEffVsBin(data, var, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx, xmin, xmax, xbins), errors);
}

vector<double> DrawingToolsTpcEcal::GetPurEffVsBin(DataSample& data,
   const string var, const string& signal, const string& cut, int nx,
   double *xbins, vector<double> *errors)
{
   if(errors && (signed) errors->size() != nx)
   {
      cout << "Passed error vector has incorrect size! Resizing..." << endl;
      errors->resize(nx);
   }

   double temp = 0;

   // These are filled in the Get... methods.
   vector<double> eff_errs(nx);
   vector<double> pur_errs(nx);

   // Get the efficiencies and purities and fill the statistical errors.
   vector<double> eff = GetEfficiencyVsBin(data, var, signal, cut, nx, xbins,
      &eff_errs);
   vector<double> pur = GetPurityVsBin(data, var, signal, cut, nx, xbins,
      &pur_errs);

   vector<double> pureff(nx);

   for(int i = 0; i < nx; i++)
   {
      pureff.at(i) = eff.at(i)*pur.at(i);

      // Statistical errors only
      temp = GetPoissonUncertainty(eff.at(i), eff_errs.at(i), pur.at(i),
         pur_errs.at(i));
      // Statistical errors only
      errors->at(i) = temp;
   }

   return pureff;
}

vector< vector<double> > DrawingToolsTpcEcal::GetPurityVsBin(DataSample& data,
   const string var, const string var2, const string& signal, const string& cut,
   int nx, double xmin, double xmax, int ny, double ymin, double ymax,
   vector< vector<double> > *purerrors)
{
   double xbins[NMAXBINS];
   double ybins[NMAXBINS];

   return GetPurityVsBin(data, var, var2, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), ny,
      DrawingToolsBase::GetVariableBins(ny,ymin,ymax,ybins), purerrors);
}

vector< vector<double> > DrawingToolsTpcEcal::GetPurityVsBin(DataSample& data,
   const string var, const string var2, const string& signal, const string& cut,
   int nx, double *xbins, int ny, double *ybins,
   vector< vector<double> > *purerrors)
{
   vector< vector<double> > purities(nx, vector<double> (ny, 0));

   double temp = 0;
   string sel = cut + " && " + signal;
   string fvar = var2 + ":" + var;

   TH2F *selec = DrawingToolsBase::GetHisto(data.GetTree(), GetUniqueName(""),
      fvar, nx, xbins, ny, ybins, sel, "", "", 1);
   TH2F *total = DrawingToolsBase::GetHisto(data.GetTree(), GetUniqueName(""),
      fvar, nx, xbins, ny, ybins, signal, "", "", 1);

   for(int i = 1; i < nx + 1; i++)
   {
      for(int j = 1; j < ny + 1; j++)
      {
         if(total->GetBinContent(i,j) != 0)
         {
            purities.at(i-1).at(j-1) = (double) (selec->GetBinContent(i,j)) /
               (double) (total->GetBinContent(i,j));
         }

         // Sort out errors.
         if(!purerrors)
         {
            continue;
         }
         // Uncertainty in fraction
         temp = GetBinomialUncertainty(selec->GetBinContent(i, j),
            total->GetBinContent(i, j));
         // Statistical errors only
         purerrors->at(i-1).at(j-1) = temp;
      }
   }

   return purities;
}

vector< vector<double> > DrawingToolsTpcEcal::GetEfficiencyVsBin(
   DataSample& data, const string var, const string var2, const string& signal,
   const string& cut, int nx, double xmin, double xmax, int ny, double ymin,
   double ymax, vector< vector<double> > *efferrors)
{
   double xbins[NMAXBINS];
   double ybins[NMAXBINS];

   return GetEfficiencyVsBin(data, var, var2, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx, xmin, xmax, xbins), ny,
      DrawingToolsBase::GetVariableBins(ny, ymin, ymax, ybins), efferrors);
}

vector< vector<double> > DrawingToolsTpcEcal::GetEfficiencyVsBin(
   DataSample& data, const string var, const string var2, const string& signal,
   const string& cut, int nx, double *xbins, int ny, double *ybins,
   vector< vector<double> > *efferrors)
{
   vector< vector<double> > efficiencies(nx, vector<double> (ny, 0));

   double temp = 0;

   string sel = cut + " && " + signal;
   string tot = "accum_level[][0]>0 &&" + signal;
   string fvar = var2 + ":" + var;

   TH2F *selec = DrawingToolsBase::GetHisto(data.GetTree(), GetUniqueName(""), fvar, nx, xbins, ny, ybins, sel, "", "", 1);
   TH2F *total = DrawingToolsBase::GetHisto(data.GetTree(), GetUniqueName(""), fvar, nx, xbins, ny, ybins, tot, "", "", 1);

   for(int i = 1; i < nx + 1; i++)
   {
      for(int j = 1; j < ny + 1; j++)
      {
         if(total->GetBinContent(i,j) != 0)
         {
            efficiencies.at(i-1).at(j-1) = (double)(selec->GetBinContent(i,j)) /
               (double)(total->GetBinContent(i,j));
         }

         // Sort out errors.
         if(!efferrors)
         {
            continue;
         }
         // Uncertainty in fraction
         temp = GetBinomialUncertainty(selec->GetBinContent(i, j),
            total->GetBinContent(i, j));
         // Statistical errors only
         efferrors->at(i-1).at(j-1) = temp;
      }
   }

   return efficiencies;
}

void DrawingToolsTpcEcal::DrawPurity(DataSample& data, const string var,
   const string& signal, const string& cut, int nx, double xmin, double xmax,
   string opt, string leg, vector<double> *errors)
{
   double xbins[NMAXBINS];

   DrawPurity(data, var, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx, xmin, xmax, xbins), opt, leg,
      errors);
}

void DrawingToolsTpcEcal::DrawPurity(DataSample& data, const string var,
   const string& signal, const string& cut, int nx, double *xbins, string opt,
   string leg, vector<double> *errors)
{
   vector<double> fillvar(nx);
   vector<double> staterrors(nx);

   fillvar = GetPurityVsBin(data, var, signal, cut, nx, xbins, &staterrors);

   TH1F *hist = new TH1F("", "", nx, xbins);

   for(int i=1; i<=nx; i++)
   {
      hist->SetBinContent(i, fillvar.at(i-1));
      // Add systematic errors to statistical errors.
      if(!isinf(staterrors.at(i-1)))
      {
         hist->SetBinError(i, staterrors.at(i-1));
      }
      else
      {
         hist->SetBinError(i, 0);
      }
      if(errors)
      {
         hist->SetBinError(i, sqrt(hist->GetBinError(i) * hist->GetBinError(i) +
            errors->at(i)*errors->at(i-1)));
      }
   }

   hist->SetAxisRange(0,1.1,"Y");

   ProcessPlot(hist, opt, leg);
   gPad->Update();
}

void DrawingToolsTpcEcal::DrawEfficiency(DataSample& data, const string var,
   const string& signal, const string& cut, int nx, double xmin, double xmax,
   string opt, string leg, vector<double> *errors)
{
   double xbins[NMAXBINS];

   DrawEfficiency(data, var, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx, xmin, xmax, xbins), opt, leg,
      errors);
}

void DrawingToolsTpcEcal::DrawEfficiency(DataSample& data, const string var,
   const string& signal, const string& cut, int nx, double *xbins, string opt,
   string leg, vector<double> *errors)
{
   vector<double> fillvar(nx);
   vector<double> staterrors(nx);

   fillvar = GetEfficiencyVsBin(data, var, signal, cut, nx, xbins, &staterrors);

   TH1F *hist = new TH1F("", "", nx, xbins);

   for(int i = 1; i < nx + 1; i++)
   {
      hist->SetBinContent(i, fillvar.at(i-1));
      // Add systematic errors to statistical errors.
      if(!isinf(staterrors.at(i-1)))
      {
         hist->SetBinError(i, staterrors.at(i-1));
      }
      else
      {
         hist->SetBinError(i, 0);
      }
      if(errors)
      {
         hist->SetBinError(i, hist->GetBinError(i) + errors->at(i-1));
      }
   }

   hist->SetAxisRange(0,1.1,"Y");

   ProcessPlot(hist, opt, leg);
   gPad->Update();
}

void DrawingToolsTpcEcal::DrawMisEfficiency(DataSample& data, const string var,
   const string& signal, const string& cut, int nx, double xmin, double xmax,
   string opt, string leg, vector<double> *errors)
{
	double xbins[NMAXBINS];

	DrawMisEfficiency(data, var, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx, xmin, xmax, xbins), opt, leg,
      errors);
}

void DrawingToolsTpcEcal::DrawMisEfficiency(DataSample& data, const string var,
   const string& signal, const string& cut, int nx, double *xbins, string opt,
   string leg, vector<double> *errors)
{
   vector<double> fillvar(nx);
   vector<double> staterrors(nx);

   fillvar = GetMisEfficiencyVsBin(data, var, signal, cut, nx, xbins,
      &staterrors);

   TH1F *hist = new TH1F("", "", nx, xbins);

   for(int i = 1; i < nx + 1; i++)
   {
      hist->SetBinContent(i, fillvar.at(i-1));
      // Add systematic errors to statistical errors.
      if(!isinf(staterrors.at(i-1)))
      {
         hist->SetBinError(i, staterrors.at(i-1));
      }
      else
      {
         hist->SetBinError(i, 0);
      }
      if(errors)
      {
         hist->SetBinError(i, hist->GetBinError(i) + errors->at(i-1));
      }
   }

   hist->SetAxisRange(0,1.1,"Y");

   ProcessPlot(hist, opt, leg);
   gPad->Update();
}

void DrawingToolsTpcEcal::DrawPurity(DataSample& data, const string var,
   const string var2, const string& signal, const string& cut, int nx,
   double xmin, double xmax, int ny, double ymin, double ymax, string opt,
   string leg, vector< vector<double> > *errors)
{
   double xbins[NMAXBINS];
   double ybins[NMAXBINS];

   DrawPurity(data, var, var2, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx, xmin, xmax, xbins), ny,
      DrawingToolsBase::GetVariableBins(ny, ymin, ymax, ybins), opt, leg,
      errors);
}

void DrawingToolsTpcEcal::DrawPurity(DataSample& data, const string var,
   const string var2, const string& signal, const string& cut, int nx,
   double *xbins, int ny, double *ybins, string opt, string leg,
   vector< vector<double> > *errors)
{
   // Both filled in the GetPurity method.
   vector< vector<double> > fillvar;
   vector< vector<double> > staterrors(nx, vector<double> (ny, 0));

   fillvar = GetPurityVsBin(data, var, var2, signal, cut, nx, xbins, ny, ybins,
      &staterrors);

   TH2F *hist = new TH2F("", "", nx, xbins, ny, ybins);

   for(int i=1; i<=nx; i++)
   {
      for(int j=1; j<=ny; j++)
      {
         hist->SetBinContent(i, j, fillvar.at(i-1).at(j-1));
         // Add systematic errors to statistical errors.
         if(!isinf(staterrors.at(i-1).at(j-1)))
         {
            hist->SetBinError(i, j, staterrors.at(i-1).at(j-1));
         }
         else
         {
            hist->SetBinError(i, j, 0);
         }
         if(errors)
         {
            hist->SetBinError(i, j, sqrt(hist->GetBinError(i, j) *
               hist->GetBinError(i, j) + errors->at(i).at(j) *
               errors->at(i-1).at(j)));
         }
      }
   }

   ProcessPlot(hist, opt, leg);
   gPad->Update();
}

void DrawingToolsTpcEcal::DrawEfficiency(DataSample& data, const string var,
   const string var2, const string& signal, const string& cut, int nx,
   double xmin, double xmax, int ny, double ymin, double ymax, string opt,
   string leg, vector< vector<double> > *errors)
{
   double xbins[NMAXBINS];
   double ybins[NMAXBINS];

   DrawEfficiency(data, var, var2, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), ny,
      DrawingToolsBase::GetVariableBins(ny,ymin,ymax,ybins), opt, leg, errors);
}

void DrawingToolsTpcEcal::DrawEfficiency(DataSample& data, const string var,
   const string var2, const string& signal, const string& cut, int nx,
   double *xbins, int ny, double *ybins, string opt, string leg,
   vector< vector<double> > *errors)
{
   // Both filled in the GetEfficiency method.
   vector< vector<double> > fillvar;
   vector< vector<double> > staterrors(nx, vector<double> (ny, 0));

   fillvar = GetEfficiencyVsBin(data, var, var2, signal, cut, nx, xbins, ny,
      ybins, &staterrors);

   TH2F *hist = new TH2F("", "", nx, xbins, ny, ybins);

   for(int i=1; i<=nx; i++)
   {
      for(int j=1; j<=ny; j++)
      {
         hist->SetBinContent(i, j, fillvar.at(i-1).at(j-1));

         // Add systematic errors to statistical errors.
         if(!isinf(staterrors.at(i-1).at(j-1)))
         {
            hist->SetBinError(i, j, staterrors.at(i-1).at(j-1));
         }
         else
         {
            hist->SetBinError(i, j, 0);
         }

         if(errors)
         {
            hist->SetBinError(i, j, sqrt(hist->GetBinError(i, j) *
               hist->GetBinError(i, j) + errors->at(i).at(j) *
               errors->at(i-1).at(j)));
         }
      }
   }

   ProcessPlot(hist, opt, leg);
   gPad->Update();
}

void DrawingToolsTpcEcal::DrawPurEff(DataSample& data, const string var,
   const string& signal, const string& cut, int nx, double xmin, double xmax,
   string opt, string leg, vector<double> *errors)
{
   double xbins[NMAXBINS];

   DrawPurEff(data, var, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx, xmin, xmax, xbins), opt, leg,
      errors);
}

void DrawingToolsTpcEcal::DrawPurEff(DataSample& data, const string var,
   const string& signal, const string& cut, int nx, double *xbins, string opt,
   string leg, vector<double> *errors)
{
   vector<double> fillvar(nx);
   vector<double> staterrors(nx);

   fillvar = GetPurEffVsBin(data, var, signal, cut, nx, xbins, &staterrors);

   TH1F *hist = new TH1F("", "", nx, xbins);

   for(int i=1; i<=nx; i++)
   {
      hist->SetBinContent(i, fillvar.at(i-1));
      // Add systematic errors to statistical errors.
      if(!isinf(staterrors.at(i-1)))
      {
         hist->SetBinError(i, staterrors.at(i-1));
      }
      else
      {
         hist->SetBinError(i, 0);
      }
      if(errors)
      {
         hist->SetBinError(i, sqrt(hist->GetBinError(i)*hist->GetBinError(i) +
            errors->at(i)*errors->at(i-1)));
      }
   }

   hist->SetAxisRange(0,1.1,"Y");

   ProcessPlot(hist, opt, leg);
   gPad->Update();
}

void DrawingToolsTpcEcal::CalculateSystematic(DataSample& data, DataSample& mc,
   const string var, const string signal, const string cut, int nx, double xmin,
   double xmax, const string opt, const string leg)
{
   double xbins[NMAXBINS];

   CalculateSystematic(data, mc, var, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx, xmin, xmax, xbins), opt, leg);
}

void DrawingToolsTpcEcal::CalculateSystematic(DataSample& data, DataSample& mc,
   const string var, const string signal, const string cut, int nx,
   double *xbins, const string opt, const string leg)
{
   // These are filled in the GetEfficiencies methods.
   vector<double> data_errs(nx);
   vector<double> mc_errs(nx);

   vector<double> data_eff = GetEfficiencyVsBin(data, var, signal, cut, nx,
      xbins, &data_errs);
   vector<double> mc_eff = GetEfficiencyVsBin(mc, var, signal, cut, nx, xbins,
      &mc_errs);

   double FillVar = 0;

   TH1F *hist = new TH1F("", "", nx, xbins);
   for(int i = 0; i < nx; i++)
   {
      // Difference in data and MC squared + errors squared.
      FillVar = sqrt((data_eff.at(i) - mc_eff.at(i)) *
         (data_eff.at(i) - mc_eff.at(i)) + mc_errs.at(i) * mc_errs.at(i) +
         data_errs.at(i)*data_errs.at(i));

      // Not -nan or inf
      if(FillVar == FillVar && !isinf(FillVar))
      {
         hist->SetBinContent(i+1, FillVar);
         // Don't want errors drawn to systematics plots.
         hist->SetBinError(i+1, 0.0000001);
         cout << "For bin: " << xbins[i] << " - " << xbins[i + 1] <<
            ", uncertainty = " << FillVar << endl;
      }
      else
      {
         cout << "Error in bin " << xbins[i] << " - " << xbins[i + 1] <<
            " inf or nan propagated!" << endl;
         cout << "data eff = " << data_eff.at(i) << " +/- " <<
            data_errs.at(i) << ", mc eff = " << mc_eff.at(i) << " +/- " <<
            mc_errs.at(i) << endl;
         hist->SetBinContent(i+1, 0);
         hist->SetBinError(i+1, 0.0000001);
      }
   }

   hist->SetMinimum(0);
   ProcessPlot(hist, opt, leg);
   gPad->Update();
}

void DrawingToolsTpcEcal::CalculateSystematic(DataSample& data, DataSample& mc,
   const string var1, const string var2, const string signal, const string cut,
   int nx, double xmin, double xmax, int ny, double ymin, double ymax,
   const string opt, const string leg)
{
   double xbins[NMAXBINS];
   double ybins[NMAXBINS];

   CalculateSystematic(data, mc, var1, var2, signal, cut, nx,
      DrawingToolsBase::GetVariableBins(nx, xmin, xmax, xbins), ny,
      DrawingToolsBase::GetVariableBins(ny, ymin, ymax, ybins), opt, leg);
}

void DrawingToolsTpcEcal::CalculateSystematic(DataSample& data, DataSample& mc,
   const string var1, const string var2, const string signal, const string cut,
   int nx, double *xbins, int ny, double *ybins, const string opt,
   const string leg)
{
   vector< vector<double> > mc_errs(nx, vector<double>(ny));
   vector< vector<double> > data_errs(nx, vector<double>(ny));
   vector< vector<double> > mc_eff = GetEfficiencyVsBin(mc, var1, var2, signal,
      cut, nx, xbins, ny, ybins, &mc_errs);
   vector< vector<double> > data_eff = GetEfficiencyVsBin(data, var1, var2,
      signal, cut, nx, xbins, ny, ybins, &data_errs);

   double FillVar = 0;

   TH2F *hist = new TH2F("", "", nx, xbins, ny, ybins);
   for(int i=0; i<nx; i++)
   {
      for(int j=0; j<ny; j++)
      {
         // Difference in data and MC squared + errors squared.
         FillVar = sqrt((data_eff.at(i).at(j) - mc_eff.at(i).at(j)) *
            (data_eff.at(i).at(j) - mc_eff.at(i).at(j) + mc_errs.at(i).at(j) *
            mc_errs.at(i).at(j) + data_errs.at(i).at(j) *
            data_errs.at(i).at(j)));

         // Not -nan or inf
         if(FillVar == FillVar && !isinf(FillVar))
         {
            hist->SetBinContent(i+1, j+1, FillVar);
         }
         else
         {
            cout << "Error in bin " << xbins[i] << " - " << xbins[i+1] <<
               ", " << ybins[j] << " - " << ybins[j+1] <<
               " inf or nan propagated!" << endl;
            cout << "data eff = " << data_eff[i][j] << " +/- " <<
               data_errs[i][j] << ", mc eff = " << mc_eff[i][j] << " +/- " <<
               mc_errs[i][j] << endl;
            hist->SetBinContent(i+1, j+1, 0);
         }
      }
   }

   ProcessPlot(hist, opt, leg);
   hist->SetMinimum(0);
   gPad->Update();
}


// ************************************************************************************************************
// Combine two sample methods.
// ************************************************************************************************************

void DrawingToolsTpcEcal::DrawCombinedHisto(DataSample& mc, DataSample& amc,
   const string& var, const string& cut, int nx, double* xbins, bool weight,
   const string opt, const string leg)
{
   TH1F *hist = GetCombinedHisto(mc, amc, var, cut, nx, xbins, weight);
   ProcessPlot(hist, opt, leg);
   delete hist;
}

TH1F* DrawingToolsTpcEcal::GetCombinedHisto(DataSample& mc, DataSample& amc,
   const string& var, const string& cut, int nx, double* xbins, bool weight)
{
   TH1F *hist1 = DrawingToolsBase::GetHisto(mc.GetTree(), "selec", var, nx,
      xbins, cut, "", "", 1);
   TH1F *hist2 = DrawingToolsBase::GetHisto(amc.GetTree(), "selec", var, nx,
      xbins, cut, "", "", 1);

   hist1->Sumw2();
   hist2->Sumw2();

   TH1F temp = (*hist1) + (*hist2);

   TH1F *comb = new TH1F(temp);

   return comb;
}

void DrawingToolsTpcEcal::DrawCombinedHisto(DataSample& mc, DataSample& amc,
   const string& var1, const string& var2, const string& cut, int nx,
   double* xbins, int ny, double* ybins, bool weight, const string opt,
   const string leg)
{
   TH2F *hist = GetCombinedHisto(mc, amc, var1, var2, cut, nx, xbins, ny, ybins,
      weight);
   ProcessPlot(hist, opt, leg);
   delete hist;
}

TH2F* DrawingToolsTpcEcal::GetCombinedHisto(DataSample& mc, DataSample& amc,
   const string& var1, const string& var2, const string& cut, int nx,
   double* xbins, int ny, double* ybins, bool weight)
{
   string fvar = var2 + ":" + var1;
   TH2F *hist1 = DrawingToolsBase::GetHisto(mc.GetTree(), GetUniqueName(""),
      fvar, nx, xbins, ny, ybins, cut, "", "", 1);
   TH2F *hist2 = DrawingToolsBase::GetHisto(amc.GetTree(), GetUniqueName(""),
      fvar, nx, xbins, ny, ybins, cut, "", "", 1);

   hist1->Sumw2();
   hist2->Sumw2();

   TH2F temp = (*hist1) + (*hist2);

   TH2F *comb = new TH2F(temp);

   return comb;
}

vector<double> DrawingToolsTpcEcal::GetCombinedEfficiency(DataSample& mc,
   DataSample& amc, const string& var, const string& signal, const string& cut,
   int nx, double *xbins, vector<double> *efferrors)
{
   if(efferrors && (signed) efferrors->size() != nx)
   {
      cout << "Passed error vector has incorrect size! Resizing..." << endl;
      efferrors->resize(nx);
   }

   vector<double> efficiencies(nx);
   string sel = cut + " && " + signal;
   string tot = "accum_level[][0]>0 &&" + signal;

   TH1F* selec = GetCombinedHisto(mc, amc, var, sel, nx, xbins, true);
   TH1F* total = GetCombinedHisto(mc, amc, var, tot, nx, xbins, true);

   double temp = 0;
   double temp2 = 0;
   double err = 0;

   for(int i = 1; i < nx + 1; i++)
   {
      if(total->GetBinContent(i) != 0)
      {
         efficiencies.at(i-1) = selec->GetBinContent(i) /
            total->GetBinContent(i);
      }

      // Sort out errors.
      if(!efferrors)
      {
         continue;
      }
      // Uncertainty in fraction
      temp = GetBinomialUncertainty(selec->GetBinContent(i),
         total->GetBinContent(i));

      temp2 = temp * sqrt(pow(total->GetBinError(i) / total->GetBinContent(i),
         2) + pow(selec->GetBinError(i) / selec->GetBinContent(i), 2) +
         pow(selec->GetBinError(i) / (1 - selec->GetBinContent(i)), 2));
      err = sqrt(temp * temp + temp2 * temp2);
      // Statistical errors only
      efferrors->at(i-1) = err;
   }

   delete selec;
   delete total;

   return efficiencies;
}

vector< vector<double> > DrawingToolsTpcEcal::GetCombinedEfficiency(
   DataSample& mc, DataSample& amc, const string var, const string var2,
   const string& signal, const string& cut, int nx, double *xbins, int ny,
   double *ybins, vector< vector<double> > *efferrors)
{
   vector< vector<double> > efficiencies(nx, vector<double> (ny, 0));

   string sel = cut + " && " + signal;
   string tot = "accum_level[][0]>0 &&" + signal;

   TH2F* selec = GetCombinedHisto(mc, amc, var, var2, sel, nx, xbins, ny, ybins,
      true);
   TH2F* total = GetCombinedHisto(mc, amc, var, var2, tot, nx, xbins, ny, ybins,
      true);

   double temp = 0;
   double temp2 = 0;
   double err = 0;

   for(int i = 1; i < nx + 1; i++)
   {
      for(int j = 1; j < ny + 1; j++)
      {
         if(total->GetBinContent(i,j) != 0)
         {
            efficiencies.at(i-1).at(j-1) = (double)(selec->GetBinContent(i,j)) /
               (double)(total->GetBinContent(i,j));
         }

         // Sort out errors.
         if(!efferrors)
         {
            continue;
         }
         // Uncertainty in fraction
         temp = GetBinomialUncertainty(selec->GetBinContent(i, j),
            total->GetBinContent(i, j));

         temp2 = temp * sqrt(pow(total->GetBinError(i,j) /
            total->GetBinContent(i,j), 2) + pow(selec->GetBinError(i,j) /
            selec->GetBinContent(i,j), 2) + pow(selec->GetBinError(i,j) /
            (1-selec->GetBinContent(i,j)), 2));
         err = sqrt(temp * temp + temp2 * temp2);

         // Statistical errors only
         efferrors->at(i-1).at(j-1) = err;
      }
   }

   delete selec;
   delete total;

   return efficiencies;
}

void DrawingToolsTpcEcal::DrawCombinedEfficiency(DataSample& mc,
   DataSample& amc, const string var, const string& signal, const string& cut,
   int nx, double *xbins, string opt, string leg, vector<double> *errors)
{
   vector<double> fillvar(nx);
   vector<double> staterrors(nx);

   fillvar = GetCombinedEfficiency(mc, amc, var, signal, cut, nx, xbins, &staterrors);

   TH1F *hist = new TH1F("", "", nx, xbins);

   for(int i = 1; i < nx + 1; i++)
   {
      hist->SetBinContent(i, fillvar.at(i-1));
      // Add systematic errors to statistical errors.
      if(!isinf(staterrors.at(i-1)))
      {
         hist->SetBinError(i, staterrors.at(i-1));
      }
      else
      {
         hist->SetBinError(i, 0);
      }
      if(errors)
      {
         hist->SetBinError(i, hist->GetBinError(i) + errors->at(i - 1));
      }
   }

   hist->SetAxisRange(0, 1.1, "Y");

   ProcessPlot(hist, opt, leg);
   gPad->Update();
}

void DrawingToolsTpcEcal::DrawCombinedEfficiency(DataSample& mc,
   DataSample& amc, const string var, const string var2, const string& signal,
   const string& cut, int nx, double *xbins, int ny, double *ybins, string opt,
   string leg, vector< vector<double> > *errors)
{
   // Both filled in the GetEfficiency method.
   vector< vector<double> > fillvar;
   vector< vector<double> > staterrors(nx, vector<double> (ny, 0));

   fillvar = GetCombinedEfficiency(mc, amc, var, var2, signal, cut, nx, xbins,
      ny, ybins, &staterrors);

   TH2F *hist = new TH2F("", "", nx, xbins, ny, ybins);

   for(int i=1; i<=nx; i++)
   {
      for(int j=1; j<=ny; j++)
      {
         hist->SetBinContent(i, j, fillvar.at(i-1).at(j-1));

         // Add systematic errors to statistical errors.
         if(!isinf(staterrors.at(i-1).at(j-1)))
         {
            hist->SetBinError(i, j, staterrors.at(i-1).at(j-1));
         }
         else
         {
            hist->SetBinError(i, j, 0);
         }

         if(errors)
         {
            hist->SetBinError(i, j, sqrt(hist->GetBinError(i, j) *
               hist->GetBinError(i, j) + errors->at(i).at(j) *
               errors->at(i - 1).at(j)));
         }
      }
   }

   ProcessPlot(hist, opt, leg);
   gPad->Update();
}

void DrawingToolsTpcEcal::CalculateSystematic(DataSample& data,
   DataSample& adata, DataSample& mc, DataSample& amc, const string var,
   const string signal, const string cut, int nx, double *xbins,
   const string opt, const string leg)
{
   // These are filled in the GetEfficiencies methods.
   vector<double> data_errs(nx);
   vector<double> mc_errs(nx);

   vector<double> data_eff = GetCombinedEfficiency(data, adata, var, signal,
      cut, nx, xbins, &data_errs);
   vector<double> mc_eff = GetCombinedEfficiency(mc, amc, var, signal, cut, nx,
      xbins, &mc_errs);

   double FillVar = 0;

   TH1F *hist = new TH1F("", "", nx, xbins);
   for(int i = 0; i < nx; i++)
   {
      // Difference in data and MC squared + errors squared.
      FillVar = sqrt((data_eff.at(i) - mc_eff.at(i)) * (data_eff.at(i) -
         mc_eff.at(i)) + mc_errs.at(i) * mc_errs.at(i) +
         data_errs.at(i) * data_errs.at(i));

      // Not -nan or inf
      if(FillVar == FillVar && !isinf(FillVar))
      {
         hist->SetBinContent(i+1, FillVar);
         // Don't want errors drawn to systematics plots.
         hist->SetBinError(i+1, 0.0000001);
         cout << "For bin: " << xbins[i] << " - " << xbins[i + 1] <<
            ", uncertainty = " << FillVar << endl;
      }
      else
      {
         cout << "Error in bin " << xbins[i] << " - " << xbins[i + 1] <<
            " inf or nan propagated!" << endl;
         cout << "data eff = " << data_eff.at(i) << " +/- " <<
            data_errs.at(i) << ", mc eff = " << mc_eff.at(i) << " +/- " <<
            mc_errs.at(i) << endl;
         hist->SetBinContent(i+1, 0);
         hist->SetBinError(i+1, 0.0000001);
      }
   }

   hist->SetMinimum(0);
   ProcessPlot(hist, opt, leg);
   gPad->Update();
}

void DrawingToolsTpcEcal::CalculateSystematic(DataSample& data,
   DataSample& adata, DataSample& mc, DataSample& amc, const string var1,
   const string var2, const string signal, const string cut, int nx,
   double *xbins, int ny, double* ybins, const string opt, const string leg)
{
   vector< vector<double> > mc_errs(nx, vector<double>(ny));
   vector< vector<double> > data_errs(nx, vector<double>(ny));
   vector< vector<double> > mc_eff = GetCombinedEfficiency(mc, amc, var1, var2,
      signal, cut, nx, xbins, ny, ybins, &mc_errs);
   vector< vector<double> > data_eff = GetCombinedEfficiency(data, adata, var1,
      var2, signal, cut, nx, xbins, ny, ybins, &data_errs);

   double FillVar = 0;

   TH2F *hist = new TH2F("", "", nx, xbins, ny, ybins);
   for(int i=0; i<nx; i++)
   {
      for(int j=0; j<ny; j++)
      {
         // Difference in data and MC squared + errors squared.
         FillVar = sqrt((data_eff.at(i).at(j) - mc_eff.at(i).at(j)) *
            (data_eff.at(i).at(j) - mc_eff.at(i).at(j) + mc_errs.at(i).at(j) *
            mc_errs.at(i).at(j) + data_errs.at(i).at(j)*data_errs.at(i).at(j)));

         // Not -nan or inf
         if(FillVar == FillVar && !isinf(FillVar))
         {
            hist->SetBinContent(i+1, j+1, FillVar);
         }
         else
         {
            cout << "Error in bin " << xbins[i] << " - " << xbins[i+1] <<
               ", " << ybins[j] << " - " << ybins[j+1] <<
               " inf or nan propagated!" << endl;
            cout << "data eff = " << data_eff[i][j] << " +/- " <<
               data_errs[i][j] << ", mc eff = " << mc_eff[i][j] << " +/- " <<
               mc_errs[i][j] << endl;
            hist->SetBinContent(i+1, j+1, 0);
         }
      }
   }

   hist->SetMinimum(0);
   ProcessPlot(hist, opt, leg);
   gPad->Update();
}

/* Ignore the effect of statistics on the uncertainty by setting the
   uncertainties on the efficiencies to zero.
*/
void DrawingToolsTpcEcal::SystematicBinning(DataSample& data, DataSample& mc,
   const string var, const string signal, const string cut, int nx,
   double *xbins, const string opt, const string leg)
{
   vector<double> data_errs(nx);
   vector<double> mc_errs(nx);

   vector<double> data_eff = GetEfficiencyVsBin(data, var, signal, cut, nx,
      xbins, &data_errs);
   vector<double> mc_eff = GetEfficiencyVsBin(mc, var, signal, cut, nx, xbins,
      &mc_errs);

   double FillVar = 0;
   double FillErr = 0;

   TH1F *hist = new TH1F("", "", nx, xbins);
   for(int i = 0; i < nx; i++)
   {
      // Difference in data and MC squared + errors squared.
      FillVar = (data_eff.at(i) - mc_eff.at(i)) * (data_eff.at(i) -
         mc_eff.at(i));
      FillErr = sqrt(data_errs.at(i) * data_errs.at(i) + mc_errs.at(i) *
         mc_errs.at(i));

      hist->SetBinContent(i + 1, FillVar);
      hist->SetBinError(i + 1, FillErr);
   }

   hist->SetMinimum(0);
   ProcessPlot(hist, opt, leg);
   gPad->Update();
}
