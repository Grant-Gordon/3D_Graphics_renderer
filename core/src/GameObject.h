#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Transform {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
};

class GameObject {

public:
    GameObject(Model* model, Transform& transform);
    ~GameObject() = default;

    Transform transform;
    Model* model;

private:
};

#endif // GAMEOBJECT_H
