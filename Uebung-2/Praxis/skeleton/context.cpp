/* ----------------------------------------------------------------
   name:           context.cpp
   purpose:        GL context declarations and prototypes of GLUT callbacks
   version:        SKELETON CODE
   TODO:           add clip-space window
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include <iostream>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#elif _WIN32
#include "win32/glut.h"
#else
#include <GL/glut.h>
#endif


#include "context.h"
#include "window.h"
#include "projection.h"

using namespace std;

// gap between subwindows
static const int GAP= 25;

// window size and position
static int width= 768+GAP*3;
static int height= 512+GAP*3;

static int subWidth= (width-GAP*3)/3.0;      //TODO fit for 3 windows
static int subHeight= (height-GAP*3)/2.0;

// initial window position
static int x= 100;
static int y= 100;
// window title
static string title= "cg1 assignment 2 - projection";

// camera setup
// field of view (in degrees)
static GLfloat fov= 40.0;
// camera position
static GLfloat cameraZ= (height/2) / tan(fov/180.0);
// near and far plane
static GLfloat nearPlane= cameraZ/10.0;
static GLfloat farPlane= cameraZ*10.0;

static GLfloat lightModelAmbient[]= { 0.3, 0.3, 0.3 };

// glut bitmap font
// see setFont
static GLvoid *font= GLUT_BITMAP_HELVETICA_10;

// redisplay scene after window reshape
static void reshape(int width, int height);

static Window main, world, screen, command, clip;

static void createWindows(void){

  main= Window(NULL, title, x, y, width, height);
  main.registerDisplay(Context::display);
  main.registerReshape(reshape);
  main.registerKeyPressed(keyPressed);

  world= Window(&main, "World-space view", GAP, GAP, subWidth, subHeight);
  world.registerDisplay(World::display);
  world.registerReshape(World::reshape);
  world.registerMenu(World::menu);
  world.addMenu(World::menuOptions, World::menuText, World::numOptions);
  world.registerKeyPressed(keyPressed);

  screen= Window(&main, "Screen-space view", subWidth+2*GAP, GAP, subWidth, subHeight);
  screen.registerDisplay(Screen::display);
  screen.registerReshape(Screen::reshape);
  screen.registerMenu(Screen::menu);
  screen.addMenu(Screen::menuOptions, Screen::menuText, Screen::numOptions);
  screen.registerKeyPressed(keyPressed);

  clip= Window(&main, "Clip-space view", 2*subWidth+3*GAP, GAP, subWidth, subHeight);
  clip.registerDisplay(Clip::display);
  clip.registerReshape(Clip::reshape);
  clip.registerMenu(Clip::menu);
  clip.addMenu(Clip::menuOptions, Clip::menuText, Clip::numOptions);
  clip.registerKeyPressed(keyPressed);

  command= Window(&main, "Command manipulation window", GAP, subHeight+2*GAP, width-2*GAP, subHeight);
  command.registerDisplay(Command::display);
  command.registerReshape(Command::reshape);
  command.registerMenu(Command::menu);
  command.addMenu(Command::menuOptions, Command::menuText, Command::numOptions);
  command.registerMousePressed(Command::mousePressed);
  command.registerMouseMoved(Command::mouseMoved);
  command.registerKeyPressed(keyPressed);

}

void Context::init(int argc, char **argv){

  // create window with glut
  glutInit(&argc, argv);

  createWindows();

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);

  // some output to console
  cout << "--------------------------------------------\n";
  cout << " cg1_ex2 projection tutorial                \n";
  cout << "                                            \n";
  cout << " keyboard:                                  \n";
  cout << " p: perspective projection                  \n";
  cout << " o: orthographic projection                 \n";
  cout << " f: frustum mode                            \n";
  cout << " r: reset                                   \n";
  cout << " q/Q: quit program                          \n";
  cout << "                                            \n";
  cout << " mouse:                                     \n";
  cout << " left click+drag: rotate clip view          \n";
  cout << " right click: window-specific menus         \n";
  cout << "--------------------------------------------\n";
}

// display callback for GLUT
void Context::display(void){

  // select main window
  main.redisplay();

  // clear color and depth buffer
  glClearColor(0.8, 0.8, 0.8, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor4f(0, 0, 0, 1);
  setFont("helvetica", 12);

  drawString(GAP, GAP-5, "World-space view");
  drawString(GAP+subWidth+GAP, GAP-5, "Screen-space view");
  drawString(GAP+subWidth+GAP+GAP+subWidth, GAP-5, "Clip-space view");
  drawString(GAP, GAP+subHeight+GAP-5, "Command manipulation window");

  world.redisplay();
  screen.redisplay();
  clip.redisplay();
<<<<<<< HEAD
  command.redisplay();
=======
  command.redisplay(); 
>>>>>>> mehr zur praxis
}

// reshape-Callback for GLUT
static void reshape(int w, int h){

  width=w;
  height=h;

  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, height, 0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  subWidth= (width-GAP*4)/3.0;
  subHeight= (height-GAP*3)/2.0;

  world.reshape(GAP, GAP, subWidth, subHeight);
  screen.reshape(subWidth+2*GAP, GAP, subWidth, subHeight);
  clip.reshape(2*subWidth+3*GAP, GAP, subWidth, subHeight);
  command.reshape(GAP, subHeight+2*GAP, width-2*GAP, subHeight);

  /*Original COde:
  subWidth= (width-GAP*3)/2.0;
  subHeight= (height-GAP*3)/2.0;

  world.reshape(GAP, GAP, subWidth, subHeight);
  screen.reshape(subWidth+2*GAP, GAP, subWidth, subHeight);
  command.reshape(GAP, subHeight+2*GAP, width-2*GAP, subHeight);
  */
}

// the right button mouse menu
// TODO: add a reset option
// for all rotations
// see also registerCallbacks
// you may also add config options
// like selection  of different
// animations here (optional)
// XXX: NEEDS TO BE IMPLEMENTED
static void menu(int id){

  switch (id) {
  case 1:
    break;
  case 2:
    break;
  default:
    break;
  }
}


// select glut bitmap font
void Context::setFont(string name, int size){

  if(name.compare("helvetica") == 0){
    if(size == 10)
      font= GLUT_BITMAP_HELVETICA_10;
    else if(size == 12)
      font= GLUT_BITMAP_HELVETICA_12;
    else if(size == 18)
      font= GLUT_BITMAP_HELVETICA_18;
  }
  else if(name.compare("times") == 0){
    if(size == 10)
      font= GLUT_BITMAP_TIMES_ROMAN_10;
    if(size == 24)
      font= GLUT_BITMAP_TIMES_ROMAN_24;
  }
  else if(name.compare("8x13") == 0){
    font= GLUT_BITMAP_8_BY_13;
  }
  else if(name.compare("9x15") == 0) {
    font= GLUT_BITMAP_9_BY_15;
  }
}

void Context::drawString(int x, int y, string s){
  glRasterPos2f(x, y);
  for (int i= 0; i<s.length(); i++)
    glutBitmapCharacter(font, s[i]);
}

void Context::drawString(int x, int y, int z, string s){
  glRasterPos3f(x, y, z);
  for (int i= 0; i<s.length(); i++)
    glutBitmapCharacter(font, s[i]);
}
