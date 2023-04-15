// file: cnumpy.cpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps.

#include "cnumpy.hpp"

int main(int argc, char *argv[])
{
    vector<double> invals;
    invals.push_back(1.0);
    invals.push_back(0.5);
    invals.push_back(1.1);
    invals.push_back(0.7);
    
    vector<double> cosvals = cnp::cos(invals);

    cout << "in-values: [";
    cout << invals[0];
    cout << ", ";
    cout << invals[1];
    cout << ", ";
    cout << invals[2];
    cout << ", ";
    cout << invals[3];
    cout << "]" << endl;

    cout << "cos-values: [";
    cout << cosvals[0];
    cout << ", ";
    cout << cosvals[1];
    cout << ", ";
    cout << cosvals[2];
    cout << ", ";
    cout << cosvals[3];
    cout << "]" << endl;
    

    return 0;
}