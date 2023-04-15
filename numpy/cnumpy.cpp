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

        cerr << "expected usage: ./cnumpy --<function> <vals> ..." << endl;
        return 1;
    }

    string func = argv[1];
    if(func=="--cos") {
        string vals = argv[2];
        excos(vals);
    }
    
    return 0;
}