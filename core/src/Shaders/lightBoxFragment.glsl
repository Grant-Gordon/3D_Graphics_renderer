#version 330 core
out vec4 fOutFragColor;

uniform vec3 lightColor;
void main(){
    fOutFragColor = vec4(lightColor, 1.0); 
}
