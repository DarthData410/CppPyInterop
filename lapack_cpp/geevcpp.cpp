#include <iostream>
#include <cmath>
#include <complex>
#include <vector>
#include <lapack.h>

using namespace std;
using namespace complex_literals;

extern "C" {
     void dgeev_(int *n, int *nrhs,  double *a,  int  *lda,  
           int *ipivot, double *b, int *ldb, int *info) ;
}

int main() {
    

    return 0;
}