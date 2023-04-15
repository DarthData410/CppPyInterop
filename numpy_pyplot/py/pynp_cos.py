# file: pynp_cos.py
# author: J. Brandon George | darth.data410@gmail.com | @pyfryday
# contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps. 
# reference: https://numpy.org/doc/stable/reference/generated/numpy.cos.html

import numpy as np

inv = list()
inv.append(1.25)
inv.append(7.845)
inv.append(1.1)
inv.append(0.54)
inv.append(1.13)

cosret = np.cos(np.array(inv))

print("in-values:")
print(inv)
print("cos-values:")
print(cosret)

"""
Expected output from above:

in-values:
[1.25, 7.845, 1.1, 0.54, 1.13]
cos-values:
[0.31532236 0.00898151 0.45359612 0.85770868 0.42665981]
"""