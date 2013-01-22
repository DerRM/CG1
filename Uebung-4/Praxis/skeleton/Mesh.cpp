//
//  Mesh.cpp
//  Cg1_ex3
//
//  Created by Christopher Sierigk on 04.12.12.
//
//


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    computeBoundingSphereCenter();
    computeTexCoords();
}

void Mesh::computeVertexNormals()
{
    m_vertexNormals = new glm::vec3[m_numVertices];

    for (int i = 0; i < m_numFaces; i++)
    {
        glm::vec3 v0 = m_vertices[m_faces[i].index1];
        glm::vec3 v1 = m_vertices[m_faces[i].index2];
        glm::vec3 v2 = m_vertices[m_faces[i].index3];

        glm::vec3 edge0 = v0 - v1;
        glm::vec3 edge1 = v1 - v2;
        glm::vec3 edge2 = v2 - v0;

        float l0 = glm::dot(edge0, edge0);
        float l1 = glm::dot(edge1, edge1);
        float l2 = glm::dot(edge2, edge2);

        float f0 = 1 / l0 * l2;
        float f1 = 1 / l1 * l0;
        float f2 = 1 / l2 * l1;

        m_vertexNormals[m_faces[i].index1] += m_surfaceNormals[i] * f0;
        m_vertexNormals[m_faces[i].index2] += m_surfaceNormals[i] * f1;
        m_vertexNormals[m_faces[i].index3] += m_surfaceNormals[i] * f2;
    }

    for (int i = 0; i < m_numVertices; i++)
    {
        m_vertexNormals[i] = glm::normalize(m_vertexNormals[i]);
    }
}

// sum of all vertices and the average
void Mesh::computeBoundingSphereCenter()
{
    m_boundingSphereCenter = glm::vec3();

    for (int i = 0; i < m_numVertices; i++)
    {
        m_boundingSphereCenter += m_vertices[i];
    }

    m_boundingSphereCenter /= m_numVertices;
}

void Mesh::computeTexCoords()
{
    m_texCoords = new glm::vec2[m_numVertices];

    for (int i = 0; i < m_numVertices; i++)
    {
        double x_minus_x_s = m_vertices[i].x - m_boundingSphereCenter.x;
        double y_minus_y_s = m_vertices[i].y - m_boundingSphereCenter.y;
        double z_minus_z_s = m_vertices[i].z - m_boundingSphereCenter.z;
        glm::vec2 uv_coord = m_texCoords[i];

        uv_coord.x = (M_PI + atan2(y_minus_y_s, x_minus_x_s)) / (2 * M_PI);
        uv_coord.y = atan2(sqrt(pow(x_minus_x_s, 2.0) + pow(y_minus_y_s, 2.0)), z_minus_z_s) / M_PI;

        // flip coordinates
        //uv_coord.x = 1.0 - uv_coord.x;
        uv_coord.y = 1.0 - uv_coord.y;
        m_texCoords[i] = uv_coord;
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
            glTexCoord2d(m_texCoords[m_faces[i].index1].x, m_texCoords[m_faces[i].index1].y);
            glNormal3fv(glm::value_ptr(m_vertexNormals[m_faces[i].index1]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index1]));
            glTexCoord2d(m_texCoords[m_faces[i].index2].x, m_texCoords[m_faces[i].index2].y);
            glNormal3fv(glm::value_ptr(m_vertexNormals[m_faces[i].index2]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index2]));
            glTexCoord2d(m_texCoords[m_faces[i].index3].x, m_texCoords[m_faces[i].index3].y);
            glNormal3fv(glm::value_ptr(m_vertexNormals[m_faces[i].index3]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index3]));
        }
    }
    glEnd();
}
