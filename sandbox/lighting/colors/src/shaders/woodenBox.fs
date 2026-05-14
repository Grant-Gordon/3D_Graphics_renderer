#version 330 core
in vec2 vOutTexCoord; 
in vec3 vOutNormalPos;
in vec3 vOutFragPos;

out vec4 fOutColor;

uniform sampler2D samplerTexture;
uniform vec3 objectColor;
//lighting
uniform vec3 lightBoxPos;
uniform vec3 lightColor;
uniform float ambientLightLevel;
//unform sampler2D my other texture or whatever. 

void main(){
    vec3 norm = normalize(vOutNormalPos); //normalizes to unit vector
    vec3 lightDir = normalize(lightBoxPos - vOutFragPos);
    float diffusionFactor = max(dot(norm, lightDir), 0.0); //max so that theta>90 is not negative dot product
    vec3 diffusionLight = diffusionFactor * lightColor;
    
    vec3 ambientLight = ambientLightLevel * lightColor;
    vec3 boxColor = (ambientLight + diffusionLight) * objectColor;
    fOutColor = texture(samplerTexture, vOutTexCoord) * vec4(boxColor, 1.0);
}
