// file: dgeev.hpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Fortran-LAPACK interop. LAPACK is used by Python numpy 
// module. This C++ app showcases the ability to retrieve the same functionality as numpy.linalg by
// exposing the same Fortran LAPACK subroutines, as extern "C" calls. Creating a comparable C++ set of
// header files, allowing for comparison of Python numpy.linalg vs. C++, for specific LAPACK subroutines
// *****************************************************************************************************
#include <iostream>
#include <fstream>
#include "clap.hpp"

clap::dgeevparms dgeevparms_fromfile(string fn) {
    clap::dgeevparms ret;
    int rows,cols,idx;
    double fd;
    ifstream file(fn);
    if(!file.is_open()) {
        cout << " file: " << fn << " does not exit or could not be openned." << endl;
        throw runtime_error("fatal error in opening matrix file. dgeevparms_fromfile().");
    }
    else {
        
        try
        {
            // retrieve rows x columns as expected format: 2 2, etc.
            file >> rows;
            file >> cols;
            
            // initialize clap::dgeevparms:
            ret = clap::newdgeevparms(rows,cols);

            // linearize, general square matrix data:
            for(int x=0;x<rows;x++) {
                for(int y=0;y<cols;y++) {
                    file >> fd;
                    idx = y*rows+x;
                    ret.matrix_data[idx] = fd;
                }
            }

            file.close();
            return ret;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            throw runtime_error("fatal error reading matrix file. dgeevparms_fromfile.");
        } 
    }
}

int main(int argc, char *argv[]) {
    clap::dgeevparms vp;
    string fn;

    if(argc<2) {
        cout << " expected" << argv[0] << " <matrix filename>" << endl;
        return 1;
    }
    else {
        fn = argv[1];
        try
        {
            vp = dgeevparms_fromfile(fn);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return -1;
        }
    }

    cout << " --- EIGVALS (clap::dgeev) ---" << endl;
    vector<complex<double>> eigvals = clap::dgeev(vp);
    for(complex<double> cd : eigvals) {
        cout << " [" << to_string(cd.real());
        cout << ":" << to_string(cd.imag()) << "] \n";
    }

    cout << "\n For file: " << fn << endl;
    cout << " Matrix size: " << vp.rows << "x" << vp.cols << endl;
    cout << " Linearized Matrix Data Used: " << endl;
    cout << " [";
    string lmds;
    for(int i=0;i<(vp.rows*vp.cols);i++) {
        lmds += to_string(vp.matrix_data[i]);
        lmds += ",";
    }
    lmds = lmds.substr(0,lmds.size()-1);
    cout << lmds << "]" << endl;
    
    return 0;
}