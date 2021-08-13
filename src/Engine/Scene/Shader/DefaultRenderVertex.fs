#version 400 core

layout(location=0) out vec4 frag_volor; //fragment shader output

//input form the vertex shader
smooth in vec4 vSmoothColor; //interpolated colour to fragment shader
smooth in vec4 vFragColor;
smooth in vec2 vUV;

uniform sampler2D tex0;

void main() {
    // set the interpolated color as the shader output\n"
    vec4 texColor = texture( tex0, vUV );
    frag_volor = vFragColor;
//"    vFragColor = texture(tex0, vUV);
}
