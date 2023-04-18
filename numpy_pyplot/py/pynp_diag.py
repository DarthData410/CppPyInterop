# file: pynp_diag.py
# author: J. Brandon George | darth.data410@gmail.com | @pyfryday
# contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps. 
# reference: https://numpy.org/doc/stable/reference/generated/numpy.diag.html

import numpy as np

# array example:
a = [[0.1,0.2,0.3],
     [1.1,2.1,3.1],
     [1,2,3]]

print(a)
ad = np.diag(a)
print(ad)

# list example:
la = list()
la.append(0.11)
la.append(1.11)
la.append(2.11)

la2 = list()
la2.append(1.11)
la2.append(3.11)
la2.append(5.11)

la3 = list()
la3.append(7.11)
la3.append(9.11)
la3.append(12.11)

ladin = list()
ladin.append(la)
ladin.append(la2)
ladin.append(la3)

lad = np.diag(ladin)

print(lad)