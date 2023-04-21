import numpy as np
import numpy.linalg as LA
import numpy.linalg._umath_linalg as ul

"""
    cdgeev.cpp LAPACK Fortran dgeev subrountine called using C headers
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

