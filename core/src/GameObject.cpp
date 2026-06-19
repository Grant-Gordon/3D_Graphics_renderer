#include "GameObject.h"

#include "Model.h"

#include <memory> //shared_ptr

GameObject::GameObject(std::shared_ptr<Model> model, Transform& transform):
    m_model(model),
    m_transform(transform) {
}

const Transform& GameObject::getTransform() const {
    return m_transform;
}
