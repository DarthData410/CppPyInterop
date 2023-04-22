// file: dgeev.hpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Fortran-LAPACK interop. LAPACK is used by Python numpy 
// module. This C++ app showcases the ability to retrieve the same functionality as numpy.linalg by
// exposing the same Fortran LAPACK subroutines, as extern "C" calls. Creating a comparable C++ set of
// header files, allowing for comparison of Python numpy.linalg vs. C++, for specific LAPACK subroutines
// *****************************************************************************************************
// reference: https://netlib.org/lapack/explore-html/d8/d9b/group__double_o_t_h_e_rauxiliary_gaa677995264e5de3914a3817a66e753c5.html#gaa677995264e5de3914a3817a66e753c5

#pragma once

#include <vector>
#include <complex>
#include "clapack.hpp"
#include <iostream>

using namespace std;

namespace clap {

    /// @brief Primaty structure used to pass data between Fortran dgeev subroutine that is part of LAPACK
    /// and C++. Stores out values from dgeev_ in oreals and oimags respectively. 
    typedef struct {
        int cols;
        int rows;
        double *datax;
        double *datay;
    } drotparms;

    drotparms newdrotparms(int rows, int cols) {
        drotparms ret;
        ret.rows = rows;
        ret.cols = cols;
        ret.datax = new double[ret.rows*ret.cols];
        ret.datay = new double[ret.rows*ret.cols];
        return ret;
    }

/*
        CALL drot( i2-i, h( i-1, i+1 ), ldh, h( i, i+1 ), ldh,
               cs, sn )
*/
    vector<complex<double>> drot(drotparms v) {
        vector<complex<double>> ret;

        f2c::fortran_int n = v.rows*v.cols;
        f2c::fortran_int sp = 1; // storage spacing for incx & y
        double c=v.rows,s=v.cols;

        //f2c::dgeev_(&NO,&NO,&v.cols,v.matrix_data,&lda,v.oreals,v.oimags,vl,&ldvl,vr,&ldvr,work,&lwork,&info);
        f2c::drot_(&n,v.datax,&sp,v.datay,&sp,&c,&s);
        
        for(int i=0;i<n;i++) {
            std::cout << v.datax[i] << std::endl;
            std::cout << v.datay[i] << std::endl;
        }

        return ret;
    }

}
