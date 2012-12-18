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


// includes, file
#include "ErrorDevel.h"

// includes, system
#ifdef _WIN32
  #include <windows.h>
#endif

#include <iostream>
#include <sstream>

// includes, GL
#ifdef __APPLE__
  #include <GL/glew.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/glew.h>
  #include <GL/glu.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
//! Check for OpenGL errors (including GLSLang).
//! @return true if no error was pending, else fals
////////////////////////////////////////////////////////////////////////////////////////////////////
/* static */ void
ErrorDevel::checkErrorGL(const char* file, const int line) {

  GLenum gl_error = glGetError();
  if (gl_error != GL_NO_ERROR) {

    std::ostringstream s(std::string("gl exception:\n"));
    s << "GL Error in file  -- " << file << " -- in line " << line << " :\n";
    s << gluErrorString(gl_error);

    // here throwing different exceptions depending on the error type would be useful

    std::cerr << s.str() << '\n' << std::endl;
    exit( EXIT_FAILURE);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//! Print info log for glsl program object. The log is empty if no errors / warning occur.
//! @param program_obj handle to the program / shader object
////////////////////////////////////////////////////////////////////////////////////////////////////
/*static*/ void
ErrorDevel::checkInfologGLSL( const unsigned int& program_obj) {
  
  // errror or only warnings
  bool error = false;

  int infolog_length;
  glGetObjectParameterivARB( program_obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infolog_length);

  // bigger then 1 important while length('\n') == 1
  if (infolog_length > 1) {
    
    char* infolog = new GLcharARB[infolog_length];

    int readback_length; 
    glGetInfoLogARB( program_obj, infolog_length, &readback_length, infolog);

    //std::cerr << "GLSL infolog reports:\n" << infolog << '\n' << std::endl;
    std::cerr << infolog << '\n' << std::endl;

    // check if errors occured or only warnings
    std::string str_infolog( infolog);
    error = ( std::string::npos != str_infolog.find( "error")) ? true : false;

    delete[] infolog;
  }

  if (error) {

    // call exit handler

    // exit
    exit( EXIT_SUCCESS);
  }
} 


