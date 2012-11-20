/* ----------------------------------------------------------------
   name:           cell.h
   purpose:        class declaration for cell gui element
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

#pragma once

#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#elif _WIN32
#include "win32/glut.h"
#else
#include <GL/glut.h>
#endif

class Cell {

 public:
     
  // constructor
  Cell(int x, int y, float min, float max, float value, float step, std::string info);
  // destructor
  ~Cell();

  // draw this cell
  void draw();
  // process mouse hit 
  void hit(int x, int y);
  // set value of this cell
  void setValue(int value);
  // get value of this cell
  float getValue();
  // update value of this cell
  void update(int value);

  // x and y position
  int x, y;

  // is mouse pressed ?
  static bool active;

 private:

  // x and y position of info text
  static int width, height;
  // x and y position of info text
  static int infox, infoy;
  // is this cell selected?
  bool selected;
  // minimum and maximum value
  float min, max;
  // initial / current value
  float value;
  // increment
  float step;
  // what does this cell do?
  std::string info;
  // general usage
  static std::string usage;

  // display info text
  void drawInfo();
  // draw value
  void drawValue();
  // se position inside cell?
  bool inside(int x, int y);
};
