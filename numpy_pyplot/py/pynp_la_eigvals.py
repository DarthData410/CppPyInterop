# file: pynp_la_eigvals.py
# author: J. Brandon George | darth.data410@gmail.com | @pyfryday
# contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps. 
# reference: https://numpy.org/doc/stable/reference/generated/numpy.linalg.eigvals.html

import numpy as np
from numpy import linalg as LA

# part 1:
x = np.random.random()
Q = np.array([[np.cos(x), -np.sin(x)], [np.sin(x), np.cos(x)]])
print(Q)

ret1 = LA.norm(Q[0, :]), LA.norm(Q[1, :]), np.dot(Q[0, :],Q[1, :])
print(ret1)
#expected: (1.0, 1.0, 0.0)

# part 2:
D = np.diag((-1,1))
ret2 = LA.eigvals(D)
print(ret2)
#expected: array([-1.,  1.])

A = np.dot(Q, D)
A = np.dot(A, Q.T) # T = transpose of Q.
ret3 = LA.eigvals(A)
print(ret3)
#expected: array([ 1., -1.]) or array([-1., 1.]) # random

# ---------------
# The following are test cases comparing cpy::EigVals(NDMatrix m) calls vs. pure Python
# ---------------

"""
NDMatrix[3x3]
   0:[0.617905|0.302729|0.850671]
   1:[0.673986|0.126655|0.002773]
   2:[0.455710|0.338128|0.452462]
 
 eigvals: [1.372819  |-0.087899 0.236911|-0.087899 -0.236911]
"""

z = np.array([
    [0.617905,0.302729,0.850671],
   [0.673986,0.126655,0.002773],
   [0.455710,0.338128,0.452462],
])
print(z)
ze = LA.eigvals(z)
print(ze)

"""
NDMatrix[2x2]
   0:[0.871643|0.296733]
   1:[0.849394|0.528729]

 eigvals: [1.230696  |0.169676  ]
"""
z = np.array([
[0.871643,0.296733],
[0.849394,0.528729]
])
print(z)
ze = LA.eigvals(z)
print(ze)