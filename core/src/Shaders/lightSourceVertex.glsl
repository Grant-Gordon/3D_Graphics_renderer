#version 330 core
layout(location=0) in vec3 vInPos; //EXPECTS input to be in attrib spot location=0, does NOT SET locatio

//out vec2 vOutTexCoord; 

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main(){
    gl_Position = projectionTransform * viewTransform * modelTransform * vec4(vInPos, 1.0f);
}
