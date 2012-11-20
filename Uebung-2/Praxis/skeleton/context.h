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

namespace Context{
  
  // intialization                                             
  void init(int argc, char **argv);  
  
  // redisplay all windows
  void display(void);

  // select glut bitmap font
  void setFont(std::string name, int size);

  // draw a string on the screen
  void drawString(int x, int y, std::string s);
  void drawString(int x, int y, int z, std::string s);
};
