/* ----------------------------------------------------------------
   name:           context.h
   purpose:        GL context class declaration, prototypes of GLUT callbacks
   version:	   SKELETON CODE
   TODO:           nothing (see context.cpp)
   author:         katrin lang
		   computer graphics
		   tu berlin
   ------------------------------------------------------------- */

#pragma once

#include <string>

using namespace std;

/* ------------------------------------------------ 
    implemented glut callbacks:
    init               - initialize OpenGL
    display            - display scene
    reshape            - reshape window
    keyPressed         - keyboard callbacks
    menu               - right mouse button menu
    mouseMoved         - apply rotation when dragging
    mousePressed       - register mouse clicks
    registerCallbacks  - register all callbacks with GLUT
   ------------------------------------------------ */

namespace Context{

  // window dimensions
  extern int width, height;
  // initial window position
  extern int x, y;
  // window title
  extern string title;
  // field of view (in degrees)
  extern GLfloat fov;
  // camera position
  extern GLfloat cameraZ;
  // near and far plane
  extern GLfloat nearPlane, farPlane;
  // material ambient color
  extern GLfloat materialAmbient[4];
  // material specular color
  extern GLfloat materialSpecular[4];
  // material shininess
  extern GLfloat materialShininess[1];
  // ambient color
  extern GLfloat lightModelAmbient[3];
  // light position
  extern GLfloat lightPosition[4];
  // left mouse button pressed?
  extern bool leftButton;
  // mouse position in previous frame
  extern int mouseX, mouseY;

  // initialize above variables
  void config();
  
  // intialization                                             
  void init(int argc, char **argv);

  //register callbacks with GLUT
  void registerCallbacks(void);

  // display scene
  // register with glutDisplayFunc(display)
  void display(void);
  
  // redisplay scene after window reshape
  // register with glutReshapeFunc(reshape)
  void reshape(int width, int height);
  
  // mouse callback
  // register with glutMouseFunc(mousePressed)
  void mousePressed(int btn, int state, int x, int y);
  
  // keyboard callback
  // register with glutKeyboardFunc(keyPressed)
  void keyPressed(unsigned char key, int x, int y);
  
  // keyboard callback for special keys
  // register with glutSpecialFunc(specialKeys)
  void specialKeys(int key, int x, int y);

  // mouse motion callback
  // register with glutMotionFunc(mouseMoved)
  void mouseMoved(int x, int y);
  
  // mouse menu
  // register with glutCreateMenu(menu)
  // on right mouse button with glutAttachMenu(GLUT_RIGHT_BUTTON);
  void menu(int id);

  // idle callback (for animations)
  // register with glutIdleFunc(idle)
  void idle(void);
};
