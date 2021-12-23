#version 400 core

// RenderVertex layout
layout(location = 0) in vec3 position;	 // object space vertex position
layout(location = 1) in vec3 normal;	 // object space vertex normal
layout(location = 2) in vec3 color0;     // per-vertex diffuse colour
layout(location = 3) in vec2 texcoord0;  // per-vertex tex coord, stage 0

// output from the vertex shader
smooth out vec4 vSmoothColor;		//smooth colour to fragment shader
smooth out vec2 vUV;
smooth out vec4 vFragColor;

vec3 light_pos = vec3(0.0, 0.0, 2.0);
vec3 Ls        = vec3(1.0, 1.0, 1.0);
vec3 Ld        = vec3(0.7, 0.7, 0.7);
vec3 La        = vec3(0.2, 0.2, 0.2);

vec3 Ks = vec3(1.0, 1.0, 1.0);
vec3 Kd = vec3(0.7, 0.7, 0.7);
vec3 Ka = vec3(0.2, 0.2, 0.2);
float specular_exponent = 100.0;

uniform mat4 MVP;	//combined modelview projection matrix

void main() {
    //assign the per-vertex color to vSmoothColor varying
    vSmoothColor = vec4(color0, 1);

    vec3 Ia = La * Ka;
    vec3 Id = vec3(0.0, 0.0, 0.0);
    vec3 Is = vec3(0.0, 0.0, 0.0);
    //get the clip space position by multiplying the combined MVP matrix with the object space
    //vertex position
    gl_Position = MVP*vec4(position,1);
    vSmoothColor = vec4( color0, 1 );
    vFragColor = vec4(Is + Id + Ia, 1.0);
    vUV = texcoord0;
}
