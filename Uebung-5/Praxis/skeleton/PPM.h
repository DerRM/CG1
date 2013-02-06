//
//  PPM.h
//  Cg1_ex5
//
//  Created by Christopher Sierigk on 06.02.13.
//
//

#ifndef __Cg1_ex5__PPM__
#define __Cg1_ex5__PPM__

#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class PPM {
public:
    PPM(size_t height, size_t width);
    ~PPM(){};
    void saveImage(vector<vec3> imageData);
private:
    size_t m_height;
    size_t m_width;
};

#endif /* defined(__Cg1_ex5__PPM__) */
