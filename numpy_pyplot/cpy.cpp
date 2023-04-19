// file: cpy.cpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps.
// ------------------------------------------
// Command line execution app used for interop between C++ -> Python. Enabled by Python.h header file only.
// Showcasing specific usage of numpy and matplotlib.pyplot modules.
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

/// @brief Generate cpy::NDMatrix random values, based on inm size. Calls cpy::Random() [Python numpy.random.random]
/// @param inm cpy::NDMatrix used populating random values based on size
/// @return Returns cpy::NDMatrix, populated with random values
static cpy::NDMatrix NDMrand(cpy::NDMatrix inm) {
    inm.clear();
    tuple<int,int> inmsz = inm.size();
    int mrows = get<0>(inmsz);
    int mcols = get<1>(inmsz);

    for(int i=0;i<mrows;i++) {
        for(int x=0;x<mcols;x++) {
            double rdv = cpy::Random();
            inm.set(i,x,rdv);  
        }
    }

    return inm;
}

/// @brief ./cpynumplot command line program main execution point
int main(int argc, char *argv[])
{
    string func;
    format f = format();
    if(argc<2) {

        //cerr << " expected usage: ./cpy -<function> <vals> ..." << endl;
        //return 1;
        func = "-matrix-test";
    }
    else {
        func = argv[1];
    }
    
    // Initialize cpy from binary argv name: ./cpy
    wchar_t *program = cpy::init(argv[0]);

    
    cout << "\n ----- cpy -----" << endl;
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
        if(argc>=5) {
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
    else if(func=="-matrix-diag-test") {
        cout << " "+f.greentxt+"cpy::DiagMatrix() tests"+f.none << endl;
        cout << " -------------------" << endl;
        cout << " "+f.greentxt+"Test One: "+f.goldtxt+"NDM("+argv[2]+"x"+argv[3]+")"+f.none << endl;
        int rows = stoi(argv[2]);
        int cols = stoi(argv[3]);
        cpy::NDMatrix m = cpy::NDMatrix(rows,cols);
        m = NDMrand(m);
        cout << m.str() << endl;
        cout << endl;
        m = cpy::DiagMatrix(m);
        cout << m.str() << endl;
        cout << endl; 
    }
    else if(func=="-matrix-test") {
        
        cout << " "+f.greentxt+"cpy::NDMatrix tests"+f.none << endl;
        cout << " -------------------" << endl;
        cout << " "+f.greentxt+"Test One: "+f.goldtxt+"NDM(3x3)"+f.none << endl;
        cpy::NDMatrix m = cpy::NDMatrix(3,3);
        m = NDMrand(m);
        cout << m.str() << endl;
        cout << endl;
    
        cout << " "+f.greentxt+"Test Two: "+f.goldtxt+"NDM(8x8) Col[4]/Row[4]"+f.none << endl;
        cpy::NDMatrix m8 = cpy::NDMatrix(8,8);
        m8 = NDMrand(m8);
        cout << m8.str() << endl;
        cout << m8.getcolstr(4) << endl; 
        cout << m8.getrowstr(4) << endl;
        cout << endl;

        // cpy::NDMatrix basic operation examples:
        cpy::NDMatrix ma = cpy::NDMatrix(4,4);
        cpy::NDMatrix mb = cpy::NDMatrix(4,4);
        cpy::NDMatrix mc = cpy::NDMatrix(4,4);
        cout << " "+f.greentxt+"Test Three: "+f.goldtxt+"ma"+f.greentxt+"+"+f.goldtxt+"mb"+f.greentxt+"="+f.goldtxt+"mc"+f.none << endl;
        ma = NDMrand(ma);
        cout << ma.str() << endl;
        cout << "       "+f.greentxt+"+"+f.none+"" << endl;
        mb = NDMrand(mb);
        cout << mb.str() << endl;
        cout << "       "+f.greentxt+"="+f.none+"" << endl;
        mc = cpy::NDMatrix::add(ma,mb);
        cout << mc.str() << endl;
        cout << endl;

        cout << " "+f.greentxt+"Test Four: "+f.goldtxt+"ma"+f.greentxt+"-"+f.goldtxt+"mb"+f.greentxt+"="+f.goldtxt+"mc"+f.none << endl;
        ma = NDMrand(ma);
        cout << ma.str() << endl;
        cout << "       "+f.greentxt+"-"+f.none+"" << endl;
        mb = NDMrand(mb);
        cout << mb.str() << endl;
        cout << "       "+f.greentxt+"="+f.none+"" << endl;
        mc = cpy::NDMatrix::subtract(ma,mb);
        cout << mc.str() << endl;
        cout << endl;

        cout << " "+f.greentxt+"Test Five: "+f.goldtxt+"ma"+f.greentxt+"*"+f.goldtxt+"mb"+f.greentxt+"="+f.goldtxt+"mc"+f.none << endl;
        ma = NDMrand(ma);
        cout << ma.str() << endl;
        cout << "       "+f.greentxt+"*"+f.none+"" << endl;
        mb = NDMrand(mb);
        cout << mb.str() << endl;
        cout << "       "+f.greentxt+"="+f.none+"" << endl;
        mc = cpy::NDMatrix::multiply(ma,mb);
        cout << mc.str() << endl;
        cout << endl;

        cout << " "+f.greentxt+"Test six: "+f.goldtxt+"ma"+f.greentxt+"/"+f.goldtxt+"mb"+f.greentxt+"="+f.goldtxt+"mc"+f.none << endl;
        ma = NDMrand(ma);
        cout << ma.str() << endl;
        cout << "       "+f.greentxt+"/"+f.none+"" << endl;
        mb = NDMrand(mb);
        cout << mb.str() << endl;
        cout << "       "+f.greentxt+"="+f.none+"" << endl;
        mc = cpy::NDMatrix::divide(ma,mb);
        cout << mc.str() << endl;
        cout << endl;

    }
    else if(func=="-pi") {
        string pret = to_string(cpy::Pi());
        cout << " function: numpy.pi" << endl;
        cout << " ------------------" << endl;
        cout << " return: " << pret << endl;
    }
    else if(func=="-pyplot") {
        
        if(argc<5) {
            cerr << " "+f.red+"usage error:"+f.none+" \n expected usage: ./cpy -pyplot <start> <stop> <step> \n "+f.blue+"ie:"+f.none+" "+f.greentxt+"./cpynumplot"+f.goldtxt+" -pyplot"+f.none+" 0.1 3"+f.goldtxt+"*"+f.greentxt+"pi"+f.none+" 0.12 \n" << endl;
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