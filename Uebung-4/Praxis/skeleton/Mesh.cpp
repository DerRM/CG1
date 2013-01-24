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

    computeTexCoords();
    //    computeSupplementalVertices();
    computeVertexNormals();
    computeBoundingSphereCenter();

}

void Mesh::computeSupplementalVertices()
{
    double variation = 0.9;
    int vertices_to_add = 0;

    glDisable(GL_LIGHTING);
    glColor3f(0.1,0.51,0.91);
    glLineWidth(2.);
    glBegin(GL_LINES);

    for (int i = 0; i < m_numFaces; i++) {

        int i0 = m_faces[i].index1;
        int i1 = m_faces[i].index2;
        int i2 = m_faces[i].index3;

        // calculate texture variation on every edge
        //m_texCoords[m_faces[i].index1];

        if(abs(m_texCoords[i0].x - m_texCoords[i1].x) > variation) {
            vertices_to_add++;
            glColor3f(0.1,0.51,0.691);
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index1]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index2]));
        }
        if(abs(m_texCoords[i1].x - m_texCoords[i2].x) > variation) {
            vertices_to_add++;
            glColor3f(0.41,0.251,0.191);
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index2]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index3]));
        }
        if(abs(m_texCoords[i2].x - m_texCoords[i0].x) > variation) {
            vertices_to_add++;
            glColor3f(0.81,0.151,0.91);
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index3]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index1]));
        }
    }
    glEnd();

    // make new vertex and faces arrays
    m_vertices_new = new glm::vec3[m_numVertices+vertices_to_add];
    m_faces_new = new Face[m_numFaces+vertices_to_add];

    // copy existing data into the new arrays
    for (int i = 0; i < m_numVertices; i++) {
        m_vertices_new[i] = m_vertices[i];
        m_faces_new[i] = m_faces[i];
    }

    // we still need to loop over an array of the original size because we haven't added anything yet
    for (int i = 0, j = m_numVertices; i < m_numFaces; i++) {

        int i0 = m_faces[i].index1;
        int i1 = m_faces[i].index2;
        int i2 = m_faces[i].index3;

        glm::vec3 v0 = m_vertices[i0];
        glm::vec3 v1 = m_vertices[i1];
        glm::vec3 v2 = m_vertices[i2];

        if(abs(m_texCoords[i0].x - m_texCoords[i1].x) > variation) {

            glm::vec3 edge0 = v0 - v1;
            float l0 = glm::dot(edge0, edge0); // lenght in vertex coordinates

            double dist1 = 1 - m_texCoords[i0].x; // distance from seam in tex coord
            double dist2 = 1 - m_texCoords[i1].x;
            double l_tex = dist1 + dist2;  // lenght in tex coord

            // calculate a new vertex, to make it easy just in the middle of the edge
            glm::vec3 v_new;
            //            v_new.x = v0.x l0  (l_tex/100 * dist1);
            v_new.x = (v0.x + v1.x)/2.;
            v_new.y = (v0.y + v1.y)/2.;
            v_new.z = (v0.z + v1.z)/2.;

            // add a new entry to the list of vertices
            m_vertices_new[j] = v_new;

            // exchange this vertex in the actual face
            m_faces[i].index2 = j;

            // add a new face with the other vertices to the end of the facelist
            Face newFace;
            newFace.index1 = j;
            newFace.index2 = m_faces[i].index2;
            newFace.index3 = m_faces[i].index3;

            m_faces_new[m_numFaces+i] = newFace;

            j++;
        }
        if(abs(m_texCoords[i1].x - m_texCoords[i2].x) > variation) {
            glm::vec3 edge1 = v1 - v2;
            float l1 = glm::dot(edge1, edge1);
        }
        if(abs(m_texCoords[i2].x - m_texCoords[i0].x) > variation) {
            glm::vec3 edge2 = v2 - v0;
            float l2 = glm::dot(edge2, edge2);
        }

    }

    // reassign new arrays to the original names
    m_vertices = m_vertices_new;
    m_faces = m_faces_new;
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
    glm::vec3 texCoords;

    for (int i = 0; i < m_numVertices; i++)
    {
        /*
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
        */
        // another approch I found somewhere...
        texCoords = glm::vec3(m_vertices[i]);
        m_texCoords[i] = glm::vec2((atan2(texCoords.y, texCoords.x) / 3.1415926 + 1.0) * 0.5,
                                      (asin(texCoords.z) / 3.1415926 + 0.5));

        /*
        // how does this work and what is it for => I dont know
        float u, v;
        if (fixupMethod == EdgeFixup_Stretch) {
            // Transform x,y to [-1, 1] range, match up edges exactly.
            u = float(x) * 2.0f / (edgeLength - 1) - 1.0f;
            v = float(y) * 2.0f / (edgeLength - 1) - 1.0f;
        }
        else {
            // Transform x,y to [-1, 1] range, offset by 0.5 to point to texel center.
            u = (float(x) + 0.5f) * (2.0f / edgeLength) - 1.0f;
            v = (float(y) + 0.5f) * (2.0f / edgeLength) - 1.0f;
        }

        if (fixupMethod == EdgeFixup_Warp) {
            // Warp texel centers in the proximity of the edges.
            float a = powf(float(edgeLength), 2.0f) / powf(float(edgeLength - 1), 3.0f);
            u = a * powf(u, 3) + u;
            v = a * powf(v, 3) + v;
        }
        */
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

// fix_cube_lookup
// (http://www.altdevblogaday.com/2012/03/03/seamless-cube-map-filtering/)
// this would be the second part of something where I dont know how to do the 1st
glm::vec3 fcl(glm::vec3 v) {

    float cube_size = 100.; // need the correct size
    //    v = glm::vec3(m_vertices[m_faces[i].index1]);
    float M = max(max(abs(v.x), abs(v.y)), abs(v.z));
    float scale = (cube_size - 1) / cube_size;
    if (abs(v.x) != M) v.x *= scale;
    if (abs(v.y) != M) v.y *= scale;
    if (abs(v.z) != M) v.z *= scale;

   return v;
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
            //glVertex3fv(glm::value_ptr(fcl(m_vertices[m_faces[i].index1])));
            glTexCoord2d(m_texCoords[m_faces[i].index2].x, m_texCoords[m_faces[i].index2].y);
            glNormal3fv(glm::value_ptr(m_vertexNormals[m_faces[i].index2]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index2]));
            //glVertex3fv(glm::value_ptr(fcl(m_vertices[m_faces[i].index2])));
            glTexCoord2d(m_texCoords[m_faces[i].index3].x, m_texCoords[m_faces[i].index3].y);
            glNormal3fv(glm::value_ptr(m_vertexNormals[m_faces[i].index3]));
            glVertex3fv(glm::value_ptr(m_vertices[m_faces[i].index3]));
            //glVertex3fv(glm::value_ptr(fcl(m_vertices[m_faces[i].index3])));
        }
    }
    glEnd();
}
