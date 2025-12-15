#version 410 core
out vec4 FragColor;

uniform vec3 lightColor;

void main() {

    vec3 brighter = lightColor * 1.3;
    FragColor = vec4(brighter, 1.0);
}
