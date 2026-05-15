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
uniform float specularLightLevel;

uniform vec3 viewPos;
//unform sampler2D my other texture or whatever. 

void main(){
    //if scaling an object, need a normal matrix to put things into world view
    vec3 norm = normalize(vOutNormalPos); //normalizes to unit vector
    vec3 lightDir = normalize(lightBoxPos - vOutFragPos);
    vec3 viewDir = normalize(viewPos - vOutFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diffusionFactor = max(dot(norm, lightDir), 0.0); //max so that theta>90 is not negative dot product
    vec3 diffusionLight = diffusionFactor * lightColor;

    float shininess = 64; //2 = not shiny, 256 = very shiny
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specularLight = specularLightLevel * specularFactor * lightColor;
    
    vec3 ambientLight = ambientLightLevel * lightColor;
    vec3 boxColor = (ambientLight + diffusionLight + specularLight) * objectColor;
    //fOutColor = texture(samplerTexture, vOutTexCoord) * vec4(boxColor, 1.0);
    fOutColor = vec4(boxColor, 1.0);
}
