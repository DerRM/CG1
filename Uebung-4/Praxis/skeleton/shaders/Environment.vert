/// @file
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Copyright (C) 2011      Computer Graphics, TU Berlin
///
////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  module     : Environment.vert
///
///  project    : Computer Graphics 1
///
///  description: Simple vertex shader
///
////////////////////////////////////////////////////////////////////////////////////////////////////


varying vec3  Normal;
varying vec3  EyeDir;
varying vec3  myMatrix;

//varying float LightIntensity;

//uniform vec3  LightPos;

void main()
{
    //gl_Position    = ftransform();
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    Normal         = normalize(gl_NormalMatrix * gl_Normal);

    vec4 pos       = gl_ModelViewMatrix * gl_Vertex;
    EyeDir         = pos.xyz;

    myMatrix       = gl_ProjectionMatrix;
    //  LightIntensity = max(dot(normalize(LightPos - EyeDir), Normal), 0.0);
}
