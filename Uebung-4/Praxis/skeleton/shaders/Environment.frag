/// @file
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Copyright (C) 2011      Computer Graphics, TU Berlin
///
////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  module     : Environment.frag
///
///  project    : Computer Graphics 1
///
///  description: Simple fragment shader
///
////////////////////////////////////////////////////////////////////////////////////////////////////

const vec3 Xunitvec = vec3(1.0, 0.0, 0.0);
const vec3 Yunitvec = vec3(0.0, 1.0, 0.0);

//uniform vec3  BaseColor;
//uniform float MixRatio;
uniform sampler2D EnvMap;

varying vec3 Normal;
varying vec3  EyeDir;
varying vec3  myMatrix;

//varying float LightIntensity;

void main()
{
    // Compute reflection vector

    vec3 reflectDir = reflect(EyeDir, Normal);

    reflectDir *= myMatrix;

    // Compute altitude and azimuth angles

    vec2 index;

    index.t = dot(normalize(reflectDir), Yunitvec);
    reflectDir.y = 0.0;
    index.s = dot(normalize(reflectDir), Xunitvec) * 0.5;

    // Translate index values into proper range

    if (reflectDir.z >= 0.0)
        index = (index + 1.0) * 0.5;
    else
    {
        index.t = (index.t + 1.0) * 0.5;
        index.s = (-index.s) * 0.5 + 1.0;
    }

    // if reflectDir.z >= 0.0, s will go from 0.25 to 0.75
    // if reflectDir.z < 0.0, s will go from 0.75 to 1.25, and
    // that's OK, because we've set the texture to wrap.

    // Do a lookup into the environment map.

    vec3 envColor = vec3(texture2D(EnvMap, index));

    // Add lighting to base color and mix

    //vec3 base = LightIntensity * BaseColor;
    //envColor = mix(envColor, base, MixRatio);

    gl_FragColor = vec4(envColor, 1.0);
}




/*
void main(){

    vec3 normalized_light_dir = normalize(vertex_to_light_vector);
    vec3 half_vec = normalize(vertex_to_light_vector + eye_vec);
    float half_dot_normal = dot(half_vec, normalized_normal);
    vec4 light_spec = vec4(0.0, 0.0, 0.0, 0.0);

    if (half_dot_normal > 0.0)
        {
            light_spec = diffuseTerm * gl_FrontLightProduct[0].specular * pow(half_dot_normal, gl_FrontMaterial.shininess);
        }
}
*/
