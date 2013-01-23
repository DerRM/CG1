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
varying vec3 vertex_to_light_vector;
varying vec3 normal;
varying vec3 eye_vec;

void main(){
	// 	TODO : TRANSFORM THE VERTICES AND NORMALS AND PASS THEM TO THE RASTERIZATION STAGE
    vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;
    
    normal = gl_NormalMatrix * gl_Normal; // transform normal with inverse transpose of ModelViewMatrix
	eye_vec = vec3(-vertex_in_modelview_space);
	vertex_to_light_vector = vec3(gl_LightSource[0].position - vertex_in_modelview_space);
    
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

