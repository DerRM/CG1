/* ----------------------------------------------------------------
   name:           scenegraph.h
   purpose:        scenegraph class declaration
   version:	   SKELETON CODE
   TODO:           nothing (see scenegraph.cpp)
   author:         katrin lang
		   computer graphics
		   tu berlin
   ------------------------------------------------------------- */

#pragma once

#include "node.h"

class SceneGraph;

// this is our scenegraph
extern SceneGraph *sceneGraph;

class SceneGraph{

 public:
  
  SceneGraph(Node *root);

  ~SceneGraph();

  // traverse and draw the scenegraph
  void traverse();

  // navigation in tree
  // (needed for node selection)
  void up();
  void down();
  void left();
  void right();

  // increment / decrement rotation of selected node
  void rotate(float x, float y, float z);

  // reset all rotations
  void reset();

 private:

  // root node
  Node *root; 

  // currently selected node
  Node *selected;

  // traverse and draw the scenegraph from a given node
  void traverse(Node *node);

  // reset all rotations below given node
  void reset(Node *node);

  // destroy all nodes
  void clear(Node *node);
};
