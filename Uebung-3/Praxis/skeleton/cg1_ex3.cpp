/* ----------------------------------------------------------------
   name:           cg1-ex3.cpp
   purpose:        Sample Application for basic lighting and shading
   version:	   LIBRARY CODE
   TODO:           nothing
   author:         olivier rouiller
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */


// includes, system
#include <iostream>
#ifdef _WIN32
  #include <windows.h>
  #include <cassert>
#endif

// includes, gl
#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

// includes, project
#include "ErrorDevel.h"
#include "Checker.h"
#include "GLSLShader.h"
#include "Mesh.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// defines / flags
// #define DEBUG_READBACK

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>


///////////////////////
// DECLARATIONS
#define M_PI 3.14159265358979323846264338327
static const int TAB_KEY = 9; 
static const int LEFT_MOUSE= 0;
static const int RIGHT_MOUSE  = 2;
static const int MIDDLE_MOUSE = 1;

using namespace glm;

/////////////////////
//! width of the window to render
unsigned int  winWidth = 640;
//! height of the window to render
unsigned int  winHeight = 480;

////////////////////
// Control variables
bool rightButtondown = false;
bool leftButtondown = false;
bool midButtondown = false;
int oldX,oldY;

///////////////////
// View variables
float fovy = 60.0f;
float roty = 0;
float rotx = 0;
float s = 1;
mat4 viewMatrix = glm::lookAt(vec3(0,0,2),vec3(0,0,0),vec3(0,1,0));
mat4 projectionMatrix = glm::perspective(fovy,1.0f, 0.1f, 10.0f);
mat4 objectMatrix = glm::mat4(1.0f);

///////////////////
// Light
vec4 lightDirection(0,0,1,0);
vec4 lightColor(0.7f,0.7f,0.7f,1.0f);

////////////////////////////////////////////////////////////////////////////////////////////////////
// variables, global
GLSLShader blinnPhongShader;
Mesh mesh;


enum RenderMode{
	FLAT,
	GOURAUD,
	BLINN_PHONG,
	MAX_MODES
};

RenderMode currentRenderer = FLAT;

// opengl initializations
void initOpenGLContext(int argc, char **argv);

// mouse callback
void mousePressed(int button, int state, int x, int y);
 
// mouse motion callback
void mouseMoved(int x, int y);  

// window reshape callback
void reshape(int width, int height);


