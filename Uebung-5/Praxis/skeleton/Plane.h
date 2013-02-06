//
//  Plane.h
//  Cg1_ex5
//
//  Created by Christopher Sierigk on 05.02.13.
//
//

#ifndef __Cg1_ex5__Plane__
#define __Cg1_ex5__Plane__

#include <glm/glm.hpp>

#include "Ray.h"
#include "Hit.h"
#include "RTObject.h"

using namespace glm;

class Plane : public RTObject {
public:
    Plane(vec3 normal, vec3 point);
    ~Plane(){};
    virtual bool hit(Ray& ray, Hit& hit);
    vec3 getNormal();
    vec3 getPoint();
    void setColor(vec3 color);
    vec3 getColor();
private:
    vec3 m_Normal;
    vec3 m_Point;
    vec3 m_Color;
};

#endif /* defined(__Cg1_ex5__Plane__) */
