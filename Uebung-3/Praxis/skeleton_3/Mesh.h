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

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

typedef struct {
    GLuint index1;
    GLuint index2;
    GLuint index3;
} Face;

class Mesh
{
public:
    Mesh();
    ~Mesh(){};
    void loadOff(const string& filename, GLuint program);
    void renderFlat();
    void renderSmooth();
private:
    glm::vec3* m_vertices;
    glm::vec3* m_surfaceNormals;
    glm::vec3* m_vertexNormals;
    Face* m_faces;
    int m_numVertices;
    int m_numFaces;
    GLuint m_normalHandle;
    GLuint m_positionHandle;
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_nbo;
    
private:
    void computeVertexNormals();
    void createVBO(GLuint program);
};

#endif