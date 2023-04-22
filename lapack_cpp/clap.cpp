// file: dgeev.hpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Fortran-LAPACK interop. LAPACK is used by Python numpy 
// module. This C++ app showcases the ability to retrieve the same functionality as numpy.linalg by
// exposing the same Fortran LAPACK subroutines, as extern "C" calls. Creating a comparable C++ set of
// header files, allowing for comparison of Python numpy.linalg vs. C++, for specific LAPACK subroutines
// *****************************************************************************************************
#include <iostream>
#include "clap.hpp"

int main() {

    clap::dgeevparms vp = clap::newdgeevparms(2,2);
    vp.matrix_data[0] = 1.23;
    vp.matrix_data[2] = 0.081;
    vp.matrix_data[1] = 0.44;
    vp.matrix_data[3] = 0.13287;

    cout << sizeof(vp) << endl;

    cout << " --- EIGVALS (clap::dgeev) ---" << endl;
    vector<complex<double>> eigvals = clap::dgeev(vp);
    for(complex<double> cd : eigvals) {
        cout << " [" << to_string(cd.real());
        cout << ":" << to_string(cd.imag()) << "] \n";
    }
    
    return 0;
}