////////////////////////////////////////////////////////////////////////////////////////////////////
//! Init OpenGL
////////////////////////////////////////////////////////////////////////////////////////////////////
void setOpenGLStates(){
	glClearColor( 0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glViewport( 0, 0, winWidth, winHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	projectionMatrix = glm::perspective(fovy,((float)winWidth)/winHeight, 0.1f, 10.0f);
	gluPerspective( fovy, ((float)winWidth)/winHeight, 0.1, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt( 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glColor3f( 1.0, 0.0, 0.0);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);

	// material parameters 
	GLfloat mat_ambient[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat mat_diffuse[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat mat_specular[] = {0.4f, 0.4f, 0.4f, 1};
	GLfloat mat_shininess[] = { 128 };
	// light parameters
	GLfloat light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2 };
	GLfloat light_position[] = { -5.0, 5.0, 5.0, 0.0 };

	// Set fixed pipeline opengl material states
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	
	// Set fixed pipeline opengl light states
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, &lightDirection[0]);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(vec4(0,0,0,1)));
	

	// Enable fixed pipeline lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glutReportErrors();

}


////////////////////////////////////////////////////////////////////////////////////////////////////
//! Display mainloop. 
////////////////////////////////////////////////////////////////////////////////////////////////////
void display(){
	// set OpenGL states
	setOpenGLStates();

	// Clear the color and depth buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the render states for the current rendering technique
	switch(currentRenderer){
		case FLAT:
			// TODO ENABLE PER FACE LIGHTING
		break;
		case GOURAUD:
			// TODO ENABLE PER VERTEX LIGHTING
		break;
		case BLINN_PHONG:
			// ENABLE PER FRAGMENT LIGHTING
			blinnPhongShader.bindShader();
		break;
		break;
		default:
			break;
	}

	glPushMatrix();
	// Rotate the object
	glMultMatrixf(&objectMatrix[0][0]);

	// Render the object 
	// TODO
//	glBegin(GL_TRIANGLES);
//	glVertex3f(1,0,0);
//	glVertex3f(0,1,0);
//	glVertex3f(0,0,1);
//	glEnd();
    mesh.renderFlat();


	// disable program object to avoid side effects
	glUseProgramObjectARB( 0);
	glPopMatrix();

	glFlush();

	glutPostRedisplay();
	glutSwapBuffers();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//! Handle keyboard events
//! @param key ascii code of the key pressed 
////////////////////////////////////////////////////////////////////////////////////////////////////
void 
keyboard(unsigned char key, int /*x*/, int /*y*/) {  

  switch (key) {
	case TAB_KEY:
		currentRenderer = (RenderMode)(((int)currentRenderer+1)%MAX_MODES);
	  break;

	case 's':
		s = 2;
	  break;

    case 27:

      exit(EXIT_SUCCESS);
      break;

    default:

      break;
   }

   glutPostRedisplay();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//! Program main
//! @param argc number of command line arguments
//! @param argv handle to the array containing the command line arguments
////////////////////////////////////////////////////////////////////////////////////////////////////
int
main( int argc, char** argv) {

	initOpenGLContext(argc, argv);

	// set OpenGL states
	setOpenGLStates(); 
 
	// load ressources
	blinnPhongShader.load("shaders/BlinnPhong");
	// TODO LOAD MESH
	mesh.loadOff("meshes/cow.off");
    
	// register glut callbacks
	glutDisplayFunc( display);
	glutKeyboardFunc( keyboard);
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMoved);
	glutReshapeFunc(reshape);

	// let's rock ...
	glutMainLoop();

	return 0;
}  


void initOpenGLContext(int argc, char **argv){
	glutInit( &argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	// some output to console
	cout << "--------------------------------------------\n";
	cout << " cg1_ex3 shading tutorial                   \n";
	cout << "                                            \n";
	cout << " keyboard:                                  \n";
	cout << " q/Q: quit program                          \n";
	cout << " tab: switch to next renderer               \n";
	cout << "                                            \n";
	cout << " mouse:                                     \n";
	cout << " left click+drag: rotate the object         \n";
	cout << " right click+drag: set light direction      \n";
	cout << "--------------------------------------------\n";


	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("cg1_ex3 shading tutorial");

	// get extensions
	// has to be done after basic GL init
	if (GLEW_OK != glewInit()) {
		std::cerr << "Error init GLEW." << std::endl;
		exit( 0);
	}
	if( ! GLEW_ARB_shader_objects) {
		std::cerr << "Your graphics board does not support GLSLang. Exit.";
		exit( EXIT_SUCCESS);
	}
}

void reshape(int width, int height)
{
	winHeight = height;
	winWidth = width;
	glutReshapeWindow(width, height);
	glutPostRedisplay();
}


void mousePressed(int button, int state, int x, int y)
{
	if(button == RIGHT_MOUSE){
		rightButtondown = state == GLUT_DOWN?true:false;
	}else if(button == LEFT_MOUSE){
		leftButtondown = state == GLUT_DOWN?true:false;
	}else if(button == MIDDLE_MOUSE){
		midButtondown = state == GLUT_DOWN?true:false;
	}
	oldX = x;
	oldY = y;
}
 

void mouseMoved(int x, int y)
{
	if(rightButtondown){
		vec3 rayOrigin = vec3(0,0,2);
		vec3 rayDir = glm::unProject(vec3(x,y,1),viewMatrix,projectionMatrix,vec4(0, 0, winWidth, winHeight));
		rayDir = -normalize(rayDir-rayOrigin);
		
		// intersect ray with unit sphere centerd at 0 0 0
		float a = dot(rayDir,rayDir);
		float b = 2*dot(rayDir,rayOrigin);
		float c = dot(rayOrigin,rayOrigin)-1;
		float delta = b*b-4*a*c;
		float t = 0;
		if(delta > 0){
			t =  (-b+sqrt(b*b-4*a*c))/2;
		}else{ // project on the plane passing through the origin and orthogonal to the view vector
			t =  -2/rayDir.z;
		}
		lightDirection = vec4(rayOrigin+t*rayDir,0);
		lightDirection[1] = -lightDirection[1];

	}else if(leftButtondown){
		rotx += 0.05*(y-oldY);
		roty += 0.05*(x-oldX);

		objectMatrix = glm::rotate(rotx,vec3(1,0,0)) * glm::rotate(roty,vec3(0,1,0)) * glm::scale(vec3(s,1,1));;

		// rotate the light
		lightDirection = glm::rotateX(lightDirection,0.05f*(y-oldY));
		lightDirection = glm::rotateY(lightDirection,0.05f*(x-oldX));
	}else if(midButtondown){
		fovy += 0.5*(y-oldY);
		fovy = glm::max(fovy,20.0f);
		fovy = glm::min(fovy,120.0f);
	}

	oldX = x;
	oldY = y;
}
