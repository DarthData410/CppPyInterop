// file: cnumpy.hpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps.
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>

#define PY_SSIZE_T_CLEAN
#include <python3.10/Python.h>

using namespace std;

// defines for py modules/functions/attributes:
#define pyLIST "list"
#define pyLIST_A "append"

// numpy:
#define NP "numpy"
#define NPCOS "cos"
#define NPPi "pi"
#define NPSIN "sin"
#define NPTAN "tan"
#define NPDOT "dot"
#define NPDIAG "diag"

// numpy.random:
#define NPRAND "numpy.random"
#define NPRANDRD "random"

// numpy.linalg:
#define NPLA "numpy.linalg"
#define NPLANORM "norm"
#define NPLAEIGVS "eigvals"

// NP.arrange:
#define NPARNG "arange"

// NP.ARRAY.*
#define NPARRAY "array"
#define NPARRAY_T "T"
#define NPARRAY_SZ "size"
#define NPARRAY_I "item"
// END NP.ARRAY.*

// matplotlib:
#define MAT "matplotlib.pyplot"
#define MPLOT "plot"
#define MSHOW "show"

namespace cpy {
    
    /// @brief Must be the first call by any C++ implementation of this header. Initializes Python, using the value
    /// of the _app param passed in. This value must be retained during execution of Python function calls. Then used
    /// in call for cpy::finalize() so that the wchar_t value can free memory when C++ application scope for Python
    /// is completed.
    /// @param _app const char* value representing C++ application name. Typically argv[0] value from a C++ command line app.
    /// @return wchar_t value created with Py_DecodeLocale, used with Py_SetProgramName for Py_Initialize() call.
    wchar_t *init(const char* _app) {
        wchar_t *program = Py_DecodeLocale(_app, NULL);
        Py_SetProgramName(program);
        Py_Initialize();
        return program;
    }

    double cossintan(double x,int t) {
        // switch on 0 = cos, 1 = sin
        const char* tbc;
        switch(t) {
            case 0: tbc = NPCOS; break;
            case 1: tbc = NPSIN; break;
            case 2: tbc = NPTAN; break;
            default: return 0; break;
        }
        
        double ret;
        PyObject *np = PyImport_ImportModule(NP);
        PyObject *tp = PyTuple_New(1);
        PyObject *pyx = PyFloat_FromDouble(x);
        PyTuple_SetItem(tp,0,pyx);
        PyObject *pNAttr = PyObject_GetAttrString(np,tbc);
        PyObject *pN = PyObject_CallObject(pNAttr,tp);
        
        ret = PyFloat_AsDouble(pN);

        // Py_CLEAR(s):
        Py_CLEAR(pN);
        Py_CLEAR(pNAttr);
        Py_CLEAR(pyx);
        Py_CLEAR(tp);
        Py_CLEAR(np);

        return ret;
    } 

    /// @brief C++ call to Python numpy.cos(x). Building a list python object and passing for operation. Works with 
    /// numpy.ndarray size and item values for converting back to vector of type floats being consine of passed in values.  
    /// Showcasing interop between C++ <-> Python using Python.h.
    ///
    /// Example: 
    ///  vector<double> ncosvret = cnp::pycos(inFVec<double>);
    ///
    /// @param inFVec: Vector of double values, used to convert to numpy.ndarray for operating on.
    /// @return Vector<double> values calculated as cosine values from inFVec values, returned from Python, numpy.cos(x)
    vector<double> cosvec(vector<double> inFVec)
    {
        vector<double> ret;
        
        PyObject *np = PyImport_ImportModule(NP);
        PyObject *plist = PyList_New(0);
        
        for(double d : inFVec) {
            PyObject *pf = PyFloat_FromDouble(d);
            int plac = PyList_Append(plist,pf);
            if(plac!=0) {
                cerr << "PyList_Append: failed to add C double/PyFloat_FromDouble." << endl;
                return ret;
            }
            Py_CLEAR(pf);
        }

        PyObject *pLTup = PyTuple_New(1);
        PyTuple_SetItem(pLTup,0,plist);
        
        // np.array attr / call:
        PyObject *pNDArrayAttr = PyObject_GetAttrString(np,NPARRAY);
        PyObject *pNDArray = PyObject_CallObject(pNDArrayAttr,pLTup);

        // np.cos attr / call:
        PyObject *pNDArryTup = PyTuple_New(1);
        PyTuple_SetItem(pNDArryTup,0,pNDArray);
        PyObject *pNcosAttr = PyObject_GetAttrString(np,NPCOS);
        PyObject *pNcos = PyObject_CallObject(pNcosAttr,pNDArryTup);

        // np.cos returned ndarray, size:
        PyObject *pNcosASize = PyObject_GetAttrString(pNcos,NPARRAY_SZ);
        
        // ref: https://docs.python.org/3/c-api/long.html 
        // note: all python int types are implemented as longs:
        long count_of = PyLong_AsLong(pNcosASize);
        
        PyObject *pItmTup = PyTuple_New(1);
        for(int i=0;i<count_of;i++) {
            PyObject *pidx = PyLong_FromLong(i);
            PyTuple_SetItem(pItmTup,0,pidx);
            PyObject *pcosretAttr = PyObject_GetAttrString(pNcos,NPARRAY_I);
            PyObject *pcosret = PyObject_CallObject(pcosretAttr,pItmTup);
            
            if(!PyFloat_Check(pcosret)) {
                cerr << "Python-numpy-array: return expected long, but PyObject not long." << endl;
                return ret;
            }
            else {
                double cosret = PyFloat_AsDouble(pcosret);
                ret.push_back(cosret);
            }

            Py_CLEAR(pcosret);
            Py_CLEAR(pcosretAttr);
            Py_CLEAR(pidx);
        }

        // Py_CLEAR section:
        Py_CLEAR(pItmTup);
        Py_CLEAR(pNcosASize);
        Py_CLEAR(pNcos);
        Py_CLEAR(pNcosAttr);
        Py_CLEAR(pNDArryTup);
        Py_CLEAR(pNDArray);
        Py_CLEAR(pNDArrayAttr);
        Py_CLEAR(pLTup);
        Py_CLEAR(plist);
        Py_CLEAR(np);

        return ret;
    }

