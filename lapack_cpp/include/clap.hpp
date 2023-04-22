// file: clap.hpp
// author: J. Brandon George | darth.data410@gmail.com | @pyfryday
// contents used for testing against C++ <-> Fortran-LAPACK interop. LAPACK is used by Python numpy 
// module. This C++ app showcases the ability to retrieve the same functionality as numpy.linalg by
// exposing the same Fortran LAPACK subroutines, as extern "C" calls. Creating a comparable C++ set of
// header files, allowing for comparison of Python numpy.linalg vs. C++, for specific LAPACK subroutines
// *****************************************************************************************************
// Header file used to combine clap::* function header files. Enabling a single point of include for
// *.cpp app usage.

#pragma once

#include "functions/dgeev.hpp"
#include "functions/sgeev.hpp"
#include "functions/drot.hpp"
