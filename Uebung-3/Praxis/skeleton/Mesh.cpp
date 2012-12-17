//
//  Mesh.cpp
//  Cg1_ex3
//
//  Created by Christopher Sierigk on 04.12.12.
//
//

#include "Mesh.h"

Mesh::Mesh()
{}

void Mesh::loadOff(const string& filename)
{
    ifstream input(filename.c_str());
    
    if (input.fail())
    {
        cerr << "File could not be opened" << "\n";
        exit(EXIT_FAILURE);
    }
    
    string tmp;
    int linenumber = 1;
    
    while(getline(input, tmp))
    {
        istringstream linestream(tmp);
        string word;
        
        while (linestream >> word)
        {
            switch (linenumber) {
                case 1:
                {
                    if (word.compare("OFF") != 0)
                    {
                        cerr << "File is not an OFF-File" << "\n";
                        exit(EXIT_FAILURE);
                    }
                
                    break;
                }
                case 2:
                {
                    m_numVertices = atoi(word.c_str());
                    cout << "Number of Vertices: " << m_numVertices << "\n";
                    m_vertices = new glm::vec3[m_numVertices];
                    linestream >> word;
                    m_numFaces = atoi(word.c_str());
                    cout << "Number of Faces: " << m_numFaces << "\n";
                    m_faces = new Face[m_numFaces];
                    m_surfaceNormals = new glm::vec3[m_numFaces];
                    linestream >> word;
                    int numEdges = atoi(word.c_str());
                    cout << "Number of Edges: " << numEdges << "\n";
                    break;
                }
                default:
                {
                    if (linenumber > 2 && linenumber <= m_numVertices + 2)
                    {
                        glm::vec3 vertex;
                        vertex.x = atof(word.c_str());
                        linestream >> word;
                        vertex.y = atof(word.c_str());
                        linestream >> word;
                        vertex.z = atof(word.c_str());
                        m_vertices[linenumber - 2 - 1] = vertex;
                    }

                    if (linenumber > m_numVertices + 2 && linenumber <= (m_numFaces + m_numVertices + 2))
                    {
                        Face face;
                        linestream >> word;
                        face.index1 = atoi(word.c_str());
                        linestream >> word;
                        face.index2 = atoi(word.c_str());
                        linestream >> word;
                        face.index3 = atoi(word.c_str());
                        m_faces[linenumber - m_numVertices - 2 - 1] = face;
                        
                        glm::vec3 v1 = m_vertices[face.index1];
                        glm::vec3 v2 = m_vertices[face.index2];
                        glm::vec3 v3 = m_vertices[face.index3];
                        
                        glm::vec3 normal = glm::normalize(glm::cross(v3 - v2, v1 - v2));
                        m_surfaceNormals[linenumber - m_numVertices - 2 - 1] = normal;
                    }

                    break;
                }
            }
        }
        
        linenumber++;
    }
    
    computeVertexNormals();
}

void Mesh::computeVertexNormals()
{
    m_vertexNormals = new glm::vec3[m_numVertices];
    
    for (int i = 0; i < m_numFaces; i++)
    {
        m_vertexNormals[m_faces[i].index1] += m_surfaceNormals[i];
        m_vertexNormals[m_faces[i].index2] += m_surfaceNormals[i];
        m_vertexNormals[m_faces[i].index3] += m_surfaceNormals[i];
    }
    
    for (int i = 0; i < m_numVertices; i++)
    {
        m_vertexNormals[i] = glm::normalize(m_vertexNormals[i]);
    }
}

void Mesh::renderFlat()
{
    glBegin(GL_TRIANGLES);
    {
        for (int i = 0; i < m_numFaces; i++)
        {
            glNormal3fv(glm::value_ptr(m_surfaceNormals[i]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index1]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index2]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index3]));
        }
    }
    glEnd();
}

void Mesh::renderSmooth()
{
    glBegin(GL_TRIANGLES);
    {
        for (int i = 0; i < m_numFaces; i++)
        {
            glNormal3fv(glm::value_ptr(m_vertexNormals[m_faces[i].index1]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index1]));
            glNormal3fv(glm::value_ptr(m_vertexNormals[m_faces[i].index2]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index2]));
            glNormal3fv(glm::value_ptr(m_vertexNormals[m_faces[i].index3]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index3]));
        }
    }
    glEnd();    
}
