#version 330 core

in vec2 vOutTexCoord; 
in vec3 vOutNormalPos;
in vec3 vOutFragPos;

out vec4 fOutColor;

uniform sampler2D samplerTexture;
uniform vec3 viewPos;

struct PhongMaterial{
    vec3 color;

    vec3 ambientLevel;
    vec3 diffuseLevel;
    vec3 specularLevel;
    float shininessLevel;
};
uniform PhongMaterial material;

struct PhongLight{
    vec3 position;
    vec3 color;

    vec3 ambientLevel;
    vec3 diffuseLevel;
    vec3 specularLevel;
};
uniform PhongLight light;

void main(){
    //ambient
    vec3 ambientLight = light.ambientLevel * material.ambientLevel * light.color;


    //Diffuse
    //if scaling an object, need a normal matrix to put things into world view
    vec3 norm = normalize(vOutNormalPos); //normalizes to unit vector
    vec3 lightDir = normalize(light.position - vOutFragPos);
    float facingLightDotProd = max(dot(norm, lightDir), 0.0); //max so that theta>90 is not negative dot product
    vec3 diffusionLight = light.diffuseLevel * light.color * (facingLightDotProd * material.diffuseLevel);

    //specular
    vec3 viewDir = normalize(viewPos - vOutFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specularDotProd = pow(max(dot(viewDir, reflectDir), 0.0), material.shininessLevel);
    vec3 specularLight = light.specularLevel * light.color * (specularDotProd * material.specularLevel);
    

    vec3 boxColor = (ambientLight + diffusionLight + specularLight);// * material.color;
    //fOutColor = texture(samplerTexture, vOutTexCoord) * vec4(boxColor, 1.0);
    fOutColor = vec4(boxColor, 1.0);
}
