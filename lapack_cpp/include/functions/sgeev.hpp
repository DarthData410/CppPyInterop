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
#include "clapack.hpp"

using namespace std;

namespace clap {

    /// @brief Primaty structure used to pass data between Fortran sgeev subroutine that is part of LAPACK
    /// and C++. Stores out values from sgeev_ in oreals and oimags respectively. 
    typedef struct {
        int cols;
        int rows;
        f2c::fortran_real *matrix_data;
        f2c::fortran_real *oreals;
        f2c::fortran_real *oimags;
    } sgeevparms;

    sgeevparms newsgeevparms(int rows, int cols) {
        sgeevparms ret;
        ret.rows = rows;
        ret.cols = cols;
        ret.matrix_data = new f2c::fortran_real[ret.rows*ret.cols];
        ret.oreals = new f2c::fortran_real[ret.rows];
        ret.oimags = new f2c::fortran_real[ret.rows];
        return ret;
    }

    vector<complex<float>> sgeev(sgeevparms v) {
        vector<complex<float>> ret;

        char NO = 'N';
        f2c::fortran_int lda = v.cols;
        // unsed for current implemenation, does not return
        // left or right vectors:
        f2c::fortran_real *vl;
        f2c::fortran_real *vr;
        f2c::fortran_int ldvl=1,ldvr=1;
        // ************************
        f2c::fortran_int lwork = 6*v.cols;
        f2c::fortran_real *work = new f2c::fortran_real[lwork];
        f2c::fortran_int info;

        f2c::sgeev_(&NO,&NO,&v.cols,v.matrix_data,&lda,v.oreals,v.oimags,vl,&ldvl,vr,&ldvr,work,&lwork,&info);
        
        if(info!=0) {
            throw runtime_error("invalid work/argument issue with cla::dgeev_ fortran call. check matrix data.");
        }
        else {
            for(int i=0;i<v.cols;i++) {
                complex<float> cf;
                cf.real(v.oreals[i]);
                cf.imag(v.oimags[i]);
                ret.push_back(cf);
            }
        }

        return ret;
    }

}
