#version 330 core
in vec3 vInPos;
//in vec2 vInTexCoord;

//out vec2 vOutTexCoord; 

uniform mat4 model_transform;
uniform mat4 view_transform;
uniform mat4 projection_transform;

void main(){
    gl_Position = projection_transform * view_transform * model_transform * vec4(vInPos, 1.0f);
 //   vOutTexCoord=vInTexCoord;
}
