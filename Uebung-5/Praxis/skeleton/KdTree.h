//
//  KdTree.h
//  Cg1_ex5
//
//  Created by Christopher Sierigk on 02.02.13.
//
//

#ifndef __Cg1_ex5__KdTree__
#define __Cg1_ex5__KdTree__

#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>

typedef struct {
    
} Node;

class KdTree {
public:
    KdTree();
    ~KdTree(){};
    void makeKdTree(glm::vec3* vertexList, int listLength, int depth = 0);
private:
    static bool sortX(const glm::vec3 &vecA, const glm::vec3 &vecB);
    static bool sortY(const glm::vec3 &vecA, const glm::vec3 &vecB);
    static bool sortZ(const glm::vec3 &vecA, const glm::vec3 &vecB);
    glm::vec3* splitList(glm::vec3* vertexList, int start, int end);
};


#endif /* defined(__Cg1_ex5__KdTree__) */
