/* ----------------------------------------------------------------
   name:           projection.cpp
   purpose:        projection tutorial 
                   based on tutorial by Nate Robins, 1997
		   modified for clip-space viewing in the context 
		   of 'introduction to computer graphics' 
		   winter term 2012/2013, assignment 2
   version:	   SKELETON CODE
   TODO:           clip-space view callbacks
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

#include "glm/glm.hpp"
#include "cell.h"
#include "context.h"
#include "projection.h"
#include "objmodel.h"

using namespace std;

// use this with care
// might cause name collisions
using namespace glm;

static enum {
  PERSPECTIVE, FRUSTUM, ORTHO
} mode = PERSPECTIVE;

static mat4 projection, modelView;

static OBJModel model("data/al.obj");

// directional light in positve z-direction
static  vec4 lightPos= vec4(0.0, 0.0, 1.0, 0.0);

static bool leftButton= false;

static int old_x, old_y;

static Cell lookat[9] = {
  Cell(180, 120, -5.0, 5.0, 0.0, 0.1,
       "Specifies the X position of the eye point."),
  Cell(240, 120, -5.0, 5.0, 0.0, 0.1,
       "Specifies the Y position of the eye point."),
  Cell(300, 120, -5.0, 5.0, 2.0, 0.1,
       "Specifies the Z position of the eye point."),
  Cell(180, 160, -5.0, 5.0, 0.0, 0.1,
       "Specifies the X position of the reference point."),
  Cell(240, 160, -5.0, 5.0, 0.0, 0.1,
       "Specifies the Y position of the reference point."),
  Cell(300, 160, -5.0, 5.0, 0.0, 0.1,
       "Specifies the Z position of the reference point."),
  Cell(180, 200, -2.0, 2.0, 0.0, 0.1,
       "Specifies the X direction of the up vector."),
  Cell(240, 200, -2.0, 2.0, 1.0, 0.1,
       "Specifies the Y direction of the up vector."),
  Cell(300, 200, -2.0, 2.0, 0.0, 0.1,
       "Specifies the Z direction of the up vector."),
};

static Cell perspective[4] = {
  Cell(180, 80, 1.0, 179.0, 60.0, 1.0,
       "Specifies field of view angle (in degrees) in y direction."),
  Cell(240, 80, -3.0, 3.0, 1.0, 0.01,
       "Specifies field of view in x direction (width/height)."),
  Cell(300, 80, 0.1, 10.0, 1.0, 0.05,
       "Specifies distance from viewer to near clipping plane."),
  Cell(360, 80, 0.1, 10.0, 10.0, 0.05,
       "Specifies distance from viewer to far clipping plane."),
};

static Cell frustum[6] = {
  Cell(120, 80, -10.0, 10.0, -1.0, 0.1,
       "Specifies coordinate for left vertical clipping plane."),
  Cell(180, 80, -10.0, 10.0, 1.0, 0.1,
       "Specifies coordinate for right vertical clipping plane."),
  Cell(240, 80, -10.0, 10.0, -1.0, 0.1,
       "Specifies coordinate for bottom vertical clipping plane."),
  Cell(300, 80, -10.0, 10.0, 1.0, 0.1,
       "Specifies coordinate for top vertical clipping plane."),
  Cell(360, 80, 0.1, 5.0, 1.0, 0.01,
       "Specifies distance to near clipping plane."),
  Cell(420, 80, 0.1, 5.0, 3.5, 0.01,
       "Specifies distance to far clipping plane."),
};

static Cell ortho[6] = {
  Cell(120, 80, -10.0, 10.0, -1.0, 0.1,
       "Specifies coordinate for left vertical clipping plane."),
  Cell(180, 80, -10.0, 10.0, 1.0, 0.1,
       "Specifies coordinate for right vertical clipping plane."),
  Cell(240, 80, -10.0, 10.0, -1.0, 0.1,
       "Specifies coordinate for bottom vertical clipping plane."),
  Cell(300, 80, -10.0, 10.0, 1.0, 0.1,
       "Specifies coordinate for top vertical clipping plane."),
  Cell(360, 80, -5.0, 5.0, 1.0, 0.01,
       "Specifies distance to near clipping plane."),
  Cell(420, 80, -5.0, 5.0, 3.5, 0.01,
       "Specifies distance to far clipping plane."),
};


void setMatrices(void){

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  if(mode == PERSPECTIVE)
    gluPerspective(perspective[0].getValue(), perspective[1].getValue(), 
		   perspective[2].getValue(), perspective[3].getValue());
  else if(mode == ORTHO)
    glOrtho(ortho[0].getValue(), ortho[1].getValue(), ortho[2].getValue(),
	    ortho[3].getValue(), ortho[4].getValue(), ortho[5].getValue());
  else if(mode == FRUSTUM)
    glFrustum(frustum[0].getValue(), frustum[1].getValue(), frustum[2].getValue(),
	      frustum[3].getValue(), frustum[4].getValue(), frustum[5].getValue());
  glGetFloatv(GL_PROJECTION_MATRIX, &projection[0][0]);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  gluLookAt(lookat[0].getValue(), lookat[1].getValue(), lookat[2].getValue(),
	    lookat[3].getValue(), lookat[4].getValue(), lookat[5].getValue(),
	    lookat[6].getValue(), lookat[7].getValue(), lookat[8].getValue());    
  glGetFloatv(GL_MODELVIEW_MATRIX, &modelView[0][0]);
  glPopMatrix();
}

void keyPressed(unsigned char key, int x, int y){

  switch (key) {
  case 'p':
    mode= PERSPECTIVE;
    break;
  case 'o':
    mode= ORTHO;
    break;
  case 'f':
    mode= FRUSTUM;
    break;
  case 'r':
    perspective[0].setValue(60.0);
    perspective[1].setValue(1.0);
    perspective[2].setValue(1.0);
    perspective[3].setValue(10.0);
    ortho[0].setValue(-1.0);
    ortho[1].setValue(1.0);
    ortho[2].setValue(-1.0);
    ortho[3].setValue(1.0);
    ortho[4].setValue(1.0);
    ortho[5].setValue(3.5);
    frustum[0].setValue(-1.0);
    frustum[1].setValue(1.0);
    frustum[2].setValue(-1.0);
    frustum[3].setValue(1.0);
    frustum[4].setValue(1.0);
    frustum[5].setValue(3.5);
    lookat[0].setValue(0.0);
    lookat[1].setValue(0.0);
    lookat[2].setValue(2.0);
    lookat[3].setValue(0.0);
    lookat[4].setValue(0.0);
    lookat[5].setValue(0.0);
    lookat[6].setValue(0.0);
    lookat[7].setValue(1.0);
    lookat[8].setValue(0.0);
    break;
    // my little cheat insurance
  case 'q':
  case 'Q':
    exit(0);
    break;
  default:
    break;
  }

  setMatrices();
  Context::display();
}

static void drawAxes(void){

  glColor3ub(255, 0, 0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(1.0, 0.0, 0.0);
  glVertex3f(0.75, 0.25, 0.0);
  glVertex3f(0.75, -0.25, 0.0);
  glVertex3f(1.0, 0.0, 0.0);
  glVertex3f(0.75, 0.0, 0.25);
  glVertex3f(0.75, 0.0, -0.25);
  glVertex3f(1.0, 0.0, 0.0);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 1.0, 0.0);
  glVertex3f(0.0, 0.75, 0.25);
  glVertex3f(0.0, 0.75, -0.25);
  glVertex3f(0.0, 1.0, 0.0);
  glVertex3f(0.25, 0.75, 0.0);
  glVertex3f(-0.25, 0.75, 0.0);
  glVertex3f(0.0, 1.0, 0.0);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 1.0);
  glVertex3f(0.25, 0.0, 0.75);
  glVertex3f(-0.25, 0.0, 0.75);
  glVertex3f(0.0, 0.0, 1.0);
  glVertex3f(0.0, 0.25, 0.75);
  glVertex3f(0.0, -0.25, 0.75);
  glVertex3f(0.0, 0.0, 1.0);
  glEnd();
    
  glColor3ub(255, 255, 0);
  Context::setFont("helvetica", 12);
  Context::drawString(1.1, 0.0, 0.0, "x");
  Context::drawString(0.0, 1.1, 0.0, "y");
  Context::drawString(0.0, 0.0, 1.1, "z");
}

// -------------------------------------------------------
// WORLD-SPACE VIEW
// -------------------------------------------------------

bool World::drawModel= true;

void World::reshape(int width, int height){

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat)width/height, 1.0, 256.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);
  glRotatef(-45.0, 0.0, 1.0, 0.0);
  glShadeModel(GL_SMOOTH);
}

void World::display(void){

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHT0);

  vec3 viewDir;
    
  // 'l' is the normalized viewing direction
  viewDir[0]= lookat[3].getValue() - lookat[0].getValue(); 
  viewDir[1]= lookat[4].getValue() - lookat[1].getValue(); 
  viewDir[2]= lookat[5].getValue() - lookat[2].getValue();
  double viewLength= length(viewDir);
  viewDir= normalize(viewDir);

  glClearColor(0.0, 0.0, 0.0, 1.0);    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  // draw current model if toggled
  if(drawModel) {
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, &lightPos[0]);
    model.draw();
    glDisable(GL_LIGHTING);
  }
    
  glPushMatrix();
  // apply inverse modelview transformation to axes and frustum
  // this moves the camera position and frustum into world space
  // coordinates
  glMultMatrixf(&inverse(modelView)[0][0]);
    
  /* draw the axis and eye vector */
  glPushMatrix();
  glColor3ub(0, 0, 255);
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, -1.0 * viewLength);
  glVertex3f(0.1, 0.0, -0.9 * viewLength);
  glVertex3f(-0.1, 0.0, -0.9 * viewLength);
  glVertex3f(0.0, 0.0, -1.0 * viewLength);
  glVertex3f(0.0, 0.1, -0.9 * viewLength);
  glVertex3f(0.0, -0.1, -0.9 * viewLength);
  glVertex3f(0.0, 0.0, -1.0 * viewLength);
  glEnd();
  glColor3ub(255, 255, 0);
  Context::setFont("helvetica", 12);
  Context::drawString(0.0, 0.0, -1.1 * viewLength, "e");
  glColor3ub(255, 0, 0);
  glScalef(0.4, 0.4, 0.4);
  drawAxes();
  glPopMatrix();
    
  // apply inverse projection transformation to unit-frustum
  glMultMatrixf(&inverse(projection)[0][0]);
    
  /* draw the canonical viewing frustum */
  // back clip plane
  glColor3f(0.2, 0.2, 0.2);
  glBegin(GL_QUADS);
  glVertex3i(1, 1, 1);
  glVertex3i(-1, 1, 1);
  glVertex3i(-1, -1, 1);
  glVertex3i(1, -1, 1);
  glEnd();
    
  // four corners of frustum
  glColor3ub(128, 196, 128);
  glBegin(GL_LINES);
  glVertex3i(1, 1, -1);
  glVertex3i(1, 1, 1);
  glVertex3i(-1, 1, -1);
  glVertex3i(-1, 1, 1);
  glVertex3i(-1, -1, -1);
  glVertex3i(-1, -1, 1);
  glVertex3i(1, -1, -1);
  glVertex3i(1, -1, 1);
  glEnd();
    
  // front clip plane
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.2, 0.2, 0.4, 0.5);
  glBegin(GL_QUADS);
  glVertex3i(1, 1, -1);
  glVertex3i(-1, 1, -1);
  glVertex3i(-1, -1, -1);
  glVertex3i(1, -1, -1);
  glEnd();
  glDisable(GL_BLEND);
    
  glPopMatrix();
    
  glutSwapBuffers();
}

