/* ----------------------------------------------------------------
   name:           context.cpp
   purpose:        GL context declarations and prototypes of GLUT callbacks
   version:	   SKELETON CODE
   TODO:           menu, keyPressed, idle(optional)
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
 
#include <math.h>
#include "scenegraph.h"
#include "context.h"

// a bunch of variables

// window dimensions
int Context::width, Context::height;
// initial window position
int Context::x, Context::y;
// window title
string Context::title;
// field of view (in degrees)
GLfloat Context::fov;
// camera position
GLfloat Context::cameraZ;
// near and far plane
GLfloat Context::nearPlane, Context::farPlane;
// left mouse button pressed?
bool Context::leftButton;
// mouse position in previous frame
int Context::mouseX, Context::mouseY;

// set parameters to your own liking 
// (or leave them as they are)

// light and material
GLfloat Context::materialAmbient[]= {0.5, 0.5, 0.5, 1.0};
GLfloat Context::materialSpecular[]= {0.3, 0.3, 0.3, 1.0};
GLfloat Context::materialShininess[]= { 3.0 };
GLfloat Context::lightModelAmbient[]= { 0.3, 0.3, 0.3 };
GLfloat Context::lightPosition[]= { 5.0, 5.0, 5.0, 0.0 };

void Context::config(){

  // window size and position
  width= 600;
  height= 600;
  x= 100;
  y= 100;
  title= "cg1 assignment 1 - robot scenegraph";

  // camera setup
  fov= 40.0;
  cameraZ= (height/2) / tan(fov/180.0);
  nearPlane= cameraZ/10.0;
  farPlane= cameraZ*10.0;
}

void Context::init(int argc, char **argv){

  // configurate contexts
  config();

  // create window with glut
  glutInit(&argc, argv);

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowSize(width, height);
  glutInitWindowPosition(x, y);
  glutCreateWindow(title.c_str());
   
  // light and material
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
  glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  // shading
  glShadeModel(GL_FLAT);
  
  // clear background to black and clear depth buffer
  glClearColor(0.0,0.0,0.0,1.0);
  
  // enable depth test (z-buffer)
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  
  // enable normalization of vertex normals
  glEnable(GL_NORMALIZE);
  
  // initial view definitions
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // perspective projection
  gluPerspective(fov, width/height, nearPlane, farPlane);

  registerCallbacks();

  // some output to console
  cout << "--------------------------------------------\n";
  cout << " cg1_ex1 opengl robot scenegraph            \n";
  cout << "                                            \n";
  cout << " keyboard:                                  \n";
  cout << " arrow keys: select node                    \n";
  cout << " x/X,y/Y,z/Z: rotate node                   \n";
  cout << " r: reset all rotations                     \n";
  cout << " q/Q: quit program                          \n";
  cout << "                                            \n";
  cout << " mouse:                                     \n";
  cout << " right click: config menu                   \n";
  cout << " left click+drag: rotate selected node      \n";
  cout << "--------------------------------------------\n";
}

// display callback for GLUT
void Context::display(void){
  
  // clear color and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // switch to opengl modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // position the camera at (0,0,cameraZ) looking down the
  // negative z-axis at (0,0,0)
  gluLookAt(0.0, 0.0, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
  // draw the scenegraph
  sceneGraph->traverse();

  // display back buffer
  glutSwapBuffers();
}

// reshape-Callback for GLUT
void Context::reshape(int w, int h){
  
  width=w;
  height=h;

  // reshaped window aspect ratio
  float aspect = (float) w / (float) h;
  
  // viewport
  glViewport(0,0, (GLsizei) w, (GLsizei) h);

  // restore view definition after window reshape
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // perspective projection
  gluPerspective(fov, aspect, nearPlane, farPlane);

  display();
}

// keyboard callback
void Context::keyPressed(unsigned char key, int x, int y){
	
  float step= 2.0;

  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(key){
    
  case 'q':
  case 'Q': exit(0);
  case 'x':
    sceneGraph->rotate(step, 0, 0);
    display();
    break;
  case 'X': 
    sceneGraph->rotate(-step, 0, 0);
    display();
    break;
  case 'y':
    sceneGraph->rotate(0, step, 0);
    display();
    break;
  case 'Y' :
    sceneGraph->rotate(0, -step, 0);
    display();
    break;  
  case 'z':
    sceneGraph->rotate(0, 0, step);
    display();
    break;
  case 'Z': 
    sceneGraph->rotate(0, 0, -step);
    display();
    break;
      // XXX: reset rotations

    // INSERT YOUR CODE HERE
  case 'r':
  case 'R':
     sceneGraph->reset();
     display();
     break;

    // END XXX

  default:
    break;
  }
}

// keyboard callback for special keys
// (arrow keys for node selection)
void Context::specialKeys(int key, int x, int y){

  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(key) {

  case GLUT_KEY_UP:
    sceneGraph->up();
    display();
    break;
  case GLUT_KEY_DOWN:
    sceneGraph->down();
    display();
    break;
  case GLUT_KEY_LEFT:
    sceneGraph->left();
    display();
    break;
  case GLUT_KEY_RIGHT:
    sceneGraph->right();
    display();
    break;
  default:
    break;
  }
}

// the right button mouse menu
// TODO: add a reset option
// for all rotations
// see also registerCallbacks
// you may also add config options
// like selection  of different
// animations here (optional)
// XXX: NEEDS TO BE IMPLEMENTED
void Context::menu(int id){

  switch (id) {
  case 1: 
    delete sceneGraph;
    exit(0);
  
    // XXX: reset rotations

    // INSERT YOUR CODE HERE
  case 2:
          sceneGraph->reset();
          display();
          break;
    // END XXX
    
    // XXX: add more options (optional)
  
    // INSERT YOUR CODE HERE
    
    // END XXX

  default:
    break;
  }
}

// mouse motion
void Context::mouseMoved(int x, int y){

  // rotate selected node when left mouse button is pressed
  if (leftButton) {
    sceneGraph->rotate((float) (y-mouseY), (float) (x-mouseX), 0);
    mouseX = x;
    mouseY = y;
    display();
  }
}

// mouse callback
void Context::mousePressed(int button, int state, int x, int y){
  
  if (button == GLUT_LEFT) {
    if (state == GLUT_UP) {
      leftButton= false;
    }
    else if (state == GLUT_DOWN) {
      leftButton= true;  
      mouseX = x;
      mouseY = y;
    }
  }
}

// playground (not registered)
void Context::idle(void){}

// register callbacks with GLUT
void Context::registerCallbacks(void){
  
  glutDisplayFunc(display);
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(specialKeys);
  glutReshapeFunc(reshape);
  glutMotionFunc(mouseMoved);
  glutMouseFunc(mousePressed);
  glutCreateMenu(menu);
  glutAddMenuEntry("quit",1);
  
  // XXX: add reset option
  
  // INSERT YOUR CODE HERE
    glutAddMenuEntry("reset rotations", 2);
  // END XXX

  // XXX: add more options (optional)
  
  // INSERT YOUR CODE HERE
  
  // END XXX
  
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  return;
}
