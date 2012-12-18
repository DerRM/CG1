/// @file
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Copyright (C) 2011      Computer Graphics, TU Berlin
///
////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  module     : blinnphong.frag
///
///  project    : Computer Graphics 1
/// 
///  description: Simple fragment shader
///
////////////////////////////////////////////////////////////////////////////////////////////////////

// uniform ("global") variables
uniform vec4 color;

varying vec4 vertColor;


void main(){
	// Draw the fragment with given color
	gl_FragColor = color;
}
	