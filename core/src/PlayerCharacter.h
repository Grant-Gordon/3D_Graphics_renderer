#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "GameObject.h"
#include "Camera.h"

class playerCharacter: public GameObject {
public:
    const unsigned int playerID;

private:
    Camera m_camera;
    void draw() overide;
};
#endif // PLAYER_CHARACTER_H
