//
//  Mesh.h
//  Cg1_ex3
//
//  Created by Christopher Sierigk on 04.12.12.
//
//

#ifndef __Mesh_h__
#define __Mesh_h__

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Ray.h"
#include "Hit.h"
#include "RTObject.h"
#include "KdTree.h"

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

typedef struct {
    int index1;
    int index2;
    int index3;
} Face;

typedef struct {
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float zMin;
    float zMax;
} AABB;

class Mesh : public RTObject
{
public:
    Mesh();
    ~Mesh(){};
    void loadOff(const string& filename);
    void renderFlat();
    void renderSmooth();
    void renderBoundingBox();
    virtual bool hit(Ray& ray, Hit& hit);
    AABB* getBoundingBox();
    void setColor(glm::vec3 color);
private:
    glm::vec3* m_vertices;
    glm::vec3* m_surfaceNormals;
    glm::vec3* m_vertexNormals;
    Face* m_faces;
    int m_numVertices;
    int m_numFaces;
    AABB* m_aabb;
    glm::vec3 m_color;
    
    bool intersectTriangle(Ray& ray, Hit& hit);
    bool intersectBoundingBox(Ray& ray);
    void computeVertexNormals();
    vec3 interpolateNormal(const float beta, const float gamma, Face face);
    void computeAABB();
};

#endif