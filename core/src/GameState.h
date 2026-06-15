#ifndef GAME_STATE_H
#define GAME_STATE_H
#include<vector>
#include "GameObject.h"
#include "LevelMap.h"
#include "PlayerCaharacter.h"

class GameState{
public:

private:
    LevelMap levelMap;
    std::vector<GameObject> dynamicObjects;
    std::vector<PlayerCharacter> playerCharacters;
};
#endif // GAME_STATE_H
