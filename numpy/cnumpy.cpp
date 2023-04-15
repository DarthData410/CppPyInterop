// file: cnumpy.cpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps.

#include "cnumpy.hpp"

static void excos(string _ivals) {
    vector<double> invals;
    cout << _ivals << endl;
    vector<double> cosvals = cnp::cos(invals);
}

int main(int argc, char *argv[])
{
    if(argc<3) {
        runtime_error("Incorrect usage. Expected ./cnumpy --<function> vals ...");
        return 1;
    }

    if(argv[1]=="--cos") {
        excos(argv[2]);
    }
    
    return 0;
}