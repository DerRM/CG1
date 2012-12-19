/// @file
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Copyright (C) 2011      Computer Graphics, TU Berlin
///
////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  module     : blinnphong.vert
///
///  project    : Computer Graphics 1
/// 
///  description: Simple vertex shader
///
////////////////////////////////////////////////////////////////////////////////////////////////////


// variables that are interpolated to the fragment shader
#version 150

in vec3 aNormal;
in vec3 aPosition;
out vec3 vertex_to_light_vector;
out vec3 normal;
out vec3 eye_vec;
//out vec4 color;
uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform vec4 uLightPosition;

void main(){
	// 	TODO : TRANSFORM THE VERTICES AND NORMALS AND PASS THEM TO THE RASTERIZATION STAGE
    vec4 vertex_in_modelview_space = uMVMatrix * vec4(aPosition, 1.0);
    
    normal = vec3(uNormalMatrix * vec4(aNormal, 0.0)); // transform normal with inverse transpose of ModelViewMatrix
	eye_vec = vec3(-vertex_in_modelview_space);
	vertex_to_light_vector = vec3(uLightPosition - vertex_in_modelview_space);
    
	gl_Position = uMVPMatrix * vec4(aPosition, 1.0);
    //gl_PointSize = 5;
    
    //color = vec4(aPosition, 1.0);
}

