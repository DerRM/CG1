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

typedef struct Node {
    Node* leftChild;
    Node* rightChild;
    int median;
} Node;

class KdTree {
public:
    KdTree();
    KdTree(glm::vec3* vertexList, int listLength);
    ~KdTree(){};
    Node* getRootNode();
private:
    static bool sortX(const glm::vec3 &vecA, const glm::vec3 &vecB);
    static bool sortY(const glm::vec3 &vecA, const glm::vec3 &vecB);
    static bool sortZ(const glm::vec3 &vecA, const glm::vec3 &vecB);
    Node* makeKdTree(glm::vec3* vertexList, int listLength, int depth = 0);
    glm::vec3* splitList(glm::vec3* vertexList, int start, int end);
    glm::vec3* m_vertices;
    int m_listLength;
    Node* m_root;
};


#endif /* defined(__Cg1_ex5__KdTree__) */
