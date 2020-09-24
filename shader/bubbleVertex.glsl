#version 330 core

layout(location = 0) in vec3 mPosition;
layout(location = 1) in vec3 mOffset;
layout(location = 2) in float scale;
layout(location = 3) in vec4 vColor;

uniform mat4 vp;

out vec3 position;
out vec4 color;
out vec3 normal;

void main()
{
    vec3 vPosition = mPosition * scale + mOffset;
    vec3 tPosition = vPosition;
    tPosition.x += sin(vPosition.y * 1) * sin(vPosition.y) * cos(vPosition.y * 1) * 0.2;
    tPosition.y += sin(vPosition.x * 2) * 0.1;
    gl_Position = vp * vec4(tPosition, 1);

    position = vPosition;
    color = vColor;
    normal = normalize(mPosition);
}