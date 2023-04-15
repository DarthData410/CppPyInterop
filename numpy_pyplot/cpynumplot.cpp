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
        ret = ret * cnp::pyPi();
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
        
        string strstart = argv[2];
        float start = pyplot_float(strstart);
        string strstop = argv[3];
        float stop = pyplot_float(strstop);
        string strstep = argv[4];
        float step = pyplot_float(strstep);
        
        if(cnp::pyPlot(start,stop,step)) {
            cout << " function: cnp::pyPlot() [mixed numpy-matplotlib.pyplot]" << endl;
            cout << " -------------------------------" << endl;
            cout << " values used for rendering plot:" << endl;
            cout << " ---------" << endl;
            cout << " start: " << strstart << ", real: " << start << endl;
            cout << " stop: " << strstop << ", real: " << stop << endl;
            cout << " step: " << strstep << ", real: " << step << endl;
            cout << " rendered successful" << endl;
        }
    }
    
    cout << endl;

    Py_Finalize();
    PyMem_RawFree(program);
       
    return 0;
}