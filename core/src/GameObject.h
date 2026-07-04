#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory> //shared_ptr

struct Transform {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    glm::mat4 getModelTransform(){
        //SRT
        glm::mat4 modelTransform =glm::translate(glm::mat4(1.0f), position);
        //TODO: look into quarternions for rotation, currently assuming euler angles but IDK if thats true
        modelTransform = glm::rotate(modelTransform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        modelTransform = glm::rotate(modelTransform, rotation.y, glm::vec3(1.0f, 1.0f, 0.0f));
        modelTransform = glm::rotate(modelTransform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        modelTransform = glm::scale(modelTransform, scale);
        return modelTransform;
    };

};

class GameObject {

public:
    GameObject(std::shared_ptr<const Model> model, Transform& transform);
    ~GameObject() = default;

    // Getters
    const Transform& getTransform() const {
        return m_transform;
    }

    // Setters
    // lValues(named variables)
    void setTransform(const Transform& transform) {
        m_transform = transform;
    }
    void setPosition(const glm::vec3& position) {
        m_transform.position = position;
    }
    void setScale(const glm::vec3& scale) {
        m_transform.scale = scale;
    }
    void setRotation(const glm::vec3& rotation) {
        m_transform.rotation = rotation;
    }
    // rValues(temp vars)
    void setTransform(Transform&& transform) {
        m_transform = std::move(transform);
    }
    void setPosition(glm::vec3&& position) {
        m_transform.position = std::move(position);
    }
    void setScale(glm::vec3&& scale) {
        m_transform.scale = std::move(scale);
    }
    void setRotation(glm::vec3&& rotation) {
        m_transform.rotation = std::move(rotation);
    }

private:
    Transform m_transform;
    std::shared_ptr<const Model> m_model;
};

#endif // GAMEOBJECT_H
