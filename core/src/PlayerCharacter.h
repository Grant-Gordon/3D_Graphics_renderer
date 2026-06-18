#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "GameObject.h"
#include "Camera.h"

class PlayerCharacter: public GameObject {
public:
    PlayerCharacter();
    PlayerCharacter(const unsigned int playerID, Model* Model, , Transform transform);
    ~PlayerCharacter() = default;
    const unsigned int playerID;

private:
    Camera m_camera;
};
#endif // PLAYER_CHARACTER_H
