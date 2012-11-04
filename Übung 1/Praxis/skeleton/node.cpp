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

	// END XXX

	// translate node center to joint position
	// XXX

	// INSERT YOUR CODE HERE

	// END XXX

	// apply this node's rotation
	// XXX

        // INSERT YOUR CODE HERE

	// END XXX

	// draw Joint (rotation center)
	drawJoint();

	// translate center of rotation into limb's origin
	// XXX

	// INSERT YOUR CODE HERE

	// END XXX
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

	// END XXX

	glBegin(GL_LINES);

	// XXX: DRAW X,Y AND Z AXES IN RED,GREEN AND BLUE
	//      SEE PROVIDED cg1_ex1.exe (win32) AND cg1_ex1 (linux)

	// INSERT YOUR CODE HERE

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

