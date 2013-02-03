//
//  KdTree.cpp
//  Cg1_ex5
//
//  Created by Christopher Sierigk on 02.02.13.
//
//

#include "KdTree.h"

KdTree::KdTree()
{
    
}

KdTree::KdTree(glm::vec3* vertexList, int listLength)
:m_vertices(vertexList), m_listLength(listLength)
{
    m_root = makeKdTree(m_vertices, m_listLength);
}

Node* KdTree::getRootNode()
{
    return m_root;
}

Node* KdTree::makeKdTree(glm::vec3* vertexList, int listLength, int depth)
{
    if (listLength == 0)
    {
        return NULL;
    }
    
    int k = 3;
    int axis = depth % k;
    
    if (axis == 0)
    {
        std::sort(vertexList, vertexList + listLength, sortX);
       
        for (int i = 0; i < listLength; i++)
        {
            std::cout << "x: " << vertexList[i].x << "\n";
        }

    }
    
    if (axis == 1)
    {
        std::sort(vertexList, vertexList + listLength, sortY);
        
        for (int i = 0; i < listLength; i++)
        {
            std::cout << "y: " << vertexList[i].y << "\n";
        }
    }
    
    if (axis == 2)
    {
        std::sort(vertexList, vertexList + listLength, sortZ);
        
        for (int i = 0; i < listLength; i++)
        {
            std::cout << "z: " << vertexList[i].z << "\n";
        }
    }
    
    int median = listLength / 2;
    glm::vec3* leftList = splitList(vertexList, 0, median);
    glm::vec3* rightList = splitList(vertexList, median + 1, listLength);
    
    Node* node = new Node();
    
    node->median = median;
    node->leftChild = makeKdTree(leftList, median, depth + 1);
    node->rightChild = makeKdTree(rightList, listLength - (median + 1), depth + 1);
    return node;
}

bool KdTree::sortX(const glm::vec3 &vecA, const glm::vec3 &vecB)
{
    return vecA.x < vecB.x;
}

bool KdTree::sortY(const glm::vec3 &vecA, const glm::vec3 &vecB)
{
    return vecA.y < vecB.y;
}

bool KdTree::sortZ(const glm::vec3 &vecA, const glm::vec3 &vecB)
{
    return vecA.z < vecB.z;
}

glm::vec3* KdTree::splitList(glm::vec3 *vertexList, int start, int end)
{
    int length = end - start;
    glm::vec3* newList = new glm::vec3[length];
    
    for (int i = 0; i < length; i++)
    {
        newList[i] = vertexList[start + i];
    }
    
    return newList;
}

