#version 330 core
layout(location = 0) in vec3 vInPos;
layout(location = 1) in vec3 vInNormalPos;
layout(location = 2) in vec2 vInTexCoord;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main() {
    vec4 worldPos = modelTransform * vec4(vInPos, 1.0f);

    
    FragPos= vec3(worldPos); // put fragment in world position

    TexCoords = vInTexCoord;

    Normal = mat3(transpose(inverse(modelTransform))) * vInNormalPos;

    gl_Position = projectionTransform * viewTransform * worldPos;
}
