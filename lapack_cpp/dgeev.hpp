// file: dgeev.hpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Fortran-LAPACK interop. LAPACK is used by Python numpy 
// module. This C++ app showcases the ability to retrieve the same functionality as numpy.linalg by
// exposing the same Fortran LAPACK subroutines, as extern "C" calls. Creating a comparable C++ set of
// header files, allowing for comparison of Python numpy.linalg vs. C++, for specific LAPACK subroutines
// *****************************************************************************************************
#pragma once

#include <vector>
#include <complex>
#include "f2clapack.hpp"

using namespace std;

namespace clap {

    using namespace f2c;

    /// @brief Primaty structure used to pass data between Fortran dgeev subroutine that is part of LAPACK
    /// and C++. Stores out values from dgeev_ in oreals and oimags respectively. 
    typedef struct {
        int cols;
        int rows;
        double *matrix_data;
        double *oreals;
        double *oimags;
    } dgeevparms;

    dgeevparms newdgeevparms(int cols, int rows) {
        dgeevparms ret;
        ret.cols = cols;
        ret.rows = rows;
        ret.matrix_data = new double[ret.cols*ret.rows];
        ret.oreals = new double[ret.cols];
        ret.oimags = new double[ret.rows];
        return ret;
    }

    vector<complex<double>> dgeev(dgeevparms v) {
        vector<complex<double>> ret;

        char NO = 'N';
        fortran_int lda = v.cols;
        // unsed for current implemenation, does not return
        // left or right vectors:
        double *vl;
        double *vr;
        fortran_int ldvl=1,ldvr=1;
        // ************************
        fortran_int lwork = 6*v.cols;
        double *work = new double[lwork];
        fortran_int info;

        dgeev_(&NO,&NO,&v.cols,v.matrix_data,&lda,v.oreals,v.oimags,vl,&ldvl,vr,&ldvr,work,&lwork,&info);
        
        if(info!=0) {
            throw runtime_error("invalid work/argument issue with cla::dgeev_ fortran call. check matrix data.");
        }
        else {
            for(int i=0;i<v.cols;i++) {
                complex<double> cd;
                cd.real(v.oreals[i]);
                cd.imag(v.oimags[i]);
                ret.push_back(cd);
            }
        }

        return ret;
    }

}
