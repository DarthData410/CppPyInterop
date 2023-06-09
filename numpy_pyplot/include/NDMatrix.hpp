// file: NDMatrix.hpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Python interop, using Python.h from within C++ apps.
// *****************************************************************************************************
#pragma once

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
        bool square;   
    } Matrix;

    /// @brief class used to house N-Dimensional Matrix values of doubles. Including base operations. Used with Python
    /// calls for transfer between NDMatrix (C++) and numpy.NDArray (Python).
    class NDMatrix {
    private:
        /// @brief internal storage struct for NDMatrix object instance
        Matrix mat;

    public:
        
        /// @brief NDMatrix basic math operation functions, left+right|left-right|left*right|left/right values. 
        /// Checks for NDMatrix size matching. Throws runtime_error() if not.
        /// @param left NDMatrix left, to-be-oper with right
        /// @param right NDMatrix right, to-be-oper with left
        /// @param t Type of operation: 0=sum,1=subtract,2=multiple,3=divide
        /// @return operation of left & right, same sized, matrices
        static NDMatrix oper(NDMatrix left, NDMatrix right, int t) {
            if(left.size()!=right.size()) {
                throw runtime_error("cpy::NDMatrix sum(left,right) - size mismatch. NDMatrix sizes must match for sum operation.");
            }
            NDMatrix ret = NDMatrix(left.mat.rows,left.mat.cols);

            for(int i=0;i<left.mat.rows;i++) {
                for(int x=0;x<left.mat.cols;x++) {
                    double sv;
                    switch(t) {
                        case 0 : sv = (left.mat.values[i][x]+right.mat.values[i][x]); break;
                        case 1 : sv = (left.mat.values[i][x]-right.mat.values[i][x]); break;
                        case 2 : sv = (left.mat.values[i][x]*right.mat.values[i][x]); break;
                        case 3 : sv = (left.mat.values[i][x]/right.mat.values[i][x]); break;
                        default: sv = 0.0; break;
                    }
                    
                    ret.set(i,x,sv);
                }
            }

            return ret;
        }

        /// @brief Adds left+right NDMatrix instances
        /// @param left NDMatrix left
        /// @param right NDMatrix right
        /// @return NDMatrix with values that are the sum of left+right NDMatrix values
        static NDMatrix add(NDMatrix left, NDMatrix right) {
            return oper(left,right,0);
        }

        /// @brief Subtracts left-right NDMatrix instances
        /// @param left NDMatrix left
        /// @param right NDMatrix right
        /// @return NDMatrix with values that are the sum of left-right NDMatrix values
        static NDMatrix subtract(NDMatrix left, NDMatrix right) {
            return oper(left,right,1);
        }

        /// @brief Multiples left*right NDMatrix instances
        /// @param left NDMatrix left
        /// @param right NDMatrix right
        /// @return NDMatrix with values that are the sum of left*right NDMatrix values
        static NDMatrix multiply(NDMatrix left, NDMatrix right) {
            return oper(left,right,2);
        }

        /// @brief Divides left/right NDMatrix instances
        /// @param left NDMatrix left
        /// @param right NDMatrix right
        /// @return NDMatrix with values that are the sum of left/right NDMatrix values
        static NDMatrix divide(NDMatrix left, NDMatrix right) {
            return oper(left,right,3);
        }

        /// @brief NDMatrix initializer
        /// @param rows number of rows for NDMatrix
        /// @param cols number of cols for NDMatrix
        NDMatrix(int rows, int cols) {
            mat.rows = rows;
            mat.cols = cols;
            mat.size = to_string(rows) + "x" + to_string(cols);
            if(rows==cols) {
                mat.square = true;
            }
            else {
                mat.square = false;
            }
            clear();
        }

        /// @brief Clears all values for instanct of NDMatrix
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

        /// @brief Function that exposes Matrix.square bool value
        /// @return True if Matrix rows x cols create a square Matrix. False if not.
        bool square() {
            return mat.square;
        }

        /// @brief Sets double value at row x col for instance of NDMatrix
        /// @param row row for double value
        /// @param col column for double value
        /// @param v double value for row x column
        void set(int row, int col, double v) {
            mat.values[row][col] = v;
        }

        /// @brief Gets double value at row x col for instance of NDMatrix
        /// @param row row for double value
        /// @param col column for double value
        /// @return double value from row x column for instance of NDMatrix
        double get(int row, int col) {
            return mat.values[row][col];
        }

        /// @brief C-vector<double>(s) values of column for instance of NDMatrix
        /// @param col column of values to retrieve
        /// @return C-vector<double>(s) of column values
        vector<double> getcol(int col) {
            vector<double> ret;

            for(vector<double> vd : mat.values) {
                ret.push_back(vd[col]);
            }
            return ret;
        }

        /// @brief String representation of NDMatrix.getcol(int col)
        /// @param col column of values to retrieve
        /// @return String representation of NDMatrix.getcol()
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

        /// @brief C-vector<double>(s) values of rows for instance of NDMatrix
        /// @param row row of values to retrieve
        /// @return C-vector<double>(s) of rows values
        vector<double> getrow(int row) {
            vector<double> ret;
            ret = mat.values[row];
            return ret;
        }

        /// @brief String representation of NDMatrix.getrow(int row)
        /// @param row row of values to retrieve
        /// @return String representation of NDMatrix.getrow()
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

        /// @brief Generates a tuple of {rows,cols} representing the NDMatrix size
        /// @return tuple{rows,cols} (tuple<int,int>)
        tuple<int,int> size() {
            tuple<int, int> ret;
            ret = {mat.rows,mat.cols};
            return ret;
        }

        /// @brief String representation of NDMatrix size
        /// @return NDMatrix.size string value
        string sizestr() {
            return mat.size;
        }

        /// @brief String representation of NDMatrix
        /// @return Returns both NDMatrix[sizestr()], along with actual NDMatrix rows:cols:values
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