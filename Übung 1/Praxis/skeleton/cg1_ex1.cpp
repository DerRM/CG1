/* ----------------------------------------------------------------
   name:           cg1_ex1.c
   purpose:        robot in OpenGL with scenegraph traversal
                   entry point, cg1_ws11/12 assignment 1
   version:        SKELETON CODE
   author:         andy nealen
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
#include "scenegraph.h"
#include "node.h"
#include "robot.h"

int main(int argc, char** argv){
    
  // initialize OpenGL context
  Context::init(argc, argv);
  
  // build the robot hierarchy (see robot.cpp)
  Node *root= buildRobot();
  // make scenegraph
  sceneGraph= new SceneGraph(root);
  
  // start GLUT event loop
  glutMainLoop();
  
  return 0;
}
