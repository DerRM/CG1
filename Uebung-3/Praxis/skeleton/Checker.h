/// @file
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Copyright (C) 2011      Computer Graphics, TU Berlin
///
////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  module     : Checker.h
///
///  project    : Computer Graphics 1
/// 
///  description: Helper functions for checking 
///
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _CHECKER_H_
#define _CHECKER_H_


// includes, system
#include <iostream>
#ifdef _WIN32
  #include <windows.h>
#endif

//! namespace for checker functions
namespace Checker {

  //! Readback and print one of the current buffers. Intended to use with pbuffer.
  //! @param buffer OpenGL constant of the buffer to print (e.g. GL_FRONT)
  void  checkMatrices();

  //! Print 4x4 matrix (no range check)
  //! @param matrix handle to the array containing the matrix data (in column row order as OpenGL)
  void  print4x4( float* matrix);

  //! Print transpose of the given 4x4 matrix (no range check)
  //! @param matrix handle to the array containing the matrix data (in column row order as OpenGL)
  void printTranspose4x4( float* matrix);
}


#endif // _CHECKER_H_
