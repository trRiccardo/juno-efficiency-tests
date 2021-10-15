/*
 * Author  : K.v.Sturm
 * Date    : 30.10.2020
 * Example : Read tier1 files and draw save first n waveforms to file
 * Usage   : $ cd /folder/with/processed/file
 *           $ root -l
 *           # .L /path/to/gcu-proc/examples/plot_waveforms.C
 *           # draw_waveforms("file.root", 10, 0, 0) // plot 10 waveforms starting from 0 for channel 0
 */

#include <vector>
#include <string>
#include <fstream>

#include "TFile.h"
#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"

using namespace std;

TChain * master = nullptr;
vector<vector<double>> * wfs = nullptr;
TH1D * wf_h = nullptr;
TCanvas * can = nullptr;

void build_chain(vector<string> flist)
{
  master = new TChain("eventTree");
  for (auto f : flist) master->Add(f.c_str());
}

void plots_waveforms(string fname, int ch = 0, int n = -1, uint nsample = 304, uint e_start = 0, uint oneach = 1) {

  if (master != nullptr) {delete master; master = nullptr;}
  if (wfs    != nullptr) {delete    wfs;    wfs = nullptr;}
  if (wf_h   != nullptr) {delete   wf_h;   wf_h = nullptr;}
  if (can    != nullptr) {delete    can;    can = nullptr;}

  vector<string> v_files = {fname};
  build_chain(v_files);

  // initialize variable and set the address
  uint nch = 39;
  wfs = new vector<vector<double>>(nch, vector<double>(nsample));
  master->SetBranchAddress("waveforms", &wfs);
  uint n_evts = master->GetEntries();
  if (n < 0) n = n_evts;

gStyle->SetOptStat( "" );

  wf_h = new TH1D(Form("",ch),Form(";time [ns];ADC",ch),nsample,0,nsample);
  can = new TCanvas(Form("ch%i_%is",ch,n),Form("ch%i_%is",ch,n),800,500);

wf_h->GetXaxis()->SetTitle( "Time [ns]"  );
wf_h->GetYaxis()->SetTitle( "Voltage [ADC counts]" );
wf_h->GetXaxis()->CenterTitle( true );
wf_h->GetYaxis()->CenterTitle( true );
wf_h->GetXaxis()->SetRangeUser( 0, 300 );
wf_h->GetYaxis()->SetLimits( 9400, 12000 );

  for (int w = 0; w < n; w++) {
    if (e_start+w*oneach >= n_evts) break;
    master->GetEvent(e_start+w*oneach);
    uint i = 1;
    for (auto ww : wfs->at(ch)) wf_h->SetBinContent(i++,ww);
    while (i<=nsample) wf_h->SetBinContent(i++,0);
    wf_h->GetYaxis()->SetRangeUser(0,16000);
    if (w == 0) wf_h->DrawCopy("hist");
    else        wf_h->DrawCopy("hist same");
  }

TText* t1 = new TText( 162, 9700, "Normal acquisition" );
t1->SetTextSize( 0.035 );
t1->SetTextFont( 42 );
t1->Draw( "same" );
TText* t2 = new TText( 162, 9500, "Rate set to 10 Hz" );
t2->SetTextSize( 0.035 );
t2->SetTextFont( 42 );
t2->Draw( "same" );



}

