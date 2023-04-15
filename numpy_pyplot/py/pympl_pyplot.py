# file: pympl_ex1.py
# author: J. Brandon George | darth.data410@gmail.com | @pyfryday
# contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps.
# reference: https://matplotlib.org/3.5.3/api/_as_gen/matplotlib.pyplot.html

import numpy as np
import matplotlib.pyplot as plt

x= np.arange(1,4*np.pi,0.2)
print(x)
y=np.sin(x)
print(y)
plt.plot(x,y)
plt.show() 