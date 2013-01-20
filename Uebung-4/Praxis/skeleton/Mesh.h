//
//  Mesh.h
//  Cg1_ex3
//
//  Created by Christopher Sierigk on 04.12.12.
//
//

#ifndef __Mesh_h__
#define __Mesh_h__

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

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
    glm::vec3* m_vertices;
    glm::vec3* m_surfaceNormals;
    glm::vec3* m_vertexNormals;
    glm::vec2* m_texCoords;
    Face* m_faces;
    int m_numVertices;
    int m_numFaces;
    glm::vec3 m_boundingSphereCenter;

    void computeVertexNormals();
    void computeBoundingSphereCenter();
    void computeTexCoords();
};

#endif
