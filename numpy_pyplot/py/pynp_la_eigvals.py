# file: pynp_la_eigvals.py
# author: J. Brandon George | darth.data410@gmail.com | @pyfryday
# contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps. 
# reference: https://numpy.org/doc/stable/reference/generated/numpy.linalg.eigvals.html

import numpy as np
from numpy import linalg as LA

# part 1:
x = np.random.random()
Q = np.array([[np.cos(x), -np.sin(x)], [np.sin(x), np.cos(x)]])

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

