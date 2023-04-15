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

#define APP "cnumpy"

// defines for py modules/functions/attributes:
#define pyLIST "list"
#define pyLIST_A "append"

// numpy:
#define NP "numpy"
#define NPCOS "cos"
#define NPPi "pi"
#define NPSIN "sin"
#define NPDOT "dot"
#define NPRAND "random"
#define NPDIAG "diag"

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

namespace cnp {
    
    /// @brief C++ call to Python numpy.cos(x). Building a list python object and passing for operation. Works with 
    /// numpy.ndarray size and item values for converting back to vector of type floats being consine of passed in values.  
    /// Showcasing interop between C++ <-> Python using Python.h.
    ///
    /// Example: 
    ///  vector<double> ncosvret = cnp::cos(inFVec<double>);
    ///
    /// @param inFVec: Vector of double values, used to convert to numpy.ndarray for operating on.
    /// @return Vector<double> values calculated as cosine values from inFVec values, returned from Python, numpy.cos(x)
    vector<double> cos(vector<double> inFVec)
    {
        vector<double> ret;
        
        PyObject *np = PyImport_ImportModule(NP);
        PyObject *plist = PyList_New(0);
        
        for(double d : inFVec) {
            PyObject *pf = PyFloat_FromDouble(d);
            int plac = PyList_Append(plist,pf);
            if(plac!=0) {
                runtime_error("PyList_Append: failed to add C double/PyFloat_FromDouble.");
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
                runtime_error("Python-numpy-array: return expected long, but PyObject not long.");
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
    float pyPi() {
        
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
    bool pyPlot(float start, float stop, float step) {
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

}

