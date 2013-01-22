/* ----------------------------------------------------------------
   name:           Image.cpp
   purpose:        texturing tutorial
   'introduction to computer graphics'
   winter term 2012/2013, assignment 4
   version:    SKELETON CODE
   TODO:           texture and mipmap generation, texture filtering, modulation, texel get, painting in texture
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */


#ifdef __APPLE__
#include <GL/freeglut.h>
#elif _WIN32
#include <GL/glew.h>
#include "win32/freeglut.h"
#else
#include <GL/freeglut.h>
#endif

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Image.hpp"
#include "Context.hpp"


using namespace std;
using namespace glm;

Image::Image() : width(0), height(0), wrap(GL_CLAMP_TO_BORDER), min(GL_LINEAR), mag(GL_LINEAR), modulate(GL_MODULATE), textureID(0){
}

Image::Image(int width, int height)
    : width(width)
    , height(height)
    , data(width*height)
    , wrap(GL_CLAMP_TO_BORDER)
    , min(GL_LINEAR)
    , mag(GL_LINEAR)
    , modulate(GL_MODULATE)
    , textureID(0)
{}

Image::Image(const std::string& filename) : wrap(GL_CLAMP_TO_BORDER), min(GL_LINEAR), mag(GL_LINEAR), modulate(GL_MODULATE), textureID(0){
    load(filename);
}

Image::~Image(){
}

// generate OpenGL texture
// XXX: NEEDS TO BE IMPLEMENTED
void Image::generateTexture(){

    cout << "generating texture... " << endl;

    if(textureID==0){
        // generate texture id
        // XXX
        glGenTextures(1, &textureID);
        bind();
        //glBindTexture(GL_TEXTURE_2D, textureID);
        // END XXX
    }

    // texture filtering and repeat
    // XXX
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    setMinFilter(GL_NEAREST);
    setMagFilter(GL_NEAREST);
    // END XXX

    //enable automatic mipmap generation
    // XXX
    //  glGenerateMipmap(GL_TEXTURE_2D);
    int mipmapOk = gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_FLOAT, &data[0] );
    //cout << "mipmapOk " << mipmapOk << endl;
    // END XXX

    // upload texture data
    // XXX
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    //glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    //glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    //Ignore surface color
    setModulation(GL_DECAL);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &data[0]);

    // END XXX
}

// set texture parameter
void Image::setMinFilter(GLuint min){
    this->min= min;

    // XXX
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->min);
    // END XXX
}

// set magnifying filter
// XXX: NEEDS TO BE IMPLEMENTED
void Image::setMagFilter(GLuint mag){
    this->mag= mag;

    // XXX
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->mag);
    // END XXX
}

// set modulation
void Image::setModulation(GLuint modulation){
    this->modulate= modulation;

    cout << "modulation " << this->modulate<<endl;
    // XXX
    // GL_DECAL, GL_REPLACE, GL_MODULATE, or GL_BLEND
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, this->modulate );
    // END XXX
}

// bind texture
// XXX: NEEDS TO BE IMPLEMENTED
void Image::bind(){

    // XXX
    glBindTexture(GL_TEXTURE_2D, textureID);
    // END XXX
}

// unbind texture
// XXX: NEEDS TO BE IMPLEMENTED
void Image::unbind(){

    // XXX
    glBindTexture(GL_TEXTURE_2D, 0);
    // END XXX
}

// read a pixel from image
// XXX: NEEDS TO BE IMPLEMENTED
vec4 Image::get(unsigned int x, unsigned int y){

    // XXX
    glm::vec4 pixel;
    pixel = data[height*y+x];
    //    cout << "pixel x " << pixel.x << ", y:" << pixel.y << ", z:" << pixel.z << ", w:" << pixel.w << endl;
    return pixel;
    // END XXX
}

bool pen = false;
const int size = 10;
GLubyte subImage[size][size][4];


// draw in texture
// XXX: NEEDS TO BE IMPLEMENTED
void Image::paint(float x, float y){
    // XXX

    //    cout<<"paint " << x << "/" << y <<endl;
    if(!pen) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                subImage[i][j][0] = (GLubyte) 33;
                subImage[i][j][1] = (GLubyte) 100;
                subImage[i][j][2] = (GLubyte) 10;
                subImage[i][j][3] = (GLubyte) 255;
            }
        }
        pen = true;
        cout<<"thing set up"<<endl;
    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, size, size, GL_RGBA, GL_UNSIGNED_BYTE, subImage);
    // END XXX
}

// erase drawing from texture
// XXX: NEEDS TO BE IMPLEMENTED
void Image::erase(float x, float y){
    // XXX
    //cout << "erase " << x << "/" << y << endl;

    // just redraw original texture to delete what was painted
    //    glBindTexture(GL_TEXTURE_2D, textureID);
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &data[0]);

    // erase the previously painted dots from the texture like a rubber
    glm::vec4 subImg = get(x,y);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_FLOAT, &subImg[0]);
    // END XXX
}

void Image::load(const std::string& filename){

    cout << "loading texture..." << endl;
    data.clear();

    if(filename.substr(filename.size()-4, 4) == ".ppm") loadPPM(filename);
    else{
        cerr << "file " << filename << " is not a PPM file" << endl;
        return;
    }
}

void Image::loadPPM(const std::string& filename){

    ifstream file(filename.c_str(), ios::binary);

    if(!file.is_open()){
        cerr << "opening file " << filename << " failed" << endl;
        return;
    }

    // grab first two chars of the file and make sure that it has the
    // correct magic cookie for a raw PPM file.
    string magic;
    getline(file, magic);
    if(magic.substr(0, 2) != "P6"){
        cerr << "File " << filename << " is not a raw PPM file" << endl;
        return;
    }

    // grab the three elements in the header (width, height, maxval).
    string dimensions;
    do{
        getline(file, dimensions);
    }
    while(dimensions[0] == '#');

    stringstream(dimensions) >> width >> height;

    string max;
    getline(file, max);
    int maxValue;
    stringstream(max) >> maxValue;
    // grab all the image data in one fell swoop.
    vector<char> raw(width*height*3);
    file.read(&raw[0], raw.capacity());
    file.close();

    data.resize(width*height);
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            data[y*width+x]= vec4((unsigned char)raw[(height - y-1) * width * 3 + 3*x], (unsigned char)raw[(height - y-1) * width * 3 + 3*x + 1], (unsigned char)raw[(height - y-1) * width * 3 + 3*x + 2], maxValue);
            data[y*width+x]/= maxValue;
            //int i = y*width+x;
            //cout << data[i].r << " " << data[i].g << " " << data[i].b << " " << data[i].a << endl;
        }
    }

    raw.clear();

    std::cout << "Image " << filename << " loaded. width=" << width << " height=" << height << endl;
}
