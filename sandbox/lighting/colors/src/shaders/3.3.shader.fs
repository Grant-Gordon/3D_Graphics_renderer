#version 330 core
in vec2 vOutTexCoord; 

out vec4 fOutColor;
uniform sampler2D samplerTexture;
//unform sampler2D my other texture or whatever. 

void main(){
    fOutColor = texture(samplerTexture, vOutTexCoord) * vec4(1.0, 1.0, 1.0 , 1.0);
}
