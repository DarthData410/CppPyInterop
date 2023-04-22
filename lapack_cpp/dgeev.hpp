#include <iostream>
#include <vector>
#include <complex>

using namespace std;

/*--------  typedefs  ------------*/

typedef int         fortran_int;

typedef struct { float r, i; } f2c_complex;
typedef struct { double r, i; } f2c_doublecomplex;

typedef float             fortran_real;
typedef double            fortran_doublereal;
typedef f2c_complex       fortran_complex;
typedef f2c_doublecomplex fortran_doublecomplex;

/// @brief Primaty structure used to pass data between Fortran GEEV subroutine that is part of LAPACK
/// and C++.
typedef struct {
    fortran_int cols;
    fortran_int rows;
    double *matrix_data;
    double *oreals;
    double *oimags;
} EigValsParms;

extern "C" {
    extern fortran_int dgeev_(char *jobvl, char *jobvr, fortran_int *n,
                double *a, fortran_int *lda, double *wr, double *wi,
                double *vl, fortran_int *ldvl, double *vr, fortran_int *ldvr,
                double *work, fortran_int *lwork,
                fortran_int *info);
    }

namespace cla {

    EigValsParms newEVParms(fortran_int cols, fortran_int rows) {
        EigValsParms ret;
        ret.cols = cols;
        ret.rows = rows;
        ret.matrix_data = new double[ret.cols*ret.rows];
        ret.oreals = new double[ret.cols];
        ret.oimags = new double[ret.rows];
        return ret;
    }

    vector<complex<double>> EigVals(EigValsParms v) {
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
