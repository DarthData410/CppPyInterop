# file: pynp_cos.py
# author: J. Brandon George | darth.data410@gmail.com | @pyfryday
# contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps. 
# reference: https://numpy.org/doc/stable/reference/generated/numpy.cos.html

import numpy as np

inv = list()
inv.append(1)
inv.append(0.5)
inv.append(1.1)
inv.append(0.7)

cosret = np.cos(np.array(inv))

print("in-values:")
print(inv)
print("cos-values:")
print(cosret)
 