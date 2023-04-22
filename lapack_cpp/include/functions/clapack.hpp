// file: f2clapack.hpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Fortran-LAPACK interop. LAPACK is used by Python numpy 
// module. This C++ app showcases the ability to retrieve the same functionality as numpy.linalg by
// exposing the same Fortran LAPACK subroutines, as extern "C" calls. Creating a comparable C++ set of
// header files, allowing for comparison of Python numpy.linalg vs. C++, for specific LAPACK subroutines
// *****************************************************************************************************
// This file contains a single point for all extern "C" LAPACK Fortran subroutine reference points. 
// clap::f2c::* functions are meant to be surfaced and called from corresponding C++ header file representations
// of working with these lower level Fortran calls.

#pragma once

namespace clap::f2c {

    typedef int         fortran_int;

    typedef struct { float r, i; } f2c_complex;
    typedef struct { double r, i; } f2c_doublecomplex;

    typedef float             fortran_real;
    typedef double            fortran_doublereal;
    typedef f2c_complex       fortran_complex;
    typedef f2c_doublecomplex fortran_doublecomplex;


    extern "C" {
        
        extern fortran_int dgeev_(char *jobvl, char *jobvr, fortran_int *n,
                    double *a, fortran_int *lda, double *wr, double *wi,
                    double *vl, fortran_int *ldvl, double *vr, fortran_int *ldvr,
                    double *work, fortran_int *lwork,
                    fortran_int *info);
        
        extern fortran_int sgeev_(char *jobvl, char *jobvr, fortran_int *n,
                    fortran_real *a, fortran_int *lda, fortran_real *wr, fortran_real *wi,
                    fortran_real *vl, fortran_int *ldvl, fortran_real *vr, fortran_int *ldvr,
                    fortran_real *work, fortran_int *lwork,
                    fortran_int *info); 
        }
}