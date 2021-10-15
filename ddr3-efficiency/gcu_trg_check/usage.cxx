#include <iostream>
using namespace std;

void usage() {
    
    cout << "\n Usage: ./gcu_trigger_check <filename> <trg_win> -o <ofname> -x <value> <options> \n" << endl;
    cout << "  -h (--help)   : Print user manual\n"<< endl;
    cout << "  -o <ofname>   : Output filename\n" << endl;
    cout << "  -x <value>    : x value (frequency)\n "<< endl;
    cout << "  If output file name is given, option -x <value> is mandatory \n\n";
    cout << "  Options:\n "<< endl;
    cout << "  -d            : debug ...\n "<< endl;
    
}

