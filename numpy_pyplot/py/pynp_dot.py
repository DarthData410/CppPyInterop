# file: pynp_dot.py
# author: J. Brandon George | darth.data410@gmail.com | @pyfryday
# contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps. 
# reference: https://numpy.org/doc/stable/reference/generated/numpy.dot.html

import numpy as np

np.dot(3, 4)
# expected: 12

np.dot([2j, 3j], [2j, 3j])
# expected: (-13+0j)

a = [[1, 0], [0, 1]]
b = [[4, 1], [2, 2]]
np.dot(a, b)
# expected:
# array([[4, 1],
#       [2, 2]])

a = np.arange(3*4*5*6).reshape((3,4,5,6))
b = np.arange(3*4*5*6)[::-1].reshape((5,4,6,3))
np.dot(a, b)[2,3,2,1,2,2]
# expected: 499128

sum(a[2,3,2,:] * b[1,2,:,2])
# expcted: 499128