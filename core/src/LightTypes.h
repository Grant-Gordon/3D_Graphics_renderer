#ifndef LIGHT_TYPES_H
#define LIGHT_TYPES_H

#include "GameObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct PhongProperties{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
struct Attenuation{
    float constant;
    float linear;
    float quadratic;
};


struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;

    PhongProperties phong;
};

struct PointLight {
    GameObject gameObject;
    glm::vec3 position;
    glm::vec3 color;

    PhongProperties phong;

    Attenuation attenuation;
};

struct SpotLight {
    GameObject gameObject;
    glm::vec3 position;
    glm::vec3 color;

    PhongProperties phong;

    Attenuation attenuation;
    // spotlight cone
    glm::vec3 direction;
    float outerCutOff;
    float innerCutOff;
};
#endif // LIGHT_TYPES_H
