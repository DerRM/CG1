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

// variables that are interpolated to the fragment shader
#version 150

in vec4 normal;
in vec3 vertex_to_light_vector;
in vec3 eye_vec;
in vec4 color;
out vec4 out_Color;

void main(){
	//TODO : IMPLEMENT THE PHONG SHADING MODEL

	vec4 normalized_normal = normalize(normal);
	vec3 normalized_light_dir = normalize(vertex_to_light_vector);
//
	vec3 half_vec = normalize(vertex_to_light_vector + eye_vec);
//
	float diffuseTerm = clamp(dot(normalized_normal, vec4(normalized_light_dir, 0.0)), 0.0, 1.0);
//	
	float half_dot_normal = dot(vec4(half_vec, 0.0), normalized_normal);
//
	vec4 light_spec = vec4(0.0, 0.0, 0.0, 0.0);
//
	if (half_dot_normal > 0.0)
	{
//		light_spec = diffuseTerm * gl_FrontLightProduct[0].specular * pow(half_dot_normal, gl_FrontMaterial.shininess);
	}

	// Set the color of the fragment
	out_Color = color;//gl_FrontLightProduct[0].ambient + diffuseTerm * gl_FrontLightProduct[0].diffuse + light_spec;
}
	
