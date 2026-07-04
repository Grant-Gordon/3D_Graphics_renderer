#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS 8
uniform vec3 viewPos;
uniform int numPointLights;
uniform int numSpotLights;

struct PhongMaterial {
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
};
uniform PhongMaterial material;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight directionalLight;

struct PointLight {
    vec3 position;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform PointLight pointLights[MAX_POINT_LIGHTS];


struct SpotLight {
    vec3 position;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation
    float constant;
    float linear;
    float quadratic;

    // spotlight cone
    vec3 direction;
    float outerCutOff;
    float innerCutOff;
};
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main() {
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // directional
    vec3 result = CalcDirectionalLight(directionalLight, norm, viewDir);

    // point lights;
    for(int i = 0; i < numPointLights; i++) { result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); }

    // spotLight
    for(int i = 0; i < numSpotLights; i++) { result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir); }

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuseMap, TexCoords)) * light.color;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, TexCoords)) * light.color;
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap, TexCoords)) * light.color;

    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuseMap, TexCoords)) * light.color;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, TexCoords)) * light.color;
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap, TexCoords)) * light.color;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    // only calculate light within cone
    if(theta < light.outerCutOff) {
        return vec3(0.0);
    }
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);


    // combine lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuseMap, TexCoords)) * light.color;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, TexCoords)) * light.color;
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap, TexCoords)) * light.color;

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // edge smoothing
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
}
