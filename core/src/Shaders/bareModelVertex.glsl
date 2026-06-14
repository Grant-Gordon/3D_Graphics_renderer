#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main() {
    TexCoords = aTexCoords;
    gl_Position = projectionTransform * viewTransform * modelTransform * vec4(aPos, 1.0);
}
