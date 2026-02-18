#version 460 core

in vec3 vNormal;
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D texture_diffuse_1;

void main()
{
    vec3 n = normalize(vNormal);
    float lighting = max(dot(n, normalize(vec3(0.5, 1.0, 0.3))), 0.2);
    FragColor = texture(texture_diffuse_1, vUV) * vec4(vec3(0.6, 0.8, 1.0) * lighting, 1.0);
}