import numpy as np
import numpy.linalg as LA
import numpy.linalg._umath_linalg as ul

"""
    a[0][0] = 0.1244;
    a[0][1] = 0.4412;
    a[1][0] = 0.8344;
    a[1][1] = 0.4438;

    work output:
    work[0]:        68
    work[1]:        6.95335e-310
"""

na = np.array([
    [0.1244,0.4412],
    [0.8344,0.4438]
])

print(na)

naev = LA.eigvals(na)
print(naev)

