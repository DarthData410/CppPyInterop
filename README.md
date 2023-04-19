# CppPyInterop
By: J. Brandon George | darth.data410@gmail.com | twitter: @PyFryDay | medium.com: https://darth-data410.medium.com/ | youtube: https://www.youtube.com/@pyfryday

# Details:
Interop Examples of Embedding Python within C++, using Python.h only. This includes using vscode as IDE choice on ubunutu (linux). The contents of this project represent work used in medium.com articles for creating how-tos and training materials. Medium.com articles for each section will be linked as pubslihed.

# cpy-numpy_pyplot
Within this section of the repo is a vscode IDE targeted project, focused on Pythons numpy and matplotlib modules. The main command line tool to showcase Python being embedded within C++ is called 'cpy'. You may need to adjust the .vscode folder, tasks.json file for YOUR Python implementation. 

As-is this project debugs/builds using Visual Studio Code v1.77.3 IDE, running on Ubuntu 22.04.2 LTS, ../include/python3.10 and flag -std=c++17. 

The following section list usage information for ./cpy:
 1. -matrix-diag-test:
   Uses 2 parameters passed in, the rows and columns of a test NDMatrix. Then NDMatrices of that size are used throughout the test execution. This includes showcase of generating random numbers from numpy.random.random via Python.h within C++. Further the finding of diagonal values of an numpy.NDArray built from the custom cpy::NDMatrix found in this project. 

   Example: ./cpy -matrix-diag-test 4 4
   Output example shown in the following screen shot:
   <img src="https://github.com/DarthData410/CppPyInterop/blob/main/numpy_pyplot/imgs/cpy_matrix_diag_test_4x4.png?raw=true">

 2. -pyplot:
   Uses three parameters with call: start, stop & step values. Expects these to be floats. Can use *pi in
   order to adjust one-or-all three values by. Uses the values to load Python modules numpy and 
   matplotlib.pyplot. Generates NDarray values using start, stop and step. Then retrieves the sin(x) values
   stored in a different NDarray. Calls pyplot to plot(x,y) and show(). 
    
   Examples:
   (Example_1): ./cpy -pyplot 1 11 0.2 - Plot saved: <a href="https://github.com/DarthData410/CppPyInterop/blob/main/numpy_pyplot/saved_plots/Example_1.png?raw=true">../numpy_pyplot/saved_plots/Example_1.png</a>

   <img src="https://github.com/DarthData410/CppPyInterop/blob/main/numpy_pyplot/saved_plots/Example_1.png?raw=true">
    
   (Example_2): ./cpy -pyplot 1 3*pi 0.48 - Plot saved: <a href="https://github.com/DarthData410/CppPyInterop/blob/main/numpy_pyplot/saved_plots/Example_2.png?raw=true">../numpy_pyplot/saved_plots/Example_2.png</a>
    
   <img src="https://github.com/DarthData410/CppPyInterop/blob/main/numpy_pyplot/saved_plots/Example_2.png?raw=true">
