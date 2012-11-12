/* ----------------------------------------------------------------
   name:           robot.c
   purpose:        implementation of the scenegraph construction
   version:	   SKELETON CODE
   TODO:           buildRobot
   author:         katrin lang
		   computer graphics
		   tu berlin
   ------------------------------------------------------------- */

#include "robot.h"

// nodes of the scenegraph

// XXX: CURRENTLY ONLY TORSO. INSERT OTHER BODY PARTS HERE.
//      SEE EXAMPLE BINARY cg1_ex1.exe (win32) and cg1_ex1 (linux).
//      EXPERIMENTATION IS ENCOURAGED. THE FINAL MODEL/SCENEGRAPH
//      NEED NOT BE IDENTICAL TO THE PROPOSED SOLUTION.

// XXX: EXTEND THE SCENEGRAPH i.e.
// Node *part1, *part2, *part3, etc...

// build the robot scenegraph
Node *buildRobot(){
  
  // torso attributes and initial position.
  // for parameters of constructor and their meanings,
  // see file node.h
  Node *torso= new Node(0, 0, 0,
			200, 300, 100,
			0, 0, 0,
			0, 0, 0);
  
  // XXX: implement more body/robot/whatever parts here
  
  // INSERT YOUR CODE HERE
    Node *head = new Node(0, 250, 0,
                          100, 100, 100,
                          0, 75, 0,
                          0, 0, 0);
    
    Node *leftArm = new Node(175, 50, 0,
                             50, 200, 100,
                             50, -75, 0,
                             0, 0, 0);
    
    Node *rightArm = new Node(-175, 50, 0,
                              50, 200, 100,
                              -50, -75, 0,
                              0, 0, 0);
    
    Node *leftLeg = new Node(-63, -300, 0,
                             75, 200, 100,
                             0, -125, 0,
                             0, 0, 0);
    
    Node *rightLeg = new Node(63, -300, 0,
                              75, 200, 100,
                              0, -125, 0,
                              0, 0, 0);
    
    Node *hat = new Node(0, 125, 0,
                         200, 50, 100,
                         0, 50, 0,
                         0, 0, 0);
  // END XXX
  
  // finally, attach the robot/model parts
  // example: leftArm->setParent(torso), etc...
  // BUILD THE SCENEGRAPH
  
  // XXX: attach body parts here

  // INSERT YOUR CODE HERE
  head->setParent(torso);
  leftArm->setParent(torso);
  rightArm->setParent(torso);
  leftLeg->setParent(torso);
  rightLeg->setParent(torso);
  hat->setParent(head);
  
  // END XXX
  
  // return root node
  return torso;
}
