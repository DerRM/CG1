/// @file
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Copyright (C) 2011      Computer Graphics, TU Berlin
///
////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  module     : ErrorDevel.h
///
///  project    : Computer Graphics 1
/// 
///  description: Several handler / tester functions for errors including
///               GL / GLSLang errors
///
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _ERRORDEVEL_H_
#define _ERRORDEVEL_H_

#include <cstdlib>

//! Class providing the handler / tester functions for errors as static members
class ErrorDevel {

public:

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //! Check for OpenGL errors (including GLSLang).
  //! @return true if no error was pending, else false
  //////////////////////////////////////////////////////////////////////////////////////////////////
  static void 
  checkErrorGL(const char* file = "-" , const int line = -1);

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //! Print info log for glsl program object. The log is empty if no errors / warning occur.
  //! @param program_obj handle to the program / shader object
  //////////////////////////////////////////////////////////////////////////////////////////////////
  static void
  checkInfologGLSL( const unsigned int& program_obj);

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //! Check if a condition is true.
  //! @note In prinicple has the same functionality as assert but allows much better control
  //   this version prints an error and terminates the program, no exception is thrown.
  //////////////////////////////////////////////////////////////////////////////////////////////////
  inline  static void
  condition( bool val, const char* file, const int line);

};

// includes, system
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
//! Check if a condition is true.
//! @note In prinicple has the same functionality as assert but allows much better control
//   this version prints an error and terminates the program, no exception is thrown.
////////////////////////////////////////////////////////////////////////////////////////////////////
/* static */ inline void
ErrorDevel::condition( bool val, const char* file, const int line) {

  if ( ! val) {

    std::cerr << "Condition failed: " << file << " in line " << line << std::endl;


    exit(EXIT_FAILURE);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//! Convenience macros. The macros avoid to write file & line macro each time

#define CHECK_ERROR_GL() ErrorDevel::checkErrorGL( __FILE__, __LINE__)

#define CONDITION( val) ErrorDevel::condition( val, __FILE__, __LINE__)


#endif // _ERRORDEVEL_H_
