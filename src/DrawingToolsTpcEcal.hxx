#ifndef DrawingToolsTpcEcal_h
#define DrawingToolsTpcEcal_h


#include "DrawingTools.hxx"

/// The DrawingTools class provides functions for plotting information in the
/// micro-tree file. The DrawingTools can be used in interactive ROOT sessions
/// and simple ROOT macros.
///
/// There is a distinction between the code for drawing plots (this class,
/// DrawingTools) and the code for selecting the right data to plot
/// (DataSample).
///
/// A file must be specified when instantiating the DrawingTools class, as the
/// file contains important information in the "config" tree, that is used by
/// the DrawingTools (including the cuts that an analysis performed, and the
/// categories used when drawing stacked histograms.
///
/// More documentation on how to use the DrawingTools, including example
/// macros, can be found in the highLevelAnalysis package documentation (follow
/// the links from the main page).
///

class DrawingToolsTpcEcal: public DrawingTools {
public :

  DrawingToolsTpcEcal(const string& file="", bool useT2Kstyle=true);
  DrawingToolsTpcEcal(Experiment& exp, bool useT2Kstyle=true);
  virtual ~DrawingToolsTpcEcal(){}






/// Process a TH1F. E.g. draw axis, set colours, draw legends etc...
void ProcessPlot(TH1 *hist, const string opt, const string leg);

/// Process a TH2F. E.g. draw axis, set colours, draw legends etc...
void ProcessPlot(TH2 *hist, const string opt, const string leg);

/// Get the purity of a sample for a given cut. Pass error variable to get the uncertainty too.
/// This method is faster than using GetPurityVsBin(...).at(0);
double GetPurity(DataSample& mc, const string signal, const string cut, double *error = NULL);

/// Get the efficiency of a sample for a given cut. Pass error variable to get the uncertainty too.
/// This method is faster than using GetEfficiencyVsBin(...).at(0);
double GetEfficiency(DataSample& mc, const string signal, const string cut, double *error = NULL);

/// Returns a vector where the i th element is the purity of the i+1 th histogram bin for variable binning.
/// (Histogram bins are counted from 1)
vector<double> GetPurityVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, vector<double> *purerrors = 0);

/// Returns a vector where the i th element is the purity of the i+1 th histogram bin.
/// (Histogram bins are counted from 1)
vector<double> GetPurityVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, vector<double> *purerrors = 0);

/// Returns a vector where the i th element is the efficiency of the i+1 th histogram bin for variable binning.
/// (Histogram bins are counted from 1)
/// If lerr and herr are passed as arguements, then the values are filled as the errors in efficiency (low and high).
vector<double> GetEfficiencyVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, vector<double> *lerr = 0, vector<double> *herr = 0);

/// Returns a vector where the i th element is the purity of the i+1 th histogram bin.
/// (Histogram bins are counted from 1)
/// If lerr and herr are passed as arguements, then the values are filled as the errors in efficiency (low and high).
vector<double> GetEfficiencyVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, vector<double> *lerr = 0, vector<double> *herr = 0);

/// Returns a vector of vectors where each element is the efficiency for a bin of a 2D histogram.
/// Normal binning.
vector< vector<double> > GetEfficiencyVsBin(DataSample& data, const string var, const string var2, const string& signal, const string& cut, int nx, double xmin, double xmax, int ny, double ymin, double ymax, vector< vector<double> > *efferrors = 0);

/// Returns a vector of vectors where each element is the efficiency for a bin of a 2D histogram.
/// Variable binning.
vector< vector<double> > GetEfficiencyVsBin(DataSample& data, const string var, const string var2, const string& signal, const string& cut, int nx, double *xbins, int ny, double *ybins, vector< vector<double> > *efferrors = 0);

/// Returns a vector of vectors where each element is the purity for a bin of a 2D histogram.
/// Normal binning.
vector< vector<double> > GetPurityVsBin(DataSample& data, const string var, const string var2, const string& signal, const string& cut, int nx, double xmin, double xmax, int ny, double ymin, double ymax, vector< vector<double> > *purerrors = 0);

/// Returns a vector of vectors where each element is the purity for a bin of a 2D histogram.
/// Variable binning.
vector< vector<double> > GetPurityVsBin(DataSample& data, const string var, const string var2, const string& signal, const string& cut, int nx, double *xbins, int ny, double *ybins, vector< vector<double> > *purerrors = 0);

