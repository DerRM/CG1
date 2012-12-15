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
varying vec3 vertex_to_light_vector;
varying vec3 normal;
varying vec3 eye_vec;

void main(){
	//TODO : IMPLEMENT THE PHONG SHADING MODEL

	float n = 100.0;
	float light = 0.4;

	vec3 normalized_normal = normalize(normal);
	vec3 normalized_light_dir = normalize(vertex_to_light_vector);

	vec3 half_vec = normalize(vertex_to_light_vector + eye_vec);

	float diffuseTerm = clamp(dot(normalized_normal, normalized_light_dir), 0.0, 1.0);
	
	float half_dot_normal = dot(half_vec, normalized_normal);

	float light_spec = 0.0;

	if (half_dot_normal > 0.0)
	{
		light_spec = diffuseTerm * light * pow(half_dot_normal, n);
	}

	// Set the color of the fragment
	gl_FragColor = gl_FrontMaterial.ambient + diffuseTerm * gl_FrontMaterial.diffuse + light_spec;
}
	
