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
varying vec4 vertColor;
varying vec3 normal;

void main(){
	vertColor = gl_Vertex;
	// 	TODO : TRANSFORM THE VERTICES AND NORMALS AND PASS THEM TO THE RASTERIZATION STAGE
    normal = gl_NormalMatrix * gl_Normal;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

