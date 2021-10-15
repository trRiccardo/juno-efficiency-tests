/*
 * Author : K.v.Sturm
 * Date   : 7. June 2021
 * Note   : Rate analysis - check trigger numbers
 *
 */

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "TH1D.h"
#include "TList.h"
#include "TFile.h"
#include "TChain.h"
#include "TString.h"
#include "TCanvas.h"
#include "TSystemDirectory.h"

using namespace std;

vector<string> list_files(const char * dirname = ".", const char * ext = ".root");

// analyze single rate measurement
void tndiff_single(int f, int nch, string dir) {

  vector<string> files = list_files(dir.c_str());	// get file list in directory

  TChain * chain = new TChain("eventTree");
  for (auto file : files) chain->Add((dir + "/" + file).c_str());

  vector<uint64_t> * timestamp  = new vector<uint64_t>(nch);
  vector<uint16_t> * trg_number = new vector<uint16_t>(nch);
  vector<uint8_t>  * errors     = new vector<uint8_t>(nch);
  int  active_ch = 0;
  chain->SetBranchAddress("timestamp",&timestamp);
  chain->SetBranchAddress("trg_number",&trg_number);
  chain->SetBranchAddress("errors",    &errors);
  chain->SetBranchAddress("active_ch",    &active_ch);

  vector<uint16_t> trg_number_before(nch,0);

  vector<uint64_t> events_lost_Nch(nch+1,0);   // event has N ch less than expected
  vector<uint64_t> events_lost_in_ch(nch,0); // n channels lost in event
  vector<uint64_t> events_lost_in_ch_counter(nch,0); // n channels lost in event
  vector<uint64_t> events_ok_in_ch(nch,0);   // n channels lost in event

  vector<TH1D*> h;
  for (int c = 0; c < nch; c++)
    h.push_back(new TH1D(Form("h_ch%i_%ikHz_delta",c,f),Form("h_ch%i_%ikHz_delta",c,f),100,0,100));

  uint64_t ne = chain->GetEntries();

  // 2048x32bit block read
  // search for the first event with all channels read
  uint64_t estart = 100;
  while (estart < ne) {
    chain->GetEntry(estart++);
    if (active_ch == nch) {
      for (int c = 0; c < nch; c++) trg_number_before.at(c) = trg_number->at(c);
      break;
    }
  }

  cout << "First event with all channels active : " << estart-1 << endl;

  int found_delta_0 = 0;

  for (uint64_t e = estart; e < ne; e++) {
    chain->GetEntry(e);
    int nch_timestamp_not_found = 0;
    for (int c = 0; c < nch; c++) {
      if (errors->at(c) == 1) {
        events_lost_in_ch_counter.at(c)++;
        nch_timestamp_not_found++;
        continue;
      }

      int delta = trg_number->at(c) - trg_number_before.at(c);
      if (delta < 0) {
        delta = std::numeric_limits<uint16_t>::max() - trg_number_before.at(c) + trg_number->at(c) + 1;
      }
      if (delta > 1000) {
        cout << "Losing a lot of events (maybe gap) : channel " << c << " " << trg_number_before.at(c) << " " << trg_number->at(c) << endl;
      }
      if (delta == 0) {
        cout << "Something went wrong, delta is zero : channel " << c << " event " << e;
        cout << " timestamp " << std::hex << timestamp->at(c) << " trg_number " << trg_number->at(c) << std::dec << endl;
        found_delta_0++;
        nch_timestamp_not_found++;
      }

      h.at(c)->Fill(delta);
      events_lost_in_ch.at(c) += delta-1;
      events_ok_in_ch.at(c)++;
      trg_number_before.at(c) = trg_number->at(c);
      events_lost_in_ch_counter.at(c) = 0;
    }
    events_lost_Nch.at(nch_timestamp_not_found)++;
    if (found_delta_0 > 10) break;
  }

  // canvas with all delta histograms
  for (int c = 0; c < nch; c++) {
    events_lost_in_ch.at(c) += events_lost_in_ch_counter.at(c);
    TFile * f1 = new TFile(Form("hist_ch%i_%s.root",c,dir.c_str()),"RECREATE");
    h.at(c)->Write();
    delete f1;
  }

  cout << "end of chain loop" << endl;

  // survival per channel and total
  ofstream file1(Form("survival_per_channel_%s.dat",dir.c_str()));
  file1 << f << "\t";
  uint64_t totevents = 0;
  uint64_t survivedevents = 0;
  uint64_t lostevents = 0;
  for (int c = 0; c < nch; c++) {
    totevents += events_ok_in_ch.at(c) + events_lost_in_ch.at(c);
    survivedevents += events_ok_in_ch.at(c);
    lostevents += events_lost_in_ch.at(c);
    file1 << (double)events_ok_in_ch.at(c)/((double)events_ok_in_ch.at(c)+(double)events_lost_in_ch.at(c)) << "\t";
    cout << c << " tot " << events_ok_in_ch.at(c)+events_lost_in_ch.at(c) << " ok " << events_ok_in_ch.at(c) << " lost " << events_lost_in_ch.at(c) << endl;
  }
  file1 << (double)survivedevents/totevents << endl;
  cout << "tot wf " << totevents << " ok " << survivedevents << endl;
  file1.close();

  // events that lost N channels
  // if all channels are lost event is not in tree!
  ofstream file2(Form("events_lost_Nch_%s.dat",dir.c_str()));
  file2 << f << "\t";
  uint64_t events_in_tree = 0;
  for (int c = 0; c < nch; c++) {
    file2 << events_lost_Nch.at(c) << "\t";
    file2 << (double)events_lost_Nch.at(c)/((double)totevents/nch) << "\t";
    events_in_tree += events_lost_Nch.at(c);
    cout << "Events that lost " << c << " channels : " << events_lost_Nch.at(c) << " fraction " << (double)events_lost_Nch.at(c)/((double)totevents/nch) << endl;
  }
  file2 << (double)totevents/nch - events_in_tree << "\t";
  cout << "Events that lost all channels : " << (double)totevents/nch - events_in_tree << " " << (double)((double)totevents/nch - events_in_tree)/totevents/nch << endl;
  file2 << (double)((double)totevents/nch - events_in_tree)/totevents/nch << endl;
  file2.close();

  delete chain;
  delete timestamp;
  delete trg_number;
  delete errors;
  for (auto e : h) delete e;
  h.clear();

  return;
}


vector<string> list_files(const char *dirname, const char *ext) {
  vector<string> rootfiles;

  TSystemDirectory dir(dirname, dirname);
  TList *files = dir.GetListOfFiles();
  if (files) {
    TSystemFile *file;
    TString fname;
    TIter next(files);
    while ((file=(TSystemFile*)next())) {
      fname = file->GetName();
      if (!file->IsDirectory() && fname.EndsWith(ext)) {
        rootfiles.push_back(fname.Data());
      }
    }
  }

  std::sort(rootfiles.begin(), rootfiles.end());

  return rootfiles;
}
