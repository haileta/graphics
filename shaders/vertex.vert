#version 410 core
in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoord;

out vec2 TexCoord; // specify a texture coord output to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    TexCoord = aTexCoord;
}