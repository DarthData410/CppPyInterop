# file: pympl_ex1.py
# author: J. Brandon George | darth.data410@gmail.com | @pyfryday
# contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps.
# example for generating a ndarray of numbers, then generating sin valuess of each, and plotting the two arrays

import numpy as np
import matplotlib.pyplot as plt

x= np.arange(1,4*np.pi,0.2)
print(x)
y=np.sin(x)
print(y)
plt.plot(x,y)
plt.show() 