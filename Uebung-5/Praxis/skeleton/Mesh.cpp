//
//  Mesh.cpp
//  Cg1_ex3
//
//  Created by Christopher Sierigk on 04.12.12.
//
//

#include "Mesh.h"

Mesh::Mesh()
{
}

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
    
    //KdTree tree(m_vertices, m_numVertices);
    //Node* root = tree.getRootNode();
    //tree.makeKdTree(m_vertices, m_numVertices);
    
    computeVertexNormals();
    computeAABB();
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

void Mesh::computeAABB()
{
    m_aabb = new AABB();
    
    m_aabb->xMax = FLT_MIN;
    m_aabb->yMax = FLT_MIN;
    m_aabb->zMax = FLT_MIN;
    m_aabb->xMin = FLT_MAX;
    m_aabb->yMin = FLT_MAX;
    m_aabb->zMin = FLT_MAX;
    
    for (int i = 0; i < m_numVertices; i++)
    {
        vec3 vec = m_vertices[i];
        m_aabb->xMax = std::max(vec.x, m_aabb->xMax);
        m_aabb->yMax = std::max(vec.y, m_aabb->yMax);
        m_aabb->zMax = std::max(vec.z, m_aabb->zMax);
        m_aabb->xMin = std::min(vec.x, m_aabb->xMin);
        m_aabb->yMin = std::min(vec.y, m_aabb->yMin);
        m_aabb->zMin = std::min(vec.z, m_aabb->zMin);
    }
}

AABB* Mesh::getBoundingBox()
{
    return m_aabb;
}

bool Mesh::intersectBoundingBox(Ray& ray)
{
    double ox = ray.o.x; double oy = ray.o.y; double oz = ray.o.z;
    double dx = ray.d.x; double dy = ray.d.y; double dz = ray.d.z;
    
    vec3 min = vec3(m_aabb->xMin, m_aabb->yMin, m_aabb->zMin);
    vec3 max = vec3(m_aabb->xMax, m_aabb->yMax, m_aabb->zMax);
    
    double txMin, tyMin, tzMin;
    double txMax, tyMax, tzMax;
    
    double a = 1.0 / dx;
    
    if (a >= 0.0)
    {
        txMin = (min.x - ox) * a;
        txMax = (max.x - ox) * a;
    }
    else
    {
        txMin = (max.x - ox) * a;
        txMax = (min.x - ox) * a;
    }
    
    double b = 1.0 / dy;
    
    if (b >= 0.0)
    {
        tyMin = (min.y - oy) * b;
        tyMax = (max.y - oy) * b;
    }
    else
    {
        tyMin = (max.y - oy) * b;
        tyMax = (min.y - oy) * b;
    }

    double c = 1.0 / dz;
    
    if (c >= 0.0)
    {
        tzMin = (min.z - oz) * c;
        tzMax = (max.z - oz) * c;
    }
    else
    {
        tzMin = (max.z - oz) * c;
        tzMax = (min.z - oz) * c;
    }
    
    double t0, t1;
    
    if (txMin > tyMin)
    {
        t0 = txMin;
    }
    else
    {
        t0 = tyMin;
    }
    
    if (tzMin > t0)
    {
        t0 = tzMin;
    }
    
    if (txMax < tyMax)
    {
        t1 = txMax;
    }
    else
    {
        t1 = tyMax;
    }

    if (tzMax < t1)
    {
        t1 = tzMax;
    }
        
    return (t0 < t1 && t1 > ray.tmin);
}

void Mesh::setColor(glm::vec3 color)
{
    m_color = color;
}

bool Mesh::hit(Ray &ray, Hit &hit)
{
    return intersectTriangle(ray, hit);
}

