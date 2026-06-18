#ifndef RENDERER_H
#define RENDERER_H

#include "GameObject.h"
#include "GameState.h"
#include "Shader.h"

class Renderer {
public:
    Renderer();
    ~Renderer() = default;

    void Draw(GameState& gameState);

private:
    Shader m_castersSP;
    Shader m_lightSourceSP;

    void drawGameObject(GameObject& GameObject);
    void drawLightSource(GameObject& GameObject);
};
#endif // RENDERER_H
