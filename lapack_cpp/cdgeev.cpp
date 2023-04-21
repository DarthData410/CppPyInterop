// ref(a): https://netlib.org/lapack/explore-html/d9/d8e/group__double_g_eeigen_ga66e19253344358f5dee1e60502b9e96f.html


#include <iostream>
#include <cmath>
#include <complex>
#include <vector>

using namespace std;
using namespace complex_literals;


/*
    LAPACK->Fortran subroutine:
    subroutine dgeev 	( 	
        character  	JOBVL,
		character  	JOBVR,
		integer  	N,
		double precision, dimension( lda, * )  	A,
		integer  	LDA,
		double precision, dimension( * )  	WR,
		double precision, dimension( * )  	WI,
		double precision, dimension( ldvl, * )  	VL,
		integer  	LDVL,
		double precision, dimension( ldvr, * )  	VR,
		integer  	LDVR,
		double precision, dimension( * )  	WORK,
		integer  	LWORK,
		integer  	INFO 
	) 	
*/

typedef int         fortran_int;

typedef struct { float r, i; } f2c_complex;
typedef struct { double r, i; } f2c_doublecomplex;
/* typedef long int (*L_fp)(); */

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
}


int main() {
    int cols=2,rows=2;
    char jobvl = 'N'; // 'N': left eigenvectors of A are not computed OR 'V': left eigenvectors of A are computed.    
    char jobvr = 'N'; // ^^^ "" -> same except for right eigenvectors
    fortran_int n = cols; // The order of the matrix A. N >= 0.
    /* 
        A is DOUBLE PRECISION array, dimension (LDA,N)
        On entry, the N-by-N matrix A.
        On exit, A has been overwritten.
    */
    double *a = new double[cols*rows];
    fortran_int lda=n; // The leading dimension of the array A.  LDA >= max(1,N).
    /*
        WR and WI contain the real and imaginary parts,
        respectively, of the computed eigenvalues.  Complex
        conjugate pairs of eigenvalues appear consecutively
        with the eigenvalue having the positive imaginary part
        first.
    */
    double *wr = new double[n]; // WR is DOUBLE PRECISION array, dimension (N)
    double *wi = new double[n]; //WI is DOUBLE PRECISION array, dimension (N)
    /*
        VL is DOUBLE PRECISION array, dimension (LDVL,N)
        If JOBVL = 'V', the left eigenvectors u(j) are stored one
        after another in the columns of VL, in the same order
        as their eigenvalues.
        If JOBVL = 'N', VL is not referenced.
        If the j-th eigenvalue is real, then u(j) = VL(:,j),
        the j-th column of VL.
        If the j-th and (j+1)-st eigenvalues form a complex
        conjugate pair, then u(j) = VL(:,j) + i*VL(:,j+1) and
        u(j+1) = VL(:,j) - i*VL(:,j+1).
    */
    double *vl;
    /*
        The leading dimension of the array VL.  LDVL >= 1; if
        JOBVL = 'V', LDVL >= N.
    */
    fortran_int ldvl=1;
    // ^^^ "" -> same as vl & ldvl, except right vectors
    double *vr;
    fortran_int ldvr=1;
    /*
        LWORK is INTEGER
        The dimension of the array WORK.  LWORK >= max(1,3*N), and
        if JOBVL = 'V' or JOBVR = 'V', LWORK >= 4*N.  For good
        performance, LWORK must generally be larger.

        If LWORK = -1, then a workspace query is assumed; the routine
        only calculates the optimal size of the WORK array, returns
        this value as the first entry of the WORK array, and no error
        message related to LWORK is issued by XERBLA.
    */
    fortran_int lwork = 6*n;
    /*
        WORK is DOUBLE PRECISION array, dimension (MAX(1,LWORK))
        On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
    */
    double *work = new double[lwork];
    /*
        = 0:  successful exit
        < 0:  if INFO = -i, the i-th argument had an illegal value.
        > 0:  if INFO = i, the QR algorithm failed to compute all the
            eigenvalues, and no eigenvectors have been computed;
            elements i+1:N of WR and WI contain eigenvalues which
            have converged.
    */
    fortran_int info;
    
    // 2x2 matrix, linerized:
    // row[0]:
    a[0] = 0.1244;
    a[2] = 0.4412;
    // row[1]:
    a[1] = 0.8344;
    a[3] = 0.4438;

    dgeev_(&jobvl,&jobvr,&n,a,&lda,wr,wi,vl,&ldvl,vr,&ldvr,work,&lwork,&info);

    if(info<0) {
        cout << "argument has illegal value." << endl;
    }
    else {
        for(int i=0;i<n;i++) {
            cout << "[r:i](" << wr[i] << ":" << wi[i] << ")" << "\n";
        }
    }

    return 0;
}