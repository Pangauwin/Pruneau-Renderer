#version 460 core

in vec3 vNormal;
in vec2 vUV;
in vec4 vColor;

out vec4 FragColor;

void main()
{
    vec3 n = normalize(vNormal);
    float lighting = max(dot(n, normalize(vec3(0.5, 1.0, 0.3))), 0.2);
    FragColor = vec4(vec3(0.6 * vColor.x, 0.8 * vColor.y, 1.0 * vColor.z) * lighting, 1.0);
}