bool Mesh::intersectTriangle(Ray& ray, Hit& hit)
{
    
    if (!intersectBoundingBox(ray))
    {
        return false;
    }
    
    bool result = false;
    double distance = DBL_MAX;
    for (int i = 0; i < m_numFaces; i++)
    {
        vec3 v1 = m_vertices[m_faces[i].index1];
        vec3 v2 = m_vertices[m_faces[i].index2];
        vec3 v3 = m_vertices[m_faces[i].index3];
        
        vec3 v1v2 = v1 - v2;
        vec3 v1v3 = v1 - v3;
        vec3 rayDir = ray.d;
        vec3 v1rayOr = v1 - ray.o;
        
        double m = v1v3.y * rayDir.z - rayDir.y * v1v3.z;
        double n = v1rayOr.y * rayDir.z - rayDir.y * v1rayOr.z;
        double p = v1v3.y * v1rayOr.z - v1rayOr.y * v1v3.z;
        double q = rayDir.y * v1v2.z - v1v2.y * rayDir.z;
        double r = v1v2.y * v1rayOr.z - v1rayOr.y * v1v2.z;
        double s = v1v2.y * v1v3.z - v1v3.y * v1v2.z;
        
        double inv_denom = 1.0 / (v1v2.x * m + v1v3.x * q + rayDir.x * s);
        
        double e1 = v1rayOr.x * m - v1v3.x * n - rayDir.x * p;
        double beta = e1 * inv_denom;
        
        if (beta < 0.0 || beta > 1.0)
        {
            continue;
        }
        
        double e2 = v1v2.x * n + v1rayOr.x * q + rayDir.x * r;
        double gamma = e2 * inv_denom;
        
        if (gamma < 0.0 || gamma > 1.0)
        {
            continue;
        }
        
        if (beta + gamma > 1.0)
        {
            continue;
        }
        
        double e3 = v1v2.x * p - v1v3.x * r + v1rayOr.x * s;
        double t = e3 * inv_denom;
        
        if (t < ray.tmin)
        {
            continue;
        }
        
        vec3 x = ray.o + (float)t * ray.d;
        double tDist = glm::distance(ray.o, x);
        if (distance > tDist)
        {
            distance = tDist;
            hit.normal = interpolateNormal(beta, gamma, m_faces[i]);
            hit.hitPoint = x;
            hit.color = m_color;
        }
        
        result = true;
    }
    
    return result;
}

vec3 Mesh::interpolateNormal(const float beta, const float gamma, Face face)
{
    vec3 normal((1 - beta - gamma) * m_vertexNormals[face.index1]
                + beta * m_vertexNormals[face.index2]
                + gamma * m_vertexNormals[face.index3]);
    
    return normalize(normal);
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

void Mesh::renderBoundingBox()
{
    glBegin(GL_LINES);
    {
        glVertex3f(m_aabb->xMin, m_aabb->yMin, m_aabb->zMin);
        glVertex3f(m_aabb->xMax, m_aabb->yMin, m_aabb->zMin);

        glVertex3f(m_aabb->xMax, m_aabb->yMin, m_aabb->zMin);
        glVertex3f(m_aabb->xMax, m_aabb->yMax, m_aabb->zMin);
        
        glVertex3f(m_aabb->xMax, m_aabb->yMax, m_aabb->zMin);
        glVertex3f(m_aabb->xMin, m_aabb->yMax, m_aabb->zMin);
        
        glVertex3f(m_aabb->xMin, m_aabb->yMax, m_aabb->zMin);
        glVertex3f(m_aabb->xMin, m_aabb->yMin, m_aabb->zMin);

        glVertex3f(m_aabb->xMin, m_aabb->yMin, m_aabb->zMin);
        glVertex3f(m_aabb->xMin, m_aabb->yMin, m_aabb->zMax);

        glVertex3f(m_aabb->xMax, m_aabb->yMin, m_aabb->zMin);
        glVertex3f(m_aabb->xMax, m_aabb->yMin, m_aabb->zMax);

        glVertex3f(m_aabb->xMax, m_aabb->yMax, m_aabb->zMin);
        glVertex3f(m_aabb->xMax, m_aabb->yMax, m_aabb->zMax);

        glVertex3f(m_aabb->xMin, m_aabb->yMax, m_aabb->zMin);
        glVertex3f(m_aabb->xMin, m_aabb->yMax, m_aabb->zMax);

        glVertex3f(m_aabb->xMin, m_aabb->yMin, m_aabb->zMax);
        glVertex3f(m_aabb->xMax, m_aabb->yMin, m_aabb->zMax);
        
        glVertex3f(m_aabb->xMax, m_aabb->yMin, m_aabb->zMax);
        glVertex3f(m_aabb->xMax, m_aabb->yMax, m_aabb->zMax);
        
        glVertex3f(m_aabb->xMax, m_aabb->yMax, m_aabb->zMax);
        glVertex3f(m_aabb->xMin, m_aabb->yMax, m_aabb->zMax);
        
        glVertex3f(m_aabb->xMin, m_aabb->yMax, m_aabb->zMax);
        glVertex3f(m_aabb->xMin, m_aabb->yMin, m_aabb->zMax);
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
