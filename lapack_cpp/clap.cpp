// file: clap.cpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Fortran-LAPACK interop. LAPACK is used by Python numpy 
// module. This C++ app showcases the ability to retrieve the same functionality as numpy.linalg by
// exposing the same Fortran LAPACK subroutines, as extern "C" calls. Creating a comparable C++ set of
// header files, allowing for comparison of Python numpy.linalg vs. C++, for specific LAPACK subroutines
// *****************************************************************************************************
#include <iostream>
#include <fstream>
#include "include/clap.hpp"

clap::dgeevparms dgeevparms_fromfile(string fn, string *fdata) {
    clap::dgeevparms ret;
    int rows,cols,idx;
    double fd;
    ifstream file(fn);
    if(!file.is_open()) {
        cout << " file: " << fn << " does not exit or could not be openned." << endl;
        throw runtime_error("fatal error in opening matrix file. dgeevparms_fromfile().");
    }
    else {
        
        try
        {
            // retrieve rows x columns as expected format: 2 2, etc.
            file >> rows;
            fdata->append(to_string(rows));
            fdata->append(" ");
            file >> cols;
            fdata->append(to_string(cols));
            fdata->append(" ");
            fdata->append("\n");
            
            // initialize clap::dgeevparms:
            ret = clap::newdgeevparms(rows,cols);

            // linearize, general square matrix data:
            for(int x=0;x<rows;x++) {
                for(int y=0;y<cols;y++) {
                    file >> fd;
                    fdata->append(to_string(fd));
                    fdata->append(" ");
                    idx = y*rows+x;
                    ret.matrix_data[idx] = fd;
                }
                fdata->append("\n");
            }

            file.close();
            return ret;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            throw runtime_error("fatal error reading matrix file. dgeevparms_fromfile.");
        } 
    }
}

clap::sgeevparms sgeevparms_fromfile(string fn, string *fdata) {
    clap::sgeevparms ret;
    int rows,cols,idx;
    float fd;
    ifstream file(fn);
    if(!file.is_open()) {
        cout << " file: " << fn << " does not exit or could not be openned." << endl;
        throw runtime_error("fatal error in opening matrix file. sgeevparms_fromfile().");
    }
    else {
        
        try
        {
            // retrieve rows x columns as expected format: 2 2, etc.
            file >> rows;
            fdata->append(to_string(rows));
            fdata->append(" ");
            file >> cols;
            fdata->append(to_string(cols));
            fdata->append(" ");
            fdata->append("\n");
            
            // initialize clap::sgeevparms:
            ret = clap::newsgeevparms(rows,cols);

            // linearize, general square matrix data:
            for(int x=0;x<rows;x++) {
                for(int y=0;y<cols;y++) {
                    file >> fd;
                    fdata->append(to_string(fd));
                    fdata->append(" ");
                    idx = y*rows+x;
                    ret.matrix_data[idx] = fd;
                }
                fdata->append("\n");
            }

            file.close();
            return ret;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            throw runtime_error("fatal error reading matrix file. sgeevparms_fromfile.");
        } 
    }
}

int main(int argc, char *argv[]) {
    // params:
    clap::dgeevparms dvp;
    clap::sgeevparms svp;
    string fn,func;
    string fdata;
    double *lmd;
    float *lmf;
    string lms;

    if(argc<3) {
        cout << " expected: " << argv[0] << " -<func>(sgeev or dgeev) <matrix filename>" << endl;
        return 1;
    }
    else {

        func = argv[1];
        fn = argv[2];
        
        try
        {
            if(func=="-sgeev") {
                svp = sgeevparms_fromfile(fn,&fdata);
            }
            else if(func=="-dgeev") {
                dvp = dgeevparms_fromfile(fn,&fdata);
            }
            else {
                throw runtime_error("./clap - invalid function.");
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return -1;
        }
    }

    if(func=="-dgeev") {
        lmd = dvp.matrix_data;
    }
    else if(func=="-sgeev") {
        lmf = svp.matrix_data;
    }
    cout << endl;
    cout << " [./clap - C++ LAPACK Interop Examples]" << endl;
    cout << endl;
    cout << " eigenvalues | " << argv[0] << " " << func << endl;
    cout << " -----------------------------------" << endl;
    cout << " For file: " << fn << endl;
    cout << " File Contents:" << endl;
    cout << " --------------" << endl;
    cout << fdata << endl;
    cout << " --------------" << endl;
    cout << endl;
    if(func=="-dgeev") {
        cout << " Matrix size: " << dvp.rows << "x" << dvp.cols << endl;
    }
    else if(func=="-sgeev") {
        cout << " Matrix size: " << svp.rows << "x" << svp.cols << endl;
    }
    cout << " Linearized Matrix Data Used: " << endl;
    cout << " [";
    if(func=="-dgeev") {
        
        for(int i=0;i<(dvp.rows*dvp.cols);i++) {
            lms += to_string(lmd[i]);
            lms += ",";
        }
        lms = lms.substr(0,lms.size()-1);
    }
    else if(func=="-sgeev") {
        for(int i=0;i<(svp.rows*svp.cols);i++) {
            lms += to_string(lmf[i]);
            lms += ",";
        }
        lms = lms.substr(0,lms.size()-1);
    }
    cout << lms << "]" << endl;

    cout << endl;
    if(func=="-dgeev") {
        cout << " --- EIGVALS (clap::dgeev) ---" << endl;
        cout << " values = [real:imag]" << endl;
        vector<complex<double>> eigvals = clap::dgeev(dvp);
        for(complex<double> cd : eigvals) {
            cout << " [" << to_string(cd.real());
            cout << ":" << to_string(cd.imag()) << "] \n";
        }
        delete [] dvp.matrix_data;
        delete [] dvp.oimags;
        delete [] dvp.oreals;
    }
    else if(func=="-sgeev") {
       cout << " --- EIGVALS (clap::sgeev) ---" << endl;
        cout << " values = [real:imag]" << endl;
        vector<complex<float>> eigvals = clap::sgeev(svp);
        for(complex<float> cf : eigvals) {
            cout << " [" << to_string(cf.real());
            cout << ":" << to_string(cf.imag()) << "] \n";
        } 
        delete [] svp.matrix_data;
        delete [] svp.oimags;
        delete [] svp.oreals;
    }

    cout << endl;
    
    return 0;
}