#include <vector>
#include <string>
#include <fstream>
#include <math.h>

#include "TFile.h"
#include "TChain.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TCanvas.h"

void macroCheck( std::string filename ) {

  // first graph
  TCanvas* c = new TCanvas( "c1", "c1", 400, 400 );

  TFile* f = TFile::Open( filename.c_str() );
  TTree* tree = ( TTree* ) ( f->Get( "eventTree" ) );
  int n = tree->Draw( "trg_number:Entry$", "", "goff" );
  TGraph* g = new TGraph( n, tree->GetV2(), tree->GetV1() );
  
  g->GetXaxis()->SetTitle( "Event number" );
  g->GetYaxis()->SetTitle( "Trigger number" );
  g->GetXaxis()->CenterTitle( true );
  g->GetYaxis()->CenterTitle( true );
  g->GetXaxis()->SetLimits( -40000, 530000 );
  g->GetYaxis()->SetRangeUser( -3500, 69000 );
  g->SetMarkerColor( 60 );
  g->SetTitle( "" );
  g->SetMarkerColor( 4 );
  
  g->Draw( "ap" );  
  
  // second graph
  TCanvas* c2 = new TCanvas( "c2", "c2", 400, 400 );
  c2->cd();
  
  TChain* ch = new TChain( "eventTree" );
  ch->Add( filename.c_str() );
  double timestamp;
  ch->SetBranchAddress( "ref_timestamp", &timestamp );
  
  Int_t nEv = 500000;
  Double_t ts[n];
  Double_t x[n];
  for( int i = 0; i < n; i++) {
    ch->GetEntry( i ); 
    ts[i] = timestamp * 8. / pow(10,9);
    x[i] = i;
  }
 
  TGraph* g2 = new TGraph( nEv, x, ts);
  
  g2->GetXaxis()->SetTitle( "Event number" );
  g2->GetYaxis()->SetTitle( "Timestamp [ns]" );
  g2->GetXaxis()->CenterTitle( true );
  g2->GetYaxis()->CenterTitle( true );
  g2->GetXaxis()->SetLimits( -40000, 550000 );
  //g2->GetYaxis()->SetRangeUser( -3500, 69000 );
  g2->SetMarkerColor( 60 );
  g2->SetTitle( "" );
  g2->SetMarkerColor( 4 );
  
  g2->Draw( "ap" ); 


  return;
}
