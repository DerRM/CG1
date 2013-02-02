//
//  Ray.h
//  Cg1_ex5
//
//  Created by Christopher Sierigk on 02.02.13.
//
//

#ifndef Cg1_ex5_Ray_h
#define Cg1_ex5_Ray_h

#include <glm/glm.hpp>

/*
 * A simple structure to contain a ray
 */

using namespace glm;

struct Ray
{
    vec3 o;
    vec3 d;
    
    float tmin;
    
    Ray() : tmin(0) {}
    Ray(const vec3& origin, const vec3& dir)
    :  o(origin), d(dir), tmin(0)
    {
    }
    Ray(const vec3& origin, const vec3& dir, float eps)
    :  o(origin + eps*dir), d(dir), tmin(0)
    {
    }
    
    vec3 att(float t) const
    {
        return o + t*d;
    }
};

#endif
