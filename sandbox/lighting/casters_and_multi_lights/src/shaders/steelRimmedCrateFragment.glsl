#version 330 core

in vec2 vOutTexCoord;
in vec3 vOutNormalPos;
in vec3 vOutFragPos;

out vec4 fOutColor;

uniform vec3 viewPos;

struct PhongMaterial {
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininessLevel;
};
uniform PhongMaterial material;

struct DirectLight {
    vec3 direction;
    vec3 color;

    vec3 ambientLevel;
    vec3 diffuseLevel;
    vec3 specularLevel;
};
uniform DirectLight directLight;

struct PointLight {
    vec3 position;
    vec3 color;

    vec3 ambientLevel;
    vec3 diffuseLevel;
    vec3 specularLevel;

    // attentuation
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight pointLight;

struct Flashlight {
    vec3 position;
    vec3 color;

    vec3 ambientLevel;
    vec3 diffuseLevel;
    vec3 specularLevel;

    // attentuation
    float constant;
    float linear;
    float quadratic;

    // flashlight
    vec3 direction;
    float cutoff;
};
uniform Flashlight flashlight;


void main() {
    // ambient
    vec3 ambientLight = pointLight.ambientLevel * pointLight.color * vec3(texture(material.diffuseMap, vOutTexCoord));

    // Diffuse
    // if scaling an object, need a normal matrix to put things into world view
    vec3 norm = normalize(vOutNormalPos); // normalizes to unit vector
    // vec3 lightDir = normalize(-light.direction); //Direcitonal Light
    vec3 lightDir = normalize(pointLight.position - vOutFragPos); // positional/point light
    float facingLightDotProd = max(dot(norm, lightDir), 0.0);     // max so that theta>90 is not negative dot product
    vec3 diffusionLight = pointLight.diffuseLevel * pointLight.color * facingLightDotProd *
                          vec3(texture(material.diffuseMap, vOutTexCoord));

    // specular
    vec3 viewDir = normalize(viewPos - vOutFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specularDotProd = pow(max(dot(viewDir, reflectDir), 0.0), material.shininessLevel);
    vec3 specularLight = pointLight.specularLevel * pointLight.color *
                         (specularDotProd * vec3(texture(material.specularMap, vOutTexCoord)));

    // point light attenuation
    float distance = length(pointLight.position - vOutFragPos);
    float attenuationFactor =
        1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
    ambientLight *= attenuationFactor;
    diffusionLight *= attenuationFactor;
    specularLight *= attenuationFactor;

    // flashLight
    lightDir = normalize(flashlight.position - vOutFragPos);
    float theta = dot(lightDir, normalize(-flashlight.direction));
    
    vec3 flashlightAmbientLight =vec3(0.0);
    vec3 flashlightDiffusionLight =vec3(0.0);
    vec3 flashlightSpecularLight = vec3(0.0);
    if(theta > flashlight.cutoff){
        //do lighting calculations
        // ambient
         flashlightAmbientLight = flashlight.ambientLevel * flashlight.color * vec3(texture(material.diffuseMap, vOutTexCoord));

        // vec3 lightDir = normalize(-light.direction); //Direcitonal Light
        vec3 flashlightDir = normalize(flashlight.position - vOutFragPos); // positional/point light
        float flashlightFacingLightDotProd = max(dot(norm, flashlightDir), 0.0);     // max so that theta>90 is not negative dot product
        flashlightDiffusionLight = flashlight.diffuseLevel * flashlight.color * flashlightFacingLightDotProd *
                              vec3(texture(material.diffuseMap, vOutTexCoord));

        // specular
        vec3 flashlightViewDir = normalize(viewPos - vOutFragPos);
        vec3 flashlightReflectDir = reflect(-flashlightDir, norm);
        float flashlightSpecularDotProd = pow(max(dot(flashlightViewDir, flashlightReflectDir), 0.0), material.shininessLevel);
        flashlightSpecularLight = flashlight.specularLevel * flashlight.color *
                             (flashlightSpecularDotProd * vec3(texture(material.specularMap, vOutTexCoord)));

        //flashlight attenuation
        distance = length(flashlight.position - vOutFragPos);
        attenuationFactor =
            1.0 / (flashlight.constant + flashlight.linear * distance + flashlight.quadratic * (distance * distance));
        flashlightAmbientLight *= attenuationFactor;
        flashlightDiffusionLight *= attenuationFactor;
        flashlightSpecularLight *= attenuationFactor;

    }

    vec3 boxColor = (ambientLight + diffusionLight + specularLight + flashlightAmbientLight + flashlightDiffusionLight + flashlightSpecularLight); // * material.color;
    fOutColor = vec4(boxColor, 1.0);
}
