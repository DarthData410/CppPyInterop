// file: cnumpy.cpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps.
// ------------------------------------------
// Command line execution app used for interop between C++ -> Python. Enabled by Python.h header file
// This command line app has the following commands, and usage:
//
// 1. -cos:
//    Uses passed in string of doubles, calling Python module numpy, and creates an NDarray from
//    doubles. Then calls numpy.cos(x) to deliver the cosine values for passed in doubles.
//    This NDarray of cosine doubles is then converted to vector<double> in C++, and then iterated through
//    to display in the command output in-values and cos-values back. 
//    
//    Example:
//    ./cpynumplot -cos 1.1,0.97,1.3,2.4879
//
//    Output:
//    ----- cpynumplot -----
//    function: numpy.cos(x)
//    ----------------------
//    in-values: 1.1,0.97,1.3,2.4879
//    cos-values: [0.453596,0.565300,0.267499,-0.793844]
//
// 2. -pi:
//    Simple call, that will not operate upon any parameters. Simply loads Pythons numpy, and calls numpy.pi. 
//    Converts from a PyObject value to a float. Using PyFloat_AsDouble(x) call.
//
//    Example:
//    ./cpynumplot -pi
//
//    Output:
//    ----- cpynumplot -----
//    function: numpy.pi
//    ------------------
//    return: 3.141593 
//
// 3. -pyplot:
//    Uses three parameters with call: start, stop & step values. Expects these to be floats. Can use *pi in
//    order to adjust one-or-all three values by. Uses the values to load Python modules numpy and 
//    matplotlib.pyplot. Generates NDarray values using start, stop and step. Then retrieves the sin(x) values
//    stored in a different NDarray. Calls pyplot to plot(x,y) and show(). 
//    
//    Examples:
//    (Example_1): ./cpynumplot -pyplot 1 11 0.2 - Plot saved: ../numpy_pyplot/saved_plots/Example_1.png
//    (Example_2): ./cpynumplot -pyplot 1 3*pi 0.48 - Plot saved: ../numpy_pyplot/saved_plots/Example_2.png
//
// 4. -rand:
//    No parameters are used when function -rand is called. Function calls numpy.random.random() and returns
//    the randomly generated float value.
//
//    Example:
//    ./cpynumplot -rand
//
//    Output:
//    ----- cpynumplot -----
//    function: numpy.random.random
//    -----------------------------
//    return: 0.58332
// 
// *****************************************************************************************************

#include "cpynumplot.hpp"

/// @brief Converts a string of doubles, checking for separator, and converts to a vector of doubles
/// @param _s string value of doubles with separator
/// @return vector of doubles used with python numpy function calls
static vector<double> getDvectorFrStr(string _s)
{
    // separator replacements:
    replace(_s.begin(),_s.end(),',',' ');
    
    // beging reading stream and iterating through for double values:
    istringstream is(_s);
    return vector<double>{ 
        istream_iterator<double>(is),
        istream_iterator<double>()
    };
}

/// @brief part of static C++ ex* function calls that map to numpy python interop functions. retrieves cosine value of passed in vector of doubles
/// @param _ivals string representing double(s) as 1.0,1.1 seeking consine values.
/// @return string [d.d,*] of double values, representing cosine of _ivals.
static string excos(string _ivals) {
    string ret = "[";
    vector<double> invals = getDvectorFrStr(_ivals);
    vector<double> cosvals = cpy::cos(invals);
    for(double d : cosvals) {
        ret += to_string(d);
        ret += ",";
    }
    ret = ret.substr(0,ret.size()-1);
    ret += "]";
    return ret;
}

/// @brief part of static C++ calls, used to parse passed in string for float value. if detects *pi will apply to value and return.
/// @param _sr string representation of float. can include '*pi' and will apply cnp::pyPi() value. 
/// @return returns converted string representation of float as float, with *pi applied if specified when passed in.
static float pyplot_float(string _sr) {
    bool upi = false;
    string _spi = "*pi";
    
    size_t found=_sr.find(_spi);
    if( found != std::string::npos) {
        upi = true;
        replace(_sr.begin(),_sr.end(),'*',' ');
        replace(_sr.begin(),_sr.end(),'p',' ');
        replace(_sr.begin(),_sr.end(),'i',' ');
    }

    float ret = stof(_sr);
    if(upi) {
        ret = ret * cpy::Pi();
    }
    return ret;
}

int main(int argc, char *argv[])
{
    if(argc<2) {

        cerr << "expected usage: ./cpynumplot --<function> <vals> ..." << endl;
        return 1;
    }

    wchar_t *program = Py_DecodeLocale(APP, NULL);
    
    Py_SetProgramName(program);
    Py_Initialize();

    string func = argv[1];
    cout << "\n ----- cpynumplot -----" << endl;
    
    if(func=="-cos") {
        string vals = argv[2];
        string cosvals = excos(vals);
        cout << " function: numpy.cos(x)" << endl;
        cout << " ----------------------" << endl;
        cout << " in-values: " << vals << endl;
        cout << " cos-values: " << cosvals << endl;
    }
    else if(func=="-pi") {
        string pret = to_string(cpy::Pi());
        cout << " function: numpy.pi" << endl;
        cout << " ------------------" << endl;
        cout << " return: " << pret << endl;
    }
    else if(func=="-pyplot") {
        
        string strstart = argv[2];
        float start = pyplot_float(strstart);
        string strstop = argv[3];
        float stop = pyplot_float(strstop);
        string strstep = argv[4];
        float step = pyplot_float(strstep);
        
        if(cpy::Plot(start,stop,step)) {
            cout << " function: cpy::Plot() [mixed numpy-matplotlib.pyplot]" << endl;
            cout << " -------------------------------" << endl;
            cout << " values used for rendering plot:" << endl;
            cout << " ---------" << endl;
            cout << " start: " << strstart << ", real: " << start << endl;
            cout << " stop: " << strstop << ", real: " << stop << endl;
            cout << " step: " << strstep << ", real: " << step << endl;
            cout << " rendered successful" << endl;
        }
    }
    else if(func=="-rand") {
        cout << " function: numpy.random.random" << endl;
        cout << " -----------------------------" << endl;
        cout << " return: " << cpy::Random() << endl;
    }
    
    cout << endl;

    Py_Finalize();
    PyMem_RawFree(program);
       
    return 0;
}