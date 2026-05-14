#version 330 core
layout(location=0) in vec3 vInPos;
layout(location=1) in vec3 vInNormalPos;
layout(location=2) in vec2 vInTexCoord;

out vec2 vOutTexCoord; 
out vec3 vOutNormalPos;
out vec3 vOutFragPos;

uniform mat4 model_transform;
uniform mat4 view_transform;
uniform mat4 projection_transform;

void main(){
    gl_Position = projection_transform * view_transform * model_transform * vec4(vInPos, 1.0f);
    vOutFragPos = vec3(model_transform * vec4(vInPos, 1.0)); //put fragment in world position
    vOutTexCoord=vInTexCoord;
    vOutNormalPos = vInNormalPos;
}
