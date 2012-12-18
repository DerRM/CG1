/// @file
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Copyright (C) 2011      Computer Graphics, TU Berlin
///
////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  module     : main.cpp
///
///  project    : Computer Graphics 1
/// 
///  description: main functions including GL callbacks
///
////////////////////////////////////////////////////////////////////////////////////////////////////

// includes file
#include "Checker.h"

// includes, project
#include "ErrorDevel.h"

#ifdef __APPLE__ 
#include <GLUT/glut.h>
#elif _WIN32
#include "gl/glut.h"
#else
#include <GL/glut.h>
#endif

namespace Checker {

////////////////////////////////////////////////////////////////////////////////////////////////////
//! Print transpose of the given 4x4 matrix (no range check)
//! @param matrix handle to the array containing the matrix data (in column row order as OpenGL)
////////////////////////////////////////////////////////////////////////////////////////////////////
void
printTranspose4x4( float* matrix) {

  for( unsigned int k=0; k<4; ++k) {

    std::cout << matrix[k] << " / "
              << matrix[k + 4] << " / "
              << matrix[k + 8] << " / "
              << matrix[k + 12] << '\n';
  }

  std::cout << '\n';
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//! Print 4x4 matrix (no range check)
//! @param matrix handle to the array containing the matrix data (in column row order as OpenGL)
////////////////////////////////////////////////////////////////////////////////////////////////////
void
print4x4( float* matrix) {

  for( unsigned int k=0; k<4; ++k) {
 
    std::cout << matrix[4*k] << " / "
              << matrix[4*k + 1] << " / "
              << matrix[4*k + 2] << " / "
              << matrix[4*k + 3] << '\n';
  }

  std::cout << '\n';
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//! Readback and print one of the current buffers. Intended to use with pbuffer.
//! @param buffer OpenGL constant of the buffer to print (e.g. GL_FRONT)
////////////////////////////////////////////////////////////////////////////////////////////////////
void
checkMatrices() {

  float matrix[16];

  glGetFloatv( GL_PROJECTION_MATRIX, matrix);

  printTranspose4x4( matrix);

  glGetFloatv( GL_MODELVIEW_MATRIX, matrix);

  printTranspose4x4( matrix);

  std::cout << '\n' << std::endl;
}

}  // end namespace Checker