char World::menuOptions[]= {'m'};
string World::menuText[]= {"Toggle model"};
int World::numOptions= 1;

void World::menu(int value){

  switch (value) {
  case 'm':
    drawModel= !drawModel;
    break;
  default:
    break;
  }
  display();
}

// -------------------------------------------------------
// SCREEN-SPACE VIEW
// -------------------------------------------------------

// screen_reshape sets the current projection and modelview matrix
// in 'projection[16]' and 'modelview[16]'
void Screen::reshape(int width, int height){

  glViewport(0, 0, width, height);
  glShadeModel(GL_SMOOTH);
}

void Screen::display(void){

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(&projection[0][0]);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(&modelView[0][0]);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glClearColor(0.2, 0.2, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // directional light in positve z-direction
  // must have modelview transform applied to it in order
  // to have correct light position in eye coordinates
  glLightfv(GL_LIGHT0, GL_POSITION, &lightPos[0]);
  model.draw();
  glutSwapBuffers();
}

char Screen::menuOptions[]= {0, 0, 'a', 's', 'd', 'f', 'j', 'p', 'r', };
string Screen::menuText[]= {"Model", "", "Al Capone", "Soccerball", "Dolphins", "Flowers", "F-16", "Porsche", "Rose"};
int Screen::numOptions= 9;

void Screen::menu(int value){

  string name;
    
  switch(value){
  case 'a':
    name = "data/al.obj";
    break;
  case 's':
    name = "data/soccerball.obj";
    break;
  case 'd':
    name = "data/dolphins.obj";
    break;
  case 'f':
    name = "data/flowers.obj";
    break;
  case 'j':
    name = "data/f-16.obj";
    break;
  case 'p':
    name = "data/porsche.obj";
    break;
  case 'r':
    name = "data/rose+vase.obj";
    break;
  default:
    break;
  }
    
  model= OBJModel(name);
  Context::display();
}

// -------------------------------------------------------
// COMMAND WINDOW
// -------------------------------------------------------

void Command::reshape(int width, int height){

  glViewport(0, 0, width, height);

  setMatrices();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, height, 0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Command::display(void){

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  glColor3ub(255,255,255);
    
  Context::setFont("helvetica", 18);
    
  if(mode == PERSPECTIVE){
    Context::drawString(190, perspective[0].y-40, "fovy"); 
    Context::drawString(240, perspective[0].y-40, "aspect"); 
    Context::drawString(310, perspective[0].y-40, "zNear"); 
    Context::drawString(370, perspective[0].y-40, "zFar");
  } 
  else{
    Context::drawString(140, perspective[0].y-40, "left"); 
    Context::drawString(180, perspective[0].y-40, "right"); 
    Context::drawString(240, perspective[0].y-40, "bottom");
    Context::drawString(320, perspective[0].y-40, "top"); 
    Context::drawString(370, perspective[0].y-40, "near");
    Context::drawString(430, perspective[0].y-40, "far");
  }
    
  if(mode == PERSPECTIVE){
    Context::drawString(50, perspective[0].y, "gluPerspective(");
    Context::drawString(230, perspective[0].y, ","); 
    Context::drawString(290, perspective[0].y, ",");
    Context::drawString(350, perspective[0].y, ",");
    Context::drawString(420, perspective[0].y, ");");
  } 
  else if(mode == FRUSTUM){
    Context::drawString(30, frustum[0].y, "glFrustum(");
    Context::drawString(170, frustum[0].y, ",");
    Context::drawString(230, frustum[0].y, ","); 
    Context::drawString(290, frustum[0].y, ",");
    Context::drawString(350, frustum[0].y, ",");
    Context::drawString(410, frustum[0].y, ","); 
    Context::drawString(480, frustum[0].y, ");");
  } 
  else{
    Context::drawString(45, ortho[0].y, "glOrtho(");
    Context::drawString(170, ortho[0].y, ",");
    Context::drawString(230, ortho[0].y, ","); 
    Context::drawString(290, ortho[0].y, ",");
    Context::drawString(350, ortho[0].y, ",");
    Context::drawString(410, ortho[0].y, ","); 
    Context::drawString(480, ortho[0].y, ");");
  }
    
  Context::drawString(88, lookat[0].y, "gluLookAt(");
  Context::drawString(230, lookat[0].y, ","); 
  Context::drawString(290, lookat[0].y, ",");
  Context::drawString(350, lookat[0].y, ",");
  Context::drawString(380, lookat[0].y, "<- eye");
  Context::drawString(230, lookat[3].y, ","); 
  Context::drawString(290, lookat[3].y, ",");
  Context::drawString(350, lookat[3].y, ",");
  Context::drawString(380, lookat[3].y, "<- center");
  Context::drawString(230, lookat[6].y, ","); 
  Context::drawString(290, lookat[6].y, ",");
  Context::drawString(360, lookat[6].y, ");");
  Context::drawString(380, lookat[6].y, "<- up");
    
  if(mode == PERSPECTIVE){
    for(int i= 0; i<4; i++){
      perspective[i].draw();
    }
  }
  else if(mode == FRUSTUM){
    for(int i= 0; i<6; i++){
      frustum[i].draw();
    }
  } 
  else if(mode == ORTHO){
    for(int i= 0; i<6; i++){
      ortho[i].draw();
    }
  }   
    
  for(int i= 0; i<9; i++){
    lookat[i].draw();
  }
    
  glutSwapBuffers();
}

void Command::mousePressed(int button, int state, int x, int y){
 
  if(state == GLUT_DOWN){
    Cell::active= true;  
    if(mode == PERSPECTIVE){
      for(int i= 0; i<4; i++){
	perspective[i].hit(x, y);
      }
    } 
    else if(mode == FRUSTUM){
      for(int i= 0; i<6; i++){
	frustum[i].hit(x, y);
      }
    } 
    else if(mode == ORTHO){

      for(int i= 0; i<6; i++){
	ortho[i].hit(x, y);
      }
    }
    for(int i= 0; i<9; i++){
      lookat[i].hit(x, y);
    }
  }
  else if (state == GLUT_UP) {
    Cell::active= false;  
  }
    
  old_y = y;
    
  display();
}

void Command::mouseMoved(int x, int y){

  for(int i= 0; i<4; i++){
    perspective[i].update(old_y-y);
  }
  for(int i= 0; i<6; i++){
    frustum[i].update(old_y-y);
  }
  for(int i= 0; i<6; i++){
    ortho[i].update(old_y-y);
  }
  for(int i= 0; i<9; i++){
    lookat[i].update(old_y-y);
  }
	
  old_y = y;

  setMatrices();
   
  Context::display();

}


char Command::menuOptions[]= {'p', 'o', 'f', 'r', 'q'};
string Command::menuText[]= {"Perspective", "Ortho", "Frustum", "Reset", "Quit"};
int Command::numOptions= 5;

void Command::menu(int value){
  keyPressed((unsigned char)value, 0, 0);
}
