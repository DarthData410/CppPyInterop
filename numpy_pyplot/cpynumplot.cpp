// file: cnumpy.cpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps.
// ------------------------------------------
// Command line execution app used for interop between C++ -> Python. Enabled by Python.h header file only.
// Showcasing soecific usage of numpy and matplotlib.pyplot modules.
// *****************************************************************************************************

#include "cpynumplot.hpp"
#include "format.hpp"

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
    vector<double> cosvals = cpy::CosVec(invals);
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
static float pypi_float(string _sr) {
    bool upi = false;
    string _spi = "*pi";
    
    size_t found=_sr.find(_spi);
    if( found != string::npos) {
        upi = true;
        // using <algorithm>::replace() function here:
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

/// @brief ./cpynumplot command line program main execution point
int main(int argc, char *argv[])
{
    format f = format();
    if(argc<2) {

        cerr << " expected usage: ./cpynumplot -<function> <vals> ..." << endl;
        return 1;
    }
    
    // Initialize cpy from binary argv name: ./cpynumplot
    wchar_t *program = cpy::init(argv[0]);

    string func = argv[1];
    cout << "\n ----- cpynumplot -----" << endl;
    if(func=="-cos") {
        string val = argv[2];
        float _f = pypi_float(val);
        double retv = cpy::CST(_f,0);
        cout << " function: numpy.cos(x) [Single Value]" << endl;
        cout << " ----------------------" << endl;
        cout << " in-value: " << val << ", real: " << _f << endl;
        cout << " cos-value: " << retv << endl;
    }
    else if(func=="-cosvec") {
        string vals = argv[2];
        string cosvals = excos(vals);
        cout << " function: numpy.cos(x) [C-vector]" << endl;
        cout << " ----------------------" << endl;
        cout << " in-value(s): " << vals << endl;
        cout << " cos-value(s): " << cosvals << endl;
    }
    else if(func=="-diagvec") {
        array<vector<double>,3> in;
        cout << " function: numpy.diag(x) [C-vector(s)]" << endl;
        cout << " ----------------------" << endl;
        vector<double> inv0 = getDvectorFrStr(argv[2]);
        in[0] = inv0;
        cout << " in-value(s)[1]: " << argv[2] << endl;
        if(argc>=4) {
            vector<double> inv1 = getDvectorFrStr(argv[3]);
            in[1] = inv1;
            cout << " in-value(s)[2]: " << argv[3] << endl;
        }
        if(argc==5) {
            vector<double> inv2 = getDvectorFrStr(argv[4]);
            in[2] = inv2;
            cout << " in-value(s)[3]: " << argv[4] << endl;
        }
        
        vector<double> dvret = cpy::DiagVec(in);
        string drs = "[";
        for(double d : dvret) {
            drs += to_string(d);
            drs += ",";
        }
        drs = drs.substr(0,drs.size()-1);
        drs += "]";
        cout << " diag-values: " << drs << endl;
    }
    else if(func=="-pi") {
        string pret = to_string(cpy::Pi());
        cout << " function: numpy.pi" << endl;
        cout << " ------------------" << endl;
        cout << " return: " << pret << endl;
    }
    else if(func=="-pyplot") {
        
        if(argc<5) {
            cerr << " "+f.red+"usage error:"+f.none+" \n expected usage: ./cpynumplot -pyplot <start> <stop> <step> \n "+f.blue+"ie:"+f.none+" "+f.greentxt+"./cpynumplot"+f.goldtxt+" -pyplot"+f.none+" 0.1 3"+f.goldtxt+"*"+f.greentxt+"pi"+f.none+" 0.12 \n" << endl;
            return 1; 
        }

        string strstart = argv[2];
        float start = pypi_float(strstart);
        string strstop = argv[3];
        float stop = pypi_float(strstop);
        string strstep = argv[4];
        float step = pypi_float(strstep);
        
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
    else if(func=="-sin") {
        string val = argv[2];
        float _f = pypi_float(val);
        double retv = cpy::CST(_f,1);
        cout << " function: numpy.sin(x) [Single Value]" << endl;
        cout << " ----------------------" << endl;
        cout << " in-value: " << val << ", real: " << _f << endl;
        cout << " sin-value: " << retv << endl;    
    }
    else if(func=="-tan") {
        string val = argv[2];
        float _f = pypi_float(val);
        double retv = cpy::CST(_f,2);
        cout << " function: numpy.tan(x) [Single Value]" << endl;
        cout << " ----------------------" << endl;
        cout << " in-value: " << val << ", real: " << _f << endl;
        cout << " tan-value: " << retv << endl;    
    }
    cout << endl;

    // call cpy::finalize() to close Python program out, and free memory up.:
    if(!cpy::finalize(program)) {
        cerr << "problems finalizing python program. fatal errors." << endl;
        return 1;
    }
       
    return 0;
}