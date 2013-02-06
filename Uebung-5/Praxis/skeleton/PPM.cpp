//
//  PPM.cpp
//  Cg1_ex5
//
//  Created by Christopher Sierigk on 06.02.13.
//
//

#include "PPM.h"

PPM::PPM(size_t height, size_t width)
: m_height(height), m_width(width)
{
    
}

void PPM::saveImage(vector<vec3> imageData)
{
    ofstream file;
    file.open("test.ppm");
    size_t w = m_width;
    size_t h = m_height;
    file << "P3" << "\n";
    file << w << " " << h << "\n";
    file << 255 << "\n";
    
    for (size_t i = h; i > 0; i--)
    {
        for (size_t j = 0; j < w; j++)
        {
            vec3 color = imageData[i * w + j];
            
            file << (int) (color.x * 255) << " " << (int) (color.y * 255) << " " << (int) (color.z * 255) << " ";
        }
        
        file << "\n";
    }
}
