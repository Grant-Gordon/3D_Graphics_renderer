#ifndef GAME_STATE_H
#define GAME_STATE_H
#include "GameObject.h"
#include "LevelMap.h"
#include "PlayerCharacter.h"

#include <vector>

class GameState {
public:
private:
    GameState();
    GameState(const LevelMap& levelMap, std::vector<PlayerCharacter> playerCharacters, std::vector<GameObject> dynamicObjects);
    ~GameState() = default;

    LevelMap m_levelMap;
    std::vector<GameObject> m_dynamicObjects;
    std::vector<PlayerCharacter> m_playerCharacters;
};
#endif // GAME_STATE_H
