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
                    m_vertices = new Vertex[m_numVertices];
                    linestream >> word;
                    m_numFaces = atoi(word.c_str());
                    cout << "Number of Faces: " << m_numFaces << "\n";
                    m_faces = new Face[m_numFaces];
                    m_normals = new Normal[m_numFaces];
                    linestream >> word;
                    int numEdges = atoi(word.c_str());
                    cout << "Number of Edges: " << numEdges << "\n";
                    break;
                }
                default:
                {
                    if (linenumber > 2 && linenumber <= m_numVertices + 2)
                    {
                        Vertex vertex;
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
                        
                        Vertex v1 = m_vertices[face.index1];
                        Vertex v2 = m_vertices[face.index2];
                        Vertex v3 = m_vertices[face.index3];
                        
                        Vertex dir1;
                        dir1.x = v1.x - v2.x;
                        dir1.y = v1.y - v2.y;
                        dir1.z = v1.z - v2.z;
                        
                        Vertex dir2;
                        dir2.x = v3.x - v2.x;
                        dir2.y = v3.y - v2.y;
                        dir2.z = v3.z - v2.z;
                        
                        Normal normal;
                        normal.x = dir1.y * dir2.z - dir1.z * dir2.y;
                        normal.y = dir1.z * dir2.x - dir1.x * dir2.z;
                        normal.z = dir1.x * dir2.y - dir1.y * dir2.x;
                        
                        // normalize;
                        float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
                        normal.x /= length;
                        normal.y /= length;
                        normal.z /= length;
                        m_normals[linenumber - m_numVertices - 2 - 1] = normal;
                    }

                    break;
                }
            }
        }
        
        linenumber++;
    }
}

void Mesh::renderFlat()
{
    glBegin(GL_TRIANGLES);
    {
        for (int i = 0; i < m_numFaces; i++)
        {
            glNormal3f(m_normals[i].x, m_normals[i].y, m_normals[i].z);
            glVertex3f(m_vertices[m_faces[i].index1].x, m_vertices[m_faces[i].index1].y, m_vertices[m_faces[i].index1].z);
            glVertex3f(m_vertices[m_faces[i].index2].x, m_vertices[m_faces[i].index2].y, m_vertices[m_faces[i].index2].z);
            glVertex3f(m_vertices[m_faces[i].index3].x, m_vertices[m_faces[i].index3].y, m_vertices[m_faces[i].index3].z);
        }
    }
    glEnd();
}

void Mesh::renderSmooth()
{
    
}
