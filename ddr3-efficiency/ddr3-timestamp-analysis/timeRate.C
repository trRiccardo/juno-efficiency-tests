
#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include <math.h>

#include "TFile.h"
#include "TChain.h"

void timeRate( std::string filename, int rate ) {

	TChain* c = new TChain( "eventTree" );
	c->Add( filename.c_str() );

	double timestamp, ts_0, ts_1M;
	//int expEvents;
	std::fstream oFile;

	c->SetBranchAddress( "ref_timestamp", &timestamp );
	c->GetEntry( 0 );
	ts_0 = timestamp;
	std::cout << "Timestamp 0: " << ts_0 << "	" << ts_0 * 8. / pow(10, 9) << std::endl;
	c->GetEntry( 1e6-1 );
	ts_1M = timestamp;
	std::cout << "Timestamp 1M: " << ts_1M << "   " << ts_1M * 8. / pow(10, 9) << std::endl;

	double deltaT = ts_1M - ts_0;
	std::cout << "Delta t_1M - t_0: " << deltaT << "	" << deltaT * 8. / pow(10,9) << std::endl;

	double expEvents = deltaT/1.e9;
	expEvents *= 8. * rate;
	std::cout << "Expected number of events: " << expEvents << std::endl;

	double eff = 1.e6 / expEvents;
	std::cout << "Survival fraction: " << eff << std::endl;

	oFile.open( "outEff.dat", std::ios::out | std::ios::app );
	oFile << rate << "	" << eff << std::endl;
	oFile.close();

	/*
	std::vector<uint64_t> ts;
	ts.reserve( 1e6 );
	for( int i = 0; i < 1e6; i++ ) {
		c->GetEntry( i );
		ts.push_back( timestamp );
	}

	//TH1D* h = new TH1D( "h", "deltaT", 30, 545000000, 547000000 );
	std::vector<double> dT;
	dT.reserve(1e6);

	for( int j = 1; j < ts.size(); j++ ) {
		dT.push_back( ts[j] - ts[j-1] );
	//	h->Fill( ts[j] - ts[j-1] );
	}
	//h->Draw();

	for( int t = 0; t < 50; t++ ) std::cout << dT[t] << std::endl;
	*/
	
	return;

}

