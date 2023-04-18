// file: cnumpy.hpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps.
#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <iterator>
#include <sstream>

#define PY_SSIZE_T_CLEAN
#include <python3.10/Python.h>

using namespace std;
using namespace complex_literals;

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
    /// @param app const char* value representing C++ application name. Typically argv[0] value from a C++ command line app.
    /// @return wchar_t value created with Py_DecodeLocale, used with Py_SetProgramName for Py_Initialize() call.
    wchar_t *init(const char* app) {
        try {
            wchar_t *program = Py_DecodeLocale(app, NULL);
            Py_SetProgramName(program);
            Py_Initialize();
            return program;
        }
        catch(...) {
            throw runtime_error(("%s fata error trying to call Py_Initialize. check Python install."));
        }
    }

    /// @brief Base Python.h embedded C++ function for retriving an object attribute string, and then calling that object
    /// with passed in ptup as a PyTuple_Type, filled with any arguments that are needed. 
    /// @param py the module/object that is the target of the PyObject_GetAttrString
    /// @param tocall the function of the module/object that is the target of the PyObject_GetAttrString
    /// @param ptup the PyTuple_Type, PyObject* target of hte PyObject_CallObject, containing needed arguments for successful Python call.
    /// @return returns value from targeted Python function call. *** Calling logic should know Python Type being returned. ***
    PyObject *basepy(PyObject *py,const char* tocall,PyObject *ptup) {
        PyObject *pAttr = PyObject_GetAttrString(py,tocall);
        if(pAttr!=NULL) {
            PyObject *pRet = PyObject_CallObject(pAttr,ptup);
            Py_CLEAR(pAttr);
            return pRet;
        }
        else {
            PyErr_Print();
            string rte = "PyObject_GetAttrString failed for:";
            rte += tocall;
            throw runtime_error(rte);
        }
    }

    /// @brief base level function that safely loads a Python module. Checks for loading, and if NULL throws runtime_error() for attempted module
    /// @param m Python module to be imported using PyImport_ImportModule(m).
    /// @return returns PyObject represented successfully imported Python module.
    PyObject *modimp(const char* m) {
        PyObject *ret = PyImport_ImportModule(m);
        if(ret==NULL) {
            PyErr_Print();
            throw runtime_error("cpy::modimp - Python Module Load Error");
        }
        return ret;
    }

    /// @brief Common function, based on t value to return cos,sin or tan value of x param. Call numpy.cos,sin or tan.
    /// @param x value to perform numpy.cos,sin or tan operation upon
    /// @param t type of operation to perform. 0=cos,1=sin,2=tan
    /// @return double value based on operation. Either cos, sin or tan of param x
    double CST(double x,int t) {
        const char* tbc;
        switch(t) {
            case 0: tbc = NPCOS; break;
            case 1: tbc = NPSIN; break;
            case 2: tbc = NPTAN; break;
            default: return 0; break;
        }
        
        double ret;
        PyObject *np = modimp(NP);
        PyObject *tp = PyTuple_New(1);
        PyObject *pyx = PyFloat_FromDouble(x);
        PyTuple_SetItem(tp,0,pyx);
        PyObject *pN = basepy(np,tbc,tp);

        if(!PyFloat_Check(pN)) {
            string rte = "fatal error with cpy::CST. PyFloat_Check failed for type: ";
            rte += tbc;
            throw runtime_error(rte);
        }
        ret = PyFloat_AsDouble(pN);

        // Py_CLEAR(s):
        Py_CLEAR(pN);
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
    ///  vector<double> ncosvret = cpy::cosvec(indvec<double>);
    ///
    /// @param indvec: Vector of double values, used to convert to numpy.ndarray for operating on.
    /// @return Vector<double> values calculated as cosine values from inFVec values, returned from Python, numpy.cos(x)
    vector<double> CosVec(vector<double> indvec)
    {
        vector<double> ret;
        PyObject *np = modimp(NP);
        PyObject *plist = PyList_New(0);
        
        for(double d : indvec) {
            PyObject *pf = PyFloat_FromDouble(d);
            int plac = PyList_Append(plist,pf);
            if(plac!=0) {
                throw runtime_error("PyList_Append: failed to add C double/PyFloat_FromDouble.");
            }
            Py_CLEAR(pf);
        }

        PyObject *pLTup = PyTuple_New(1);
        PyTuple_SetItem(pLTup,0,plist);
        
        // np.array attr / call:
        PyObject *pNDArray = basepy(np,NPARRAY,pLTup);

        // np.cos attr / call:
        PyObject *pNDArryTup = PyTuple_New(1);
        PyTuple_SetItem(pNDArryTup,0,pNDArray);
        PyObject *pNcos = basepy(np,NPCOS,pNDArryTup);

        // np.cos returned ndarray, size:
        PyObject *pNcosASize = PyObject_GetAttrString(pNcos,NPARRAY_SZ);
        
        // ref: https://docs.python.org/3/c-api/long.html 
        // note: all python int types are implemented as longs:
        long count_of = PyLong_AsLong(pNcosASize);
        
        PyObject *pItmTup = PyTuple_New(1);
        for(int i=0;i<count_of;i++) {
            PyObject *pidx = PyLong_FromLong(i);
            PyTuple_SetItem(pItmTup,0,pidx);
            PyObject *pcosret = basepy(pNcos,NPARRAY_I,pItmTup);
            
            if(!PyFloat_Check(pcosret)) {
                throw runtime_error("Python-numpy-array: return expected long, but PyObject not long.");
            }
            else {
                double cosret = PyFloat_AsDouble(pcosret);
                ret.push_back(cosret);
            }

            Py_CLEAR(pcosret);
            Py_CLEAR(pidx);
        }

        // Py_CLEAR section:
        Py_CLEAR(pItmTup);
        Py_CLEAR(pNcosASize);
        Py_CLEAR(pNcos);
        Py_CLEAR(pNDArryTup);
        Py_CLEAR(pNDArray);
        Py_CLEAR(pLTup);
        Py_CLEAR(plist);
        Py_CLEAR(np);
        return ret;
    }

    vector<double> diagvec(array<vector<double>,3> nda) {
        cout << nda[0][0] << endl;
        cout << "test daivec over" << endl;
        vector<double> ret;
        ret.push_back(0.1);
        return ret;
    }

    /// @brief function that returns python, numpy.pi value
    /// @return returns numpy.pi value
    float Pi() {
        
        PyObject *np = modimp(NP);
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
        
        PyObject *np = modimp(NP);
        PyObject *mat = modimp(MAT);

        PyObject *npArTup = PyTuple_New(3);
        PyTuple_SetItem(npArTup,0,PyFloat_FromDouble(start));
        PyTuple_SetItem(npArTup,1,PyFloat_FromDouble(stop));
        PyTuple_SetItem(npArTup,2,PyFloat_FromDouble(step));
        PyObject *npAr = basepy(np,NPARNG,npArTup);

        PyObject *npSinTup = PyTuple_New(1);
        PyTuple_SetItem(npSinTup,0,npAr);
        PyObject *npSin = basepy(np,NPSIN,npSinTup);

        PyObject *mPlotTup = PyTuple_New(2);
        PyTuple_SetItem(mPlotTup,0,npAr);
        PyTuple_SetItem(mPlotTup,1,npSin);
        PyObject *mPlot = basepy(mat,MPLOT,mPlotTup);

        PyObject *ET = PyTuple_New(0);
        PyObject *mPlotShow = basepy(mat,MSHOW,ET);

        // Py_CLEAR section:
        Py_CLEAR(mPlotShow);
        Py_CLEAR(ET);
        Py_CLEAR(mPlot);
        Py_CLEAR(mPlotTup);
        Py_CLEAR(npSin);
        Py_CLEAR(npSinTup);
        Py_CLEAR(npAr);
        Py_CLEAR(npArTup);
        Py_CLEAR(mat);
        Py_CLEAR(np);

        return ret;
    }

    // **********************
    // start pycomplex section:
    
    /// @brief base Pycomplex function that converts a std::complex<double> to a PyObject of type PyComplex
    /// @param cc std::complex<double>
    /// @return PyObject of type PyComplex
    PyObject *Pycomplex(complex<double> cc) {
        Py_complex pc;
        pc.real = cc.real();
        pc.imag = cc.imag();
        PyObject *ret = PyComplex_FromCComplex(pc);
        return ret;
    }
    
    /// @brief Pycomplex function that uses a double r, double i to build a std:: complex and call Pycomplex(complex<double> cc) base function.
    /// @param r double representing real value of to-be-created std::complex object, used to generate PyComplex.
    /// @param i double representing imagenary value of to-be-created std::complex object, used to generate PyComplex.
    /// @return PObject of type PyComplex
    PyObject *Pycomplex(double r, double i) {
        complex<double> c;
        c.real(r);
        c.imag(i);
        PyObject *ret = Pycomplex(c);
        return ret;
    }

    /// @brief Converts PyObject or type PyComplex to std::complex<double> C++ object. Performs PyComplex_Check.
    /// @param pc PyObject of type PyComplex to be converted.
    /// @return Converted std::complex<double> from passed in PyObject of type PyComplex.
    complex<double> Py2Ccomplex(PyObject *pc) {
        complex<double> ret;
        if(!PyComplex_Check(pc)) {
            throw runtime_error("not a valid PyComplex_Type. Fatal error.");
        }

        Py_complex pcomp = PyComplex_AsCComplex(pc);
        ret.real(pcomp.real);
        ret.imag(pcomp.imag);
        return ret;
    }

    // end pycomplex section:
    // **********************

    /// @brief Loads numpy.random, and calls the random function to generate a PyFloat type.
    /// @return float representing randomly generated value from numpy.random.random()
    float Random() {
        float ret;
        PyObject *pNPRAND = modimp(NPRAND);
        PyObject *pNPrnd = basepy(pNPRAND,NPRANDRD,PyTuple_New(0));
        ret = PyFloat_AsDouble(pNPrnd);

        // Py_CLEARs:
        Py_CLEAR(pNPrnd);
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
        try {
            Py_Finalize();
            PyMem_RawFree(program);
        }
        catch(...) {
            ret = false;
            PyErr_Print();
        }
        return ret;
    }
}

