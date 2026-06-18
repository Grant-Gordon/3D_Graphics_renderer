#include "GameObject.h"
#include "Model.h"

GameObject::GameObject(Model* model, Transform& transform):
    model(model),
    transform(transform) {
}
