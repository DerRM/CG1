#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include <string>
#include <iostream>
#include <fstream>

// includes, system
#ifdef _WIN32
#include <windows.h>
#include <cassert>
#endif

#ifdef __APPLE__
  #include <GL/freeglut.h>
#elif _WIN32
  #include "win32/freeglut.h"
#else
  #include <GL/freeglut.h>
#endif

using namespace std;


/**
 *A class encaplulating (very quickly) a GLSL shader.
 */
class GLSLShader
{
 protected:
    // OpenGL handle to the vertex shader
    GLuint vs_object;
    // OpenGL handle to the fragment shader
    GLuint fs_object;
    // OpenGL handle to the whole shader
    GLuint prog_object;

 public:
    GLSLShader();
    ~GLSLShader();

    // check if shader is allready loaded
    bool notLoaded();

    // Load the shader from it's name (the path to the shader without extension .frag or .vert)
    void load(const string& name);

    // compile the shader from source directly
    void compileFromSource(const char* vertexShaderSource, const char* fragmentShaderSource);

    // Bind the shader to the openGL pipeline
    void bindShader()const;

    // Unbind the shader
    void unbindShader()const;

    // Set a int uniform variable
    void setIntParam(const char* pname, const int& value);

    // Set a float uniform variable
    void setFloatParam(const char* pname, const float& value);

    // Set a 2 component vector uniform parameter
    void setVector2Param(const char* pname, const glm::vec2& value);

    // Set a 3 component vector uniform parameter
    void setVector3Param(const char* pname, const glm::vec3& value);

    // Set a 4 component vector uniform parameter
    void setVector4Param(const char* pname, const glm::vec4& value);

    // Set a 4x4 matrix uniform parameter
    void setMatrix4Param(const char* pname, const glm::mat4& value);

};

// Print the info log for a program if the status is not OK.
void printProgramLog(GLuint program);

// Print the info log for a shader if the status is not OK.
void printShaderLog(GLuint shader);

// Get the source contained in a file as a string.
const std::string readShaderSourceource(const std::string& file);

// Create a shader of type stype from a source
GLuint createShaderFromSource(GLuint stype, const std::string& src);


// Create a shader of type stype from a source sile
GLuint createShader(GLuint stype, const std::string& file);


#endif
