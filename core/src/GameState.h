#ifndef GAME_STATE_H
#define GAME_STATE_H
#include<vector>
#include "GameObject.h"
#include "LevelMap.h"
#include "PlayerCharacter.h"

class GameState{
public:

private:
    GameState();
    GameState(LevelMap levelMap, std::vector<PlayerCharacter> playerCharacters, std::vector<GameObject> dynamicObjects);
    ~GameState() = default;

    LevelMap levelMap;
    std::vector<GameObject> dynamicObjects;
    std::vector<PlayerCharacter> playerCharacters;
    
};
#endif // GAME_STATE_H
