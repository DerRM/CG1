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

struct Light
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

in vec3 normal;
in vec3 vertex_to_light_vector;
in vec3 eye_vec;
//in vec4 color;
out vec4 out_Color;

uniform Light my_light;
uniform Material my_material;

void main(){
	//TODO : IMPLEMENT THE PHONG SHADING MODEL

	vec3 normalized_normal = normalize(normal);
	vec3 normalized_light_dir = normalize(vertex_to_light_vector);

	vec3 half_vec = normalize(vertex_to_light_vector + eye_vec);

	float diffuseTerm = clamp(dot(normalized_normal, normalized_light_dir), 0.0, 1.0);
	
	float half_dot_normal = dot(half_vec, normalized_normal);

	vec4 light_spec = vec4(0.0, 0.0, 0.0, 0.0);

	if (half_dot_normal > 0.0)
	{
		light_spec = diffuseTerm * my_light.specular * my_material.specular * pow(half_dot_normal, my_material.shininess);
	}

	// Set the color of the fragment
	out_Color = my_light.ambient * my_material.ambient + diffuseTerm * my_light.diffuse * my_material.diffuse + light_spec;
}
	
