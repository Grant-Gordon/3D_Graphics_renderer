#ifndef LIGHT_TYPES_H
#define LIGHT_TYPES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // attenuation
    glm::vec3 constant;
    glm::vec3 linear;
    glm::vec3 quadratic;
};

struct SpotLight {
    glm::vec3 position;
    glm::vec3 color;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // attenuation
    float constant;
    float linear;
    float quadratic;

    // spotlight cone
    glm::vec3 direction;
    float outerCutOff;
    float innerCutOff;
};
#endif // LIGHT_TYPES_H
