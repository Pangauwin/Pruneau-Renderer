#version 460 core

in vec3 vNormal;
in vec2 vUV;
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.75, 0.8, 1.0);
}