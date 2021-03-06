/* ----------------------------------------------------------------
   name:           node.cpp
   purpose:        implementation for a scenegraph node
   version:	   SKELETON CODE
   TODO:           transform, drawJoint
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

#include <iostream> 
#include "node.h"

// without this line, 
// printline debugging won't work
using namespace std;

 // constructor
  Node::Node(float x, float y, float z, 
       float length, float height, float width,
       float jointx, float jointy, float jointz,
       float rotx, float roty, float rotz){

    this->selected= false;

    this->x= x;
    this->y= y;
    this->z= z;
    
    this->length= length;
    this->height= height;
    this->width= width;
    
    this->jointx= jointx;
    this->jointy= jointy;
    this->jointz= jointz;

    this->rotx= rotx;
    this->roty= roty;
    this->rotz= rotz;

    this->parent= NULL;
    this->child= NULL;
    this->previous= NULL;
    this->next= NULL;
  }

// destructor
// nothing to do so far
Node::~Node(){}

// sets the parent of this node
// and adds this node 
// to the list of the parent's children
void Node::setParent(Node* parent){
  
  this->parent= parent;
  if(parent->child==NULL){
    parent->child= this;
  }
  else{
    Node *sibling= parent->child;
    while(sibling->next != NULL) sibling= sibling->next;
    sibling->next= this;
    this->previous= sibling;
  }
}

  // transform an individual node
  // according to its position, 
  // rotation, and rotation center
  // XXX: NEEDS TO BE IMPLEMENTED
void Node::transform(){
 
	// note the order of the operations:
	// the transformations are applied in "reverse" order
	// of glRotate/glTranslate calls
	// (also see cg1 lecture notes on this topic)

	// translate to final position
	// XXX

	// INSERT YOUR CODE HERE
    glTranslatef(x, y, z);
	// END XXX

	// translate node center to joint position
	// XXX

	// INSERT YOUR CODE HERE
    glTranslatef(-jointx, -jointy, -jointz);
	// END XXX

	// apply this node's rotation
	// XXX

        // INSERT YOUR CODE HERE
    glRotatef(rotx, 1.0f, 0.0f, 0.0f);
    glRotatef(roty, 0.0f, 1.0f, 0.0f);
    glRotatef(rotz, 0.0f, 0.0f, 1.0f);
	// END XXX

	// draw Joint (rotation center)
	drawJoint();

	// translate center of rotation into limb's origin
	// XXX
	// INSERT YOUR CODE HERE
    glTranslatef(jointx, jointy, jointz);
	// END XXX
    
    glGetDoublev(GL_MODELVIEW_MATRIX, mv);
}

// draw an individual node
void Node::draw(){

	// save lighting bit for red light
	glPushAttrib(GL_LIGHTING_BIT);

	GLfloat red[]= {1.0, 0, 0};
	if(selected) glLightModelfv(GL_LIGHT_MODEL_AMBIENT, red);

	glPushMatrix();

	glScalef(length, height, width);
	glutSolidCube(1.0);

	glPopMatrix();

	glPopAttrib();
}

// draw the joint (rotation center)
// of an individual node
// XXX: NEEDS TO BE IMPLEMENTED
void Node::drawJoint(){

	// save enable bit for lighting
	// and current bit for color
	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

	// draw coordinate axes at center of rotation
	// note: lighting disabled for axis drawing
	glDisable(GL_LIGHTING);

	// draw a sphere at center of rotation
	// (with glutwiredphere...)
	// XXX
    
	// INSERT YOUR CODE HERE
    glutWireSphere(25, 10, 10);
	// END XXX

	glBegin(GL_LINES);

	// XXX: DRAW X,Y AND Z AXES IN RED,GREEN AND BLUE
	//      SEE PROVIDED cg1_ex1.exe (win32) AND cg1_ex1 (linux)

	// INSERT YOUR CODE HERE
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0 + 50, 0, 0);
    
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0 + 50, 0);
    
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 0 + 50);
	// END XXX

	glEnd();
	glPopAttrib();
}

// increment / decrement rotation
void Node::rotate(float x, float y, float z){
  
  rotx+= x;
  roty+= y;
  rotz+= z;
}

void Node::hitTest(float rayX, float rayY, float rayZ)
{
    //cout << "Node x: " << x << " z: " << y << " z: " << z << "\n";
    
    cout << mv[0] << " " << mv[4] << " " << mv[8] << " " << mv[12] << "\n";
    cout << mv[1] << " " << mv[5] << " " << mv[9] << " " << mv[13] << "\n";
    cout << mv[2] << " " << mv[6] << " " << mv[10] << " " << mv[14] << "\n";
    cout << mv[3] << " " << mv[7] << " " << mv[11] << " " << mv[15] << "\n";
}

Node *Node::getNext(){
  return next;
}

Node *Node::getPrevious(){
  return previous;
}

Node *Node::getParent(){
  return parent;
}

Node *Node::getChild(){
  return child;
}

void Node::select(){
  selected= true;
}

void Node::deselect(){
  selected= false;
}

