#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Camera.h"
#include "GameObject.h"
#include "Model.h"
#include <memory>

class PlayerCharacter: public GameObject {
public:
    PlayerCharacter();
    PlayerCharacter(const unsigned int playerID, std::shared_ptr<const Model>, Transform transform);
    ~PlayerCharacter() = default;
    const unsigned int playerID;

    const Camera& getCamera() const {return m_camera;}

private:
    Camera m_camera;
};
#endif // PLAYER_CHARACTER_H
