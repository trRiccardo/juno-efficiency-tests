#include "TROOT.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TPad.h"
#include "TF1.h"
#include "TStyle.h"
#include "TLatex.h"

#include <iostream>
#include <fstream>
#include <vector>

void plotEffRate() {

  // TRG WINDOW = 40
  std::ifstream iFile;
  iFile.open( "rateCH_win40.txt" );
  int n = 297;
  double x[n], y_eff[n], y_att[n], e[n];
  for( int i = 0; i < n; i++ ) {
    iFile >> x[i] >> y[i]
      >> y[i] >> e[i];
  }
  iFile.close();
  
  iFile.open( "rateGCU_win40_corrected.txt" );
  int n1 = 195;
  double x1[n1], y1_eff[n1], y1_att[n1], e1[n1];
  for( int i = 0; i < n1; i++ ) {
    iFile >> x1[i] >> y1_eff[i]
      >> y1_att[i] >> e1[i];
  }
  iFile.close();
  
  TCanvas* c = new TCanvas( "c", "c" );
  gROOT->SetStyle( "Plain" );
  gStyle->SetOptStat( "" );
  gStyle->SetOptFit( 0 );
  gStyle->SetTitleAlign( 23 );
  gStyle->SetTitleBorderSize( 0 );

  TGraph* g1 = new TGraph( , &x[0], &e[0] );
  g1->SetMarkerStyle( kFullCircle );
  g1->SetMarkerSize( 1 );
  g1->SetMarkerColor( 214 );
  TGraph* g1 = new TGraph( , &x1[0], &e1[0] );
  g1->SetMarkerStyle( kFullCircle );
  g1->SetMarkerSize( 1 );
  g1->SetMarkerColor( 214 );
  
  // da finire ...
    
  return;
  
}
