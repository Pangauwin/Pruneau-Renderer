#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

out vec3 vNormal;
out vec2 vUV;

void main()
{
    vNormal = mat3(transpose(inverse(model))) * normal;
    vUV = uv_coord;
    gl_Position = perspective * view * model * vec4(aPos, 1.0);
}