#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

uniform mat4 mvp;

out vec3 fPosition;
out vec3 fNormal;

void main()
{
    vec3 tPosition = vPosition;
    gl_Position = mvp * vec4(tPosition, 1);

    fPosition = vPosition;
    fNormal = normalize(vNormal);
}