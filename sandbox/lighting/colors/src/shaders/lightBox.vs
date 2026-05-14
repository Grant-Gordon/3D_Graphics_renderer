#version 330 core
layout(location=0) in vec3 vInPos; //EXPECTS input to be in attrib spot location=0, does NOT SET locatio
//in vec2 vInTexCoord; //Does not need texCoord, even on same VAO, since layout defined by VAO not shader

//out vec2 vOutTexCoord; 

uniform mat4 model_transform;
uniform mat4 view_transform;
uniform mat4 projection_transform;

void main(){
    gl_Position = projection_transform * view_transform * model_transform * vec4(vInPos, 1.0f);
 //   vOutTexCoord=vInTexCoord;
}
