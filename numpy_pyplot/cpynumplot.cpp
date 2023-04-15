// file: cnumpy.cpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps.

#include "cpynumplot.hpp"

/// @brief Converts a string of doubles, checking for separator, and converts to a vector of doubles
/// @param _s string value of doubles with separator
/// @return vector of doubles used with python numpy function calls
static vector<double> getDvectorFrStr(string _s)
{
    // separator replacements:
    replace(_s.begin(),_s.end(),',',' ');
    replace(_s.begin(),_s.end(),'|',' ');
    
    // beging reading stream and iterating through for double values:
    istringstream is(_s);
    return vector<double>{ 
        istream_iterator<double>(is),
        istream_iterator<double>()
    };
}

/// @brief part of static C++ ex* function calls that map to numpy python interop functions. retrieves cosine value of passed in vector of doubles
/// @param _ivals string representing double(s) as 1.0,1.1 or 1.0|1.1, seeking consine values.
/// @return string [d.d,*] of double values, representing cosine of _ivals.
static string excos(string _ivals) {
    string ret = "[";
    vector<double> invals = getDvectorFrStr(_ivals);
    vector<double> cosvals = cnp::cos(invals);
    for(double d : cosvals) {
        ret += to_string(d);
        ret += ",";
    }
    ret = ret.substr(0,ret.size()-1);
    ret += "]";
    return ret;
}

int main(int argc, char *argv[])
{
    if(argc<2) {

        cerr << "expected usage: ./cnumpy --<function> <vals> ..." << endl;
        return 1;
    }

    wchar_t *program = Py_DecodeLocale(APP, NULL);
    
    Py_SetProgramName(program);
    Py_Initialize();

    string func = argv[1];
    cout << "\n ----- cnumpy -----" << endl;
    
    if(func=="--cos") {
        string vals = argv[2];
        string cosvals = excos(vals);
        cout << " function: numpy.cos(x)" << endl;
        cout << " ----------------------" << endl;
        cout << " in-values: " << vals << endl;
        cout << " cos-values: " << cosvals << endl;
    }
    else if(func=="--pi") {
        string pret = to_string(cnp::pyPi());
        cout << " function: numpy.pi" << endl;
        cout << " ------------------" << endl;
        cout << " return: " << pret << endl;
    }
    else if(func=="--pyplot") {
        float start = stof(argv[2]);
        float stop = stof(argv[3]);
        float step = stof(argv[4]);
        if(cnp::pyPlot(start,stop,step)) {
            cout << " function: cnumpy::cnp::pyPlot()" << endl;
            cout << " -------------------------------" << endl;
            cout << " values used for rendering plot:" << endl;
            cout << " ---------" << endl;
            cout << " start: " << start << endl;
            cout << " stop: " << stop << endl;
            cout << " step: " << step << endl;
            cout << " rendered successful" << endl;
        }
    }
    
    cout << endl;

    Py_Finalize();
    PyMem_RawFree(program);
       
    return 0;
}