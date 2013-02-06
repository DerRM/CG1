//
//  Plane.cpp
//  Cg1_ex5
//
//  Created by Christopher Sierigk on 05.02.13.
//
//

#include "Plane.h"

Plane::Plane(vec3 normal, vec3 point)
:m_Normal(normal), m_Point(point)
{
    m_Color = vec3(1.0, 1.0, 1.0);
}

vec3 Plane::getNormal()
{
    return m_Normal;
}

vec3 Plane::getPoint()
{
    return m_Point;
}

vec3 Plane::getColor()
{
    return m_Color;
}

void Plane::setColor(vec3 color)
{
    m_Color = color;
}

#define EPSILON 0.001

bool Plane::hit(Ray &ray, Hit &hit)
{
    double denom = dot(ray.d, m_Normal);
    
    if ((denom >= 0.0 + EPSILON && denom <= 0.0 - EPSILON) || denom >= 0.0 + EPSILON)
    {
        return false;
    }
    
    double t = dot(m_Point - ray.o, m_Normal) / denom;
    
    if (t < ray.tmin)
    {
        return false;
    }
    
    hit.normal = m_Normal;
    hit.color = m_Color;
    hit.hitPoint = ray.o + (float) t * ray.d;
    
    return true;
}