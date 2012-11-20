/* ----------------------------------------------------------------
   name:           cg1_ex2.cpp
   purpose:        projection tutorial
                   entry point, cg1_ws12/13 assignment 2
   version:        SKELETON CODE
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include <iostream>
#ifdef __APPLE__ 
#include <GLUT/glut.h>
#elif _WIN32
#include "win32/glut.h"
#else
#include <GL/glut.h>
#endif

#include "context.h"
#include "objmodel.h"

int main(int argc, char** argv){
    
  // initialize OpenGL context
  Context::init(argc, argv);

  // start GLUT event loop
  glutMainLoop();
  
  return 0;
}