    /// @brief function that returns python, numpy.pi value
    /// @return returns numpy.pi value
    float Pi() {
        
        PyObject *np = PyImport_ImportModule(NP);
        PyObject *pPi = PyObject_GetAttrString(np,NPPi);
        float ret = PyFloat_AsDouble(pPi);
        
        // Py_CLEAR section:
        Py_CLEAR(pPi);
        Py_CLEAR(np);
        return ret;
    }

    /// @brief Using Python modules numpy & matplotlib for plotting visually float values converted to NDaarays.
    /// @param start float value for starting point
    /// @param stop float value for stopping point
    /// @param step float value used for stepping between start & stop float values
    /// @return true is executed without error, false if executed with error. 
    bool Plot(float start, float stop, float step) {
        bool ret = true;
        
        PyObject *np = PyImport_ImportModule(NP);
        PyObject *mat = PyImport_ImportModule(MAT);

        PyObject *npArTup = PyTuple_New(3);
        PyTuple_SetItem(npArTup,0,PyFloat_FromDouble(start));
        PyTuple_SetItem(npArTup,1,PyFloat_FromDouble(stop));
        PyTuple_SetItem(npArTup,2,PyFloat_FromDouble(step));
        PyObject *npArAttr = PyObject_GetAttrString(np,NPARNG);
        PyObject *npAr = PyObject_CallObject(npArAttr,npArTup);

        PyObject *npSinTup = PyTuple_New(1);
        PyTuple_SetItem(npSinTup,0,npAr);
        PyObject *npSinAttr = PyObject_GetAttrString(np,NPSIN);
        PyObject *npSin = PyObject_CallObject(npSinAttr,npSinTup);

        PyObject *mPlotTup = PyTuple_New(2);
        PyTuple_SetItem(mPlotTup,0,npAr);
        PyTuple_SetItem(mPlotTup,1,npSin);
        PyObject *mPlotAttr = PyObject_GetAttrString(mat,MPLOT);
        PyObject *mPlot = PyObject_CallObject(mPlotAttr,mPlotTup);

        PyObject *ET = PyTuple_New(0);
        PyObject *mPlotShowAttr = PyObject_GetAttrString(mat,MSHOW);
        PyObject *mPlotShow = PyObject_CallObject(mPlotShowAttr,ET);

        // Py_CLEAR section:
        Py_CLEAR(mPlotShow);
        Py_CLEAR(mPlotShowAttr);
        Py_CLEAR(ET);
        Py_CLEAR(mPlot);
        Py_CLEAR(mPlotAttr);
        Py_CLEAR(mPlotTup);
        Py_CLEAR(npSin);
        Py_CLEAR(npSinAttr);
        Py_CLEAR(npSinTup);
        Py_CLEAR(npAr);
        Py_CLEAR(npArAttr);
        Py_CLEAR(npArTup);
        Py_CLEAR(mat);
        Py_CLEAR(np);

        return ret;
    }

    /// @brief Loads numpy.random, and calls the random function to generate a PyFloat type.
    /// @return float representing randomly generated value from numpy.random.random()
    float Random() {
        float ret;
        PyObject *pNPRAND = PyImport_ImportModule(NPRAND);
        PyObject *pNPRNAttr = PyObject_GetAttrString(pNPRAND,NPRANDRD);
        PyObject *pNPrnd = PyObject_CallObject(pNPRNAttr,PyTuple_New(0));
        ret = PyFloat_AsDouble(pNPrnd);

        // Py_CLEARs:
        Py_CLEAR(pNPrnd);
        Py_CLEAR(pNPRNAttr);
        Py_CLEAR(pNPRAND);

        return ret;
    }

    /// @brief Final call that needs to be made by C++ application when the scope of embedded Python is completed.
    /// Calls Py_Finalize() and free's up any Python objects from memory, using wchar_t program name value that was
    /// generated as part of cpy::init() - initialize call.
    /// @param program wchar_t * value representing the name of the C++ program, represented in the scope of embeeded Python usage.
    /// @return True on successful Python finalizing, False upon any error.  
    bool finalize(wchar_t *program) {
        bool ret = true;
        Py_Finalize();
        PyMem_RawFree(program);
        return ret;
    }
}

