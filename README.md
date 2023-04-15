# CppPyInterop
By: J. Brandon George | darth.data410@gmail.com | twitter: @PyFryDay | medium.com: https://darth-data410.medium.com/ | youtube: https://www.youtube.com/@pyfryday

# Details:
Interop Examples of Embedding Python within C++, using Python.h only. This includes using vscode as IDE choice on ubunutu (linux). The contents of this project represent work used in medium.com articles for creating how-tos and training materials.

# numpy_pyplot - cpynumplot
Within this section of the repo is a vscode IDE targeted project, focused on Pythons numpy and matplotlib modules. The main command line tool to showcase Python being embedded within C++ is called 'cpynumplot'. You may need to adjust the .vscode folder, tasks.json file for YOUR Python implementation. 

As-is this project debugs/builds using Visual Studio Code v1.77.3 IDE, running on Ubuntu 22.04.2 LTS, ../include/python3.10 and flag -std=c++17. 

The following section list usage information for ./cpynumplot:
 1. --cos:
    Uses passed in string of doubles, calling Python module numpy, and creates an NDarray from
    doubles. Then calls numpy.cos(x) to deliver the cosine values for passed in doubles.
    This NDarray of cosine doubles is then converted to vector(double) in C++, and then iterated through
    to display in the command output in-values and cos-values back. 
    
    Example:
    ./cpynumplot --cos 1.1,0.97,1.3,2.4879

    Output:
    in-values: 1.1,0.97,1.3,2.4879
    cos-values: (0.453596,0.565300,0.267499,-0.793844) 

 2. --pi:
    Simple call, that will not operate upon any parameters. Simply loads Pythons numpy, and calls numpy.pi. 
    Converts from a PyObject value to a float. Using PyFloat_AsDouble(x) call.

    Example:
    ./cpynumplot --pi

    Output:
    return: 3.141593 

 3. --pyplot:
    Uses three parameters with call: start, stop & step values. Expects these to be floats. Can use *pi in
    order to adjust one-or-all three values by. Uses the values to load Python modules numpy and 
    matplotlib.pyplot. Generates NDarray values using start, stop and step. Then retrieves the sin(x) values
    stored in a different NDarray. Calls pyplot to plot(x,y) and show(). 
    
    Examples:
    (Example_1): ./cpynumplot --pyplot 1 11 0.2 - Plot saved: <a href="https://github.com/DarthData410/CppPyInterop/blob/main/numpy_pyplot/saved_plots/Example_1.png?raw=true">../numpy_pyplot/saved_plots/Example_1.png</a>

    <img src="https://github.com/DarthData410/CppPyInterop/blob/main/numpy_pyplot/saved_plots/Example_1.png?raw=true">
    
    (Example_2): ./cpynumplot --pyplot 1 3*pi 0.48 - Plot saved: <a href="https://github.com/DarthData410/CppPyInterop/blob/main/numpy_pyplot/saved_plots/Example_2.png?raw=true">../numpy_pyplot/saved_plots/Example_2.png</a>
    
    <img src="https://github.com/DarthData410/CppPyInterop/blob/main/numpy_pyplot/saved_plots/Example_2.png?raw=true">