// Returns 1 - efficiency. The ith element of the vector is for the i+1th histogram bin
vector<double> GetMisEfficiencyVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, vector<double> *lerr = 0, vector<double> *herr = 0);

// Returns 1 - efficiency. The ith element of the vector is for the i+1th histogram bin
vector<double> GetMisEfficiencyVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, vector<double> *lerr = 0, vector<double> *herr = 0);

/// Vector containing purity * efficiency. Normal binning.
/// Pass optional arguement "errors" and it will be filled with the statistical errors.
vector<double> GetPurEffVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, vector<double> *errors = 0);

/// Vector containing purity * efficiency. Variable binning.
/// Pass optional arguement "errors" and it will be filled with the statistical errors.
vector<double> GetPurEffVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, vector<double> *errors = 0);

/// Draw 1D purity vs bin.
/// Errors are statistical only if errors variable isn't used. Added in quadrature with errors otherwise.
/// Calculates statistical uncertainties using binomial statistics.
void DrawPurity(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string opt = "", string leg = "", vector<double> *errors = 0);

/// Draw 1D purity vs bin.
/// Errors are statistical only if errors variable isn't used. Added in quadrature with errors otherwise.
/// Calculates statistical uncertainties using binomial statistics.
void DrawPurity(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string opt = "", string leg = "", vector<double> *errors = 0);

/// Draw 1D efficiency vs bin.
/// Errors are statistical only if errors variable isn't used. Added in quadrature with errors otherwise.
/// Calculates statistical uncertainties using binomial statistics.
void DrawEfficiency(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string opt = "", string leg = "", vector<double> *errors = 0);

/// Draw 1D efficiency vs bin.
/// Errors are statistical only if errors variable isn't used. Added in quadrature with errors otherwise.
/// Calculates statistical uncertainties using binomial statistics.
void DrawEfficiency(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string opt = "", string leg = "", vector<double> *errors = 0);

/// Draw 1 - efficiency
/// Useful for looking at the systematic uncertainty of an isolated ECal object actually having a TPC object.
/// Regular binning.
void DrawMisEfficiency(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string opt, string leg, vector<double> *errors = 0);

/// Draw 1 - efficiency
/// Useful for looking at the systematic uncertainty of an isolated ECal object actually having a TPC object.
/// Variable binning.
void DrawMisEfficiency(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string opt, string leg, vector<double> *errors = 0);

/// Draw 2D purity vs bin.
/// Errors are statistical only if errors variable isn't used. Added in quadrature with errors otherwise.
/// Calculates statistical uncertainties using binomial statistics.
void DrawPurity(DataSample& data, const string var, const string var2, const string& signal, const string& cut, int nx, double xmin, double xmax, int ny, double ymin, double ymax, string opt = "COLZ", string leg = "", vector< vector<double> > *errors = 0);

/// Draw 2D purity vs bin.
/// Errors are statistical only if errors variable isn't used. Added in quadrature with errors otherwise.
/// Calculates statistical uncertainties using binomial statistics.
void DrawPurity(DataSample& data, const string var, const string var2, const string& signal, const string& cut, int nx, double *xbins, int ny, double *ybins, string opt = "COLZ", string leg = "", vector< vector<double> > *errors = 0);

/// Draw 2D efficiency vs bin.
/// Errors are statistical only if errors variable isn't used. Added in quadrature with errors otherwise.
/// Calculates statistical uncertainties using binomial statistics.
void DrawEfficiency(DataSample& data, const string var, const string var2, const string& signal, const string& cut, int nx, double xmin, double xmax, int ny, double ymin, double ymax, string opt = "COLZ", string leg = "", vector< vector<double> > *errors = 0);

/// Draw 2D efficiency vs bin.
/// Errors are statistical only if errors variable isn't used. Added in quadrature with errors otherwise.
/// Calculates statistical uncertainties using binomial statistics.
void DrawEfficiency(DataSample& data, const string var, const string var2, const string& signal, const string& cut, int nx, double *xbins, int ny, double *ybins, string opt = "COLZ", string leg = "", vector< vector<double> > *errors = 0);

/// Draw purity * efficiency. Normal binning.
void DrawPurEff(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string opt = "", string leg = "", vector<double> *errors = 0);

