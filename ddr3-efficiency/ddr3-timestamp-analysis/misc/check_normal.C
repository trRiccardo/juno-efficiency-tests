#include <vector>
#include <string>
#include <fstream>
#include <math.h>

#include "TFile.h"
#include "TChain.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TCanvas.h"

void check_normal( std::string filename ) {

  // first graph
  TCanvas* c = new TCanvas( "c1", "c1", 400, 400 );

  TFile* f = TFile::Open( filename.c_str() );
  TTree* tree = ( TTree* ) ( f->Get( "eventTree" ) );
  int n = tree->Draw( "trg_number:ref_timestamp", "", "goff" );
  TGraph* g = new TGraph( n, tree->GetV2(), tree->GetV1() );
  
  g->GetXaxis()->SetTitle( "Timestamp [a.u.]" );
  g->GetYaxis()->SetTitle( "Trigger number" );
  g->GetXaxis()->CenterTitle( true );
  g->GetYaxis()->CenterTitle( true );
  //g->GetXaxis()->SetLimits( -40000, 530000 );
  g->GetYaxis()->SetRangeUser( -3500, 69000 );
  g->SetMarkerColor( 60 );
  g->SetTitle( "" );
  g->SetMarkerColor( 4 );
  
  g->Draw( "ap" );  
  
  return;
}
