# file: npeigvas.py
# author: J. Brandon George | darth.data410@gmail.com | @pyfryday
# contents used for testing against C++ <-> Fortran-LAPACK interop. LAPACK is used by Python numpy 
# module. This C++ app showcases the ability to retrieve the same functionality as numpy.linalg by
# exposing the same Fortran LAPACK subroutines, as extern "C" calls. Creating a comparable C++ set of
# header files, allowing for comparison of Python numpy.linalg vs. C++, for specific LAPACK subroutines
# *****************************************************************************************************
# Python file for testing output from numpy.linalg.eigvals(x) vs. LAPACK dgeev & sgeev subroutines
# exposed via C/C++.

import numpy as np
import numpy.linalg as LA
import numpy.linalg._umath_linalg as ul

"""
    (cdgeev.cpp) 
    LAPACK Fortran dgeev subrountine called using C headers
    within C++:
    -----------
    // 2x2 matrix, linearized:
    // row[0]:
    a[0] = 0.1244;
    a[2] = 0.4412;
    // row[1]:
    a[1] = 0.8344;
    a[3] = 0.4438;

    C++ output:
    [r:i](-0.343308:0)
    [r:i](0.911508:0)
"""

na = np.array([
    [0.1244,0.4412],
    [0.8344,0.4438]
])

print(na)

naev = LA.eigvals(na)
print(naev)

"""
    Python numpy.linalg.eigvals(x) output:
    [-0.34330846  0.91150846]
"""

"""
    Test two:
    ./cla - cla.cpp - dgeev.hpp
    vp.matrix_data[0] = 1.23;
    vp.matrix_data[2] = 0.081;
    vp.matrix_data[1] = 0.44;
    vp.matrix_data[3] = 0.13287;

    ./cla output:
    --- EIGVALS ---
    [1.261576:0.000000] 
    [0.101294:0.000000] 
"""

na = np.array([
    [1.23,0.081],
    [0.44,0.13287]
])

print(na)

naev = LA.eigvals(na)
print(naev)

"""
    Python numpy.linalg.eigvals(x) output:
    [1.26157598 0.10129402]
"""