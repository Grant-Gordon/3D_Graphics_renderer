#include "GameState.h"
#include "LevelMap.h"
#include "PlayerCharacter.h"
#include "GameObject.h"
#include <vector>


GameState::GameState(){};

GameState::GameState(LevelMap levelMap, std::vector<PlayerCharacter> playerCharacters, std::vector<GameObject> dynamicObjects):
m_levelMap(levelMap),
m_playerCharacters(playerCharacters),
m_dynamicObjects(dynamicObjects){
}