/// Draw purity * efficiency. Variable binning.
void DrawPurEff(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string opt = "", string leg = "", vector<double> *errors = 0);

/// Calculate the systematic uncertainty using the difference in efficiencies.
/// 1D
/// Variable binning.
void CalculateSystematic(DataSample& data, DataSample& mc, const string var, const string signal, const string cut, int nx, double* xbins, const string opt = "", const string leg = "");

/// Calculate the systematic uncertainty using the difference in efficiencies.
/// 1D
/// Normal binning.
void CalculateSystematic(DataSample& data, DataSample& mc, const string var, const string signal, const string cut, int nx, double xmin, double xmax, const string opt = "", const string leg = "");

/// Calculate the systematic uncertainty using the difference in efficiencies.
/// 2D
/// Normal binning.
void CalculateSystematic(DataSample& data, DataSample& mc, const string var1, const string var2, const string signal, const string cut, int nx, double xmin, double xmax, int ny, double ymin, double ymax, const string opt = "COLZ", const string leg = "");

/// Calculate the systematic uncertainty using the difference in efficiencies.
/// 2D
/// Variable binning
void CalculateSystematic(DataSample& data, DataSample& mc, const string var1, const string var2, const string signal, const string cut, int nx, double *xbins, int ny, double *ybins, const string opt = "COLZ", const string leg = "");

/// Combine histograms from two different samples and draw.
/// Useful for combining two neutrino and anti-neutrino data.
/// Variable binning
void DrawCombinedHisto(DataSample& mc, DataSample& amc, const string& var, const string& cut, int nx, double* xbins, bool weight = false, const string opt = "", const string leg = "");

/// Get combined histograms from two different samples.
/// Useful for combining two neutrino and anti-neutrino data.
TH1F* GetCombinedHisto(DataSample& mc, DataSample& amc, const string& var, const string& cut, int nx, double* xbins, bool weight = false);

void DrawCombinedHisto(DataSample& mc, DataSample& amc, const string& var1, const string& var2, const string& cut, int nx, double* xbins, int ny, double* ybins, bool weight = false, const string opt = "", const string leg = "");

TH2F* GetCombinedHisto(DataSample& mc, DataSample& amc, const string& var1, const string& var2, const string& cut, int nx, double* xbins, int ny, double* ybins, bool weight = false);

vector<double> GetCombinedEfficiency(DataSample& mc, DataSample& amc, const string& var, const string& signal, const string& cut, int nx, double *xbins, vector<double> *efferrors = 0);

vector< vector<double> > GetCombinedEfficiency(DataSample& mc, DataSample& amc, const string var, const string var2, const string& signal, const string& cut, int nx, double *xbins, int ny, double *ybins, vector< vector<double> > *efferrors = 0);

void DrawCombinedEfficiency(DataSample& mc, DataSample& amc, const string var, const string& signal, const string& cut, int nx, double *xbins, string opt = "", string leg = "", vector<double> *errors = 0);

void DrawCombinedEfficiency(DataSample& mc, DataSample& amc, const string var, const string var2, const string& signal, const string& cut, int nx, double *xbins, int ny, double *ybins, string opt = "", string leg = "", vector< vector<double> > *errors = 0);

void CalculateSystematic(DataSample& data, DataSample& adata, DataSample& mc, DataSample& amc, const string var, const string signal, const string cut, int nx, double *xbins, const string opt = "", const string leg = "");

void CalculateSystematic(DataSample& data, DataSample& adata, DataSample& mc, DataSample& amc, const string var1, const string var2, const string signal, const string cut, int nx, double *xbins, int ny, double* ybins, const string opt = "", const string leg = "");

/// Look at how the systematic uncertainty changes with var.
/// Where the statistical uncertainties on the efficiencies are set to zero.
/// Useful for determining the binning in the final analysis.
void SystematicBinning(DataSample& data, DataSample& mc, const string var, const string signal, const string cut, int nx, double *xbins, const string opt = "", const string leg = "");

/// Set the title in Z
void SetTitleZ(const std::string& titleZ){_titleZ=titleZ;}
void SetMin(double min){_min = min;}
void SetMax(double max){_max = max;}
void SetRange(double min, double max){_min = min; _max = max;}
void ApplyRange(bool range){_range = range;}

protected:
  std::string _titleZ;
  bool _range;
  double _min;
  double _max;

};

#endif
