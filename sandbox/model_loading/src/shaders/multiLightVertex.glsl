#version 330 core
layout(location = 0) in vec3 vInPos;
layout(location = 1) in vec3 vInNormalPos;
layout(location = 2) in vec2 vInTexCoord;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model_transform;
uniform mat4 view_transform;
uniform mat4 projection_transform;

void main() {
    vec4 worldPos = model_transform * vec4(vInPos, 1.0f);

    
    FragPos= vec3(worldPos); // put fragment in world position

    TexCoords = vInTexCoord;

    Normal = mat3(transpose(inverse(model_transform))) * vInNormalPos;

    gl_Position = projection_transform * view_transform * worldPos;
}
