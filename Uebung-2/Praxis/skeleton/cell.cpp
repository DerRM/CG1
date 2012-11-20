/* ----------------------------------------------------------------
   name:           cell.cpp
   purpose:        implementation for cell gui element
                   based on tutorial by Nate Robins, 1997
                   modified for clip-space viewing in the context 
                   of 'introduction to computer graphics' 
                   winter term 2012/2013, assignment 2
   version:	   LIBRARY CODE
   TODO:           nothing
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include <sstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#elif _WIN32
#include "win32/glut.h"
#else
#include <GL/glut.h>
#endif


#include "cell.h"
#include "context.h"

using namespace std;

  // constructor
Cell::Cell(int x, int y, float min, float max, float v, float step, string info) : 
  x(x), y(y), min(min), max(max), value(v), step(step), info(info), selected(false) 
{}

  // destructor
Cell::~Cell(){}

// set value of this cell
void Cell::setValue(int value){

    if (value < min) value= min;
    else if (value > max) value= max;
    
    this-> value= value;
}

  // get value of this cell
float Cell::getValue(){
  return value;
}

void Cell::draw(){
    
    glColor3ub(255, 255, 0);
    if(selected || !active) drawInfo();
    if(selected && active) glColor3ub(255, 0, 0);	 
    else glColor3ub(0, 255, 128);
    drawValue();
}

int Cell::infox= 10;
int Cell::infoy= 240;

string Cell::usage= "Click on the arguments and move the mouse to modify values.";

bool Cell::active= false;

void Cell::drawInfo(){
    
  string s= active? info : usage;
  Context::drawString(infox, infoy, s);
}

void Cell::drawValue(){

  stringstream ss (stringstream::in | stringstream::out);
 
  if(value/100>=1) ss.precision(0);
  else if(value/10>=1) ss.precision(1);
  else ss.precision(2);
 ss.width(6);
  ss.setf(stringstream::fixed);
  if(value <0) ss.setf(stringstream::left);
  else ss.setf(stringstream::right);
  ss << value;

  Context::drawString(x, y, ss.str());
}

int Cell::width= 60;
int Cell::height= 40;

bool Cell::inside(int x, int y){

    if (x > this->x && x < this->x + width &&
        y > this->y-0.66*height && y < this->y+0.33*height)
        return true;
    return false;
}

void Cell::hit(int x, int y){
  if(inside(x,y)) selected= true;
  else selected= false;
}

void Cell::update(int update){

    if(!selected) return;
    
    value += update * step;
    
    if (value < min) value= min;
    else if (value > max) value= max;
}
