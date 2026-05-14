#version 330 core
in vec2 vOutTexCoord; 

out vec4 fOutColor;

uniform sampler2D samplerTexture;
uniform vec3 objectColor;
uniform vec3 lightColor;
//unform sampler2D my other texture or whatever. 

void main(){
    fOutColor = texture(samplerTexture, vOutTexCoord) * vec4(lightColor * objectColor, 1.0) ;
}
