#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GameObject{

public:

    GameObject();
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    
    Model::Model &model;
    
    virtual void draw();
private:
};

#endif //GAMEOBJECT_H

