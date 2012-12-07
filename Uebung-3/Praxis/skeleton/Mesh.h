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
#include <math.h>

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

typedef struct {
    float x;
    float y;
    float z;
} Vertex;

typedef struct {
    float x;
    float y;
    float z;
} Normal;

typedef struct {
    int index1;
    int index2;
    int index3;
} Face;

class Mesh
{
public:
    Mesh();
    ~Mesh(){};
    void loadOff(const string& filename);
    void renderFlat();
    void renderSmooth();
private:
    Vertex* m_vertices;
    Normal* m_normals;
    Face* m_faces;
    int m_numVertices;
    int m_numFaces;
};

#endif