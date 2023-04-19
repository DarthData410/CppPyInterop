// file: NDMatrix.hpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps.
// *****************************************************************************************************
#include <iostream>
#include <vector>
#include <string>
#include <tuple>

using namespace std;

namespace cpy {
    
    /// @brief typedef of base matrix concept for supporting the creation of a N-Dimensional Matrix of double values
    typedef struct matrix {
        vector<vector<double>> values;
        int rows;
        int cols;
        string size;    
    } Matrix;

    /// @brief class used to house N-Dimensional Matrix values of doubles. Including base operations. Used with Python
    /// calls for transfer between NDMatrix (C++) and numpy.NDArray (Python).
    class NDMatrix {
    private:
        /// @brief internal storage struct for NDMatrix object instance
        Matrix mat;

    public:
        /// @brief NDMatrix sum function. Checks for 
        /// @param left 
        /// @param right 
        /// @return 
        static NDMatrix sum(NDMatrix left, NDMatrix right) {
            if(left.sizestr()!=right.sizestr()) {
                throw runtime_error("cpy::NDMatrix sum(left,right) - size mismatch. NDMatrix sizes must match for sum operation.");
            }
            NDMatrix ret = NDMatrix(left.mat.rows,left.mat.cols);

            for(int i=0;i<left.mat.rows;i++) {
                for(int x=0;x<left.mat.cols;x++) {
                    double sv = (left.mat.values[i][x]+right.mat.values[i][x]);
                    ret.set(i,x,sv);
                }
            }

            return ret;
        }

        NDMatrix(int rows, int cols) {
            mat.rows = rows;
            mat.cols = cols;
            mat.size = to_string(rows) + "x" + to_string(cols);
            clear();
        }

        void clear() {
            mat.values.clear();
            for(int i=0;i<mat.rows;i++) {
                vector<double> vcols;
                for(int x=0;x<mat.cols;x++) {
                    vcols.push_back(0.0);
                }
                mat.values.push_back(vcols);
            }
        }

        void set(int row, int col, double v) {
            mat.values[row][col] = v;
        }

        double get(int row, int col) {
            return mat.values[row][col];
        }

        vector<double> getcol(int col) {
            vector<double> ret;

            for(vector<double> vd : mat.values) {
                ret.push_back(vd[col]);
            }
            return ret;
        }

        string getcolstr(int col) {
            string ret = " NDMatrix[col:"+to_string(col)+"] \n";
            vector<double> cols;
            cols = getcol(col);
            for(double d : cols) {
                ret += "   ";
                ret += "[";
                ret += to_string(d);
                ret += "]\n";
            }
            ret = ret.substr(0,ret.size()-1);
            return ret;
        }

        vector<double> getrow(int row) {
            vector<double> ret;
            ret = mat.values[row];
            return ret;
        }

        string getrowstr(int row) {
            string ret = " NDMatrix[row:"+to_string(row)+"] \n";
            vector<double> rows;
            rows = getrow(row);
            ret += "   ";
            ret += "[";
            for(double d : rows) {
                
                ret += to_string(d);
                ret += "|";
            }
            ret = ret.substr(0,ret.size()-1);
            ret += "]";
            return ret;
        }

        tuple<int,int> size() {
            tuple<int, int> ret;
            ret = {mat.rows,mat.cols};
            return ret;
        }

        string sizestr() {
            return mat.size;
        }

        string str() {
            string ret = " NDMatrix[" + sizestr() + "]\n";
            
            int i = 0;
            for (vector<double> vd : mat.values) {
                ret += "   "; // buffer for text align
                ret += to_string(i);
                ret += ":[";
                int x = 0;
                for (double d : vd) {
                    ret += to_string(d);
                    ret += "|";
                    x++;
                }
                ret = ret.substr(0,ret.size()-1);
                ret += "]\n";
                i++;
            }
            ret = ret.substr(0,ret.size()-1);
            return ret;
        }
    };
}