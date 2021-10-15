#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <stdint.h>
#include <vector>
#include <functional>
#include <cstring>

#include "Constants.hpp"

using namespace std;

void usage();

inline uint16_t swap_word( uint16_t word ) {
  return ( ( word << 8 ) & 0xFF00 ) | ( ( word >> 8 ) & 0x00FF );
}

int main(int argc, char* argv[]) {

  //declare useful variables
  fstream file_in;
  ofstream file_out;
  ofstream file_check;
  uint16_t val, trg_w, tmp = 0;
  const unsigned int buffer_is = 500;
  vector<uint16_t> buffer(buffer_is);
  vector<uint16_t>::iterator it;
  vector<uint64_t> trg_c;
  trg_c.reserve(100000);
  unsigned int i = 0, of = 0;
  int a_ev, e_ev, win, m = 1;
  bool found, debug = false;
 
  //some checks on input line
  if ( argc < 2 ) {
    cout << "Wrong usage!" << endl;
    usage();
    return -1;
  }
  
  if( (!strcmp( argv[1], "--help" )) || (!strcmp( argv[1], "-h" )) ) {
    usage();
    return 0;
  }

  if( argc > 3 ) {
    if( argc == 4 && !strcmp( argv[3], "-d" ) )
	debug = true;
    else if( argc > 6 && !(strcmp( argv[3], "-o" ) || strcmp( argv[5], "-x" )) ) {
      if( argc > 7 && !strcmp( argv[7], "-d" ) )
	debug = true;
      file_out.open(argv[4], ios::app);
    }
    else {
      cout << "Wrong usage!" << endl;
      usage();
      return -1;
    }
  }

  win = atoi(argv[2]);

  file_in.open(argv[1], ios::in|ios::binary);
  
  if(debug)
    file_check.open("check.txt", ios::out);

  if(!file_in.is_open())
    cout << "Error opening input file" << endl;

  double conversion = 2.0 / sizeof(char);
  if(debug)
    cout << "conversion: " <<conversion << endl;
 

  //skip first not valid data
  file_in.ignore(conversion*(win+16)*1000);

  //seek for next trailer
  do {
    found = true;
    
    //check if reading too much
    if ( m > 10 ) {
      cout << "First trailer not found" << endl;
      return -1;
    }    
    
    if(debug){
      cout << "Searching for first trailer, attempt " << m << endl;
      file_check << "--Searching for first trailer--" << endl;
    }
    
    //fill buffer with first m*buffer_is words
    while ( i < buffer.size() ) {
    
      file_in.read( reinterpret_cast<char*>( &buffer[i] ), sizeof( uint16_t ) );
      ++i;
    }
    if(debug) {
      for( auto c : buffer )
	file_check << swap_word(c) << endl;
      
      file_check << endl;
    }

    //search for trailer start sequence
    it = search( buffer.begin(), buffer.end(),
		    boyer_moore_searcher(Constants::trailer::start, Constants::trailer::start + 6 ) );
    
    //if not found retry with bigger buffer size
    if( it == buffer.end() ){
      found = false;
      buffer.resize((++m)*buffer_is);
    }
    
  }while(!found);

  cout << "First trailer found!" << endl;
  
  //set reading position to end of first useful trailer  
  file_in.seekg( conversion*( it - buffer.begin() + (win+16)*1000 ) );
  file_in.ignore( conversion*8.0);
  if(debug)
    file_check << "----------Reading data----------" << endl;

  //read file after first trailer
  while( file_in.read( reinterpret_cast<char*>( &val ), sizeof( val ) ) ) {

    if( swap_word( val ) != Constants::header::m1 ) {
      cout << "File corrupted at trg count: " << (*(trg_c.end()-1) ? *(trg_c.end()-1) : 0) << endl;
    }

    file_in.ignore( conversion*1.0 );
    //getting trigger window
    file_in.read( reinterpret_cast<char*>( &val ), sizeof( val ) );
    trg_w = swap_word( val ); 
    if(debug)
      file_check << trg_w << endl;
    
    //getting trigger count
    file_in.read( reinterpret_cast<char*>( &val ), sizeof( val ) );
    val = swap_word( val );
    trg_c.push_back( val );

    if(trg_w != win)
      cout << "Trg_win mismatch at trg_c: " << val << endl;
    
    //counting overflows
    if( val < tmp )
      ++of;

    tmp = val;
    
    if(debug)
      file_check << *(trg_c.end()-1) << endl; 
    
    //set position to next header
    if(debug)
      for(int k = 0; k < (trg_w - 1)*8 + 4; ++k) {
	file_in.read( reinterpret_cast<char*>( &val ), sizeof( val ) );
	file_check << swap_word(val) << endl;
      }
    else
      file_in.ignore( conversion*( (trg_w - 1)*8 + 4) );
  }

  if(debug) {
    file_check << "-----trigger counts found------" << endl;
    for( auto c : trg_c )
      file_check << c << endl;
    file_check.close();
  }

  //close input file
  file_in.close();
  
  //print results on screen
  a_ev = trg_c.size();
  e_ev = *(trg_c.end()-1) + of*65536 - *trg_c.begin() + 1;
  cout << "Actual read events:\t" << a_ev << endl;
  cout << "Expected events:\t" << e_ev << endl;
  cout << "Efficiency:\t\t"
       << a_ev*100.0 / e_ev << "%" << endl;

  i = 0;
  //print results on file if open
  if(file_out.is_open()) {
    file_out << atoi(argv[6]) << '\t'
	     << e_ev << '\t' << a_ev << '\t' << a_ev*100.0 / e_ev << endl;
    file_out.close();
  }
  
  return 0;
  }

