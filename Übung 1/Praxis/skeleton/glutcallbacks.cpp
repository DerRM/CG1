/* ----------------------------------------------------------------
   name:           glutcallbacks.c
   purpose:        init, display and everything else needed by GLUT
   version:		   SKELETON CODE
   author:         andy nealen
				   computer graphics
				   tu berlin
   ------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__ 
#include <GLUT/glut.h>
#elif _WIN32
#include "win32/glut.h"
#else
#include <GL/glut.h>
#endif

#include "glutcallbacks.h"
#include "robot.h"

// some stateholders for mouse motion
// last mouse position in window
int posx = 0, posy = 0;
// is left mouse button pressed
int lbutton = 0;

// pointer to currently selected node for transformations
// start with root node (torso)
Node* selected = &torso;

/* ------------------------------------------------ 
    implemented glutCallbacks:
    init        - initialize OpenGL
    display     - display scene
    reshape     - reshape window
    keyboard    - keyboard callbacks
    mouseMenu   - right mouse button menu
    mouseMotion - apply rotation when dragging
	mouse       - register mouse clicks
	registerCallbacks - 
	              register all callbacks with GLUT
   ------------------------------------------------ */

void init(void)
{
    // light and material
    GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat mat_specular[] = {0.6, 0.6, 0.6, 1.0};
    GLfloat mat_shininess[] = { 3.0 };
    GLfloat model_ambient[] = { 0.3, 0.3, 0.3 };
    GLfloat light_position[] = { 5.0, 5.0, 5.0, 0.0 };

    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("CG_1 Mini-Scenegraph Robot");

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // shading
    glShadeModel(GL_FLAT);

    // clear background to black and clear depth buffer
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
	// enable depth test (z-buffer)
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// enable normalization of vertex normals
	glEnable(GL_NORMALIZE);

    // initial view definitions
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	// perspective projection
    gluPerspective(40.0, 1.0, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// some output to console
	printf("--------------------------------------------\n");
	printf(" cg1_ex1 opengl robot scenegraph            \n");
	printf("                                            \n");
	printf(" keyboard:                                  \n");
    printf(" arrow keys: select node                          \n");
    printf(" x/X,y/Y,z/Z: rotate node                   \n");
    printf(" q/Q: quit program                          \n");
	printf("                                            \n");
	printf(" mouse:                                     \n");
    printf(" right click: config menu              \n");
    printf(" left click+drag: rotate node              \n");
    printf("--------------------------------------------\n");
}

// display callback for GLUT
void display(void)
{
	// clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// switch to opengl modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	// position the camera at (0,0,3) looking down the
	// negative z-axis at (0,0,0)
    gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	// draw the scenegraph
    sceneGraphTraversal(&torso);

	// display back buffer
    glutSwapBuffers();
}

// reshape-Callback for GLUT
void reshape(int w, int h)
{
	// reshaped window aspect ratio
	float aspect = (float) w / (float) h;

    // viewport
    glViewport(0,0, (GLsizei) w, (GLsizei) h);

	// clear background and depth buffer
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // restore view definition after window reshape
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	// perspective projection
    gluPerspective(40.0, aspect, 1.0, 10.0);

	display();
}

// keyboard callback
void keyboard(unsigned char key, int x, int y)
{
	// rotate selected node around 
	// x,y and z axes with keypresses
    switch( key ) {
	case 'q':
	case 'Q': exit(0);
	case 'z':
		selected->rz += 2.0;
		display();
		break;
	case 'Z': 
		selected->rz -= 2.0;
		display();
		break;
	case 'x':
		selected->rx += 2.0;
		display();
		break;
	case 'X': 
		selected->rx -= 2.0;
		display();
		break;
	case 'y':
		selected->ry += 2.0;
		display();
		break;
	case 'Y': 
		selected->ry -= 2.0;
		display();
		break;
	default:
		break;
	}
}

// the right button mouse menu
void mouseMenu(int id)
{
    switch (id) {
		case 1: 
		  delete sceneGraph;
		  exit(0);
		case 2: 
			// select torso for transformations
			selected = &torso;
			break;

		// XXX: allow selecting/transforming 
		//      other body parts as well

		// INSERT YOUR CODE HERE

		// END XXX

		default:
			break;
	}
}

// mouse motion
void mouseMotion(int x, int y)
{
	// rotate selected node when left mouse button is pressed
	if (lbutton) {
		selected->rx += (float) (y-posy);
		posy = y;
		selected->ry += (float) (x-posx);
		posx = x;
		display();
	}
}

// mouse callback
void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT) {
		if (state == GLUT_UP) {
			lbutton = 0;
		}
		if (state == GLUT_DOWN) {
			lbutton = 1;
			posx = x; posy = y;
		}
	}
}

// register callbacks with GLUT
void registerCallbacks(void)
{
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouse);
    glutCreateMenu(mouseMenu);
       glutAddMenuEntry("quit",1);
       glutAddMenuEntry("torso",2);
	   // XXX: allow selecting/transforming 
	   //      other body parts as well
	   
	   // INSERT YOUR CODE HERE
	   
	   // END XXX
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    return;
}
