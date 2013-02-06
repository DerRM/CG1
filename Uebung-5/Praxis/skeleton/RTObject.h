//
//  RTObject.h
//  Cg1_ex5
//
//  Created by Christopher Sierigk on 06.02.13.
//
//

#ifndef __Cg1_ex5__RTObject__
#define __Cg1_ex5__RTObject__

#include "Ray.h"
#include "Hit.h"

class RTObject {
public:
    virtual bool hit(Ray& ray, Hit& hit) = 0;
};

#endif /* defined(__Cg1_ex5__RTObject__) */
