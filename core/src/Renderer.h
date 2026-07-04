#ifndef RENDERER_H
#define RENDERER_H

#include "GameObject.h"
#include "GameState.h"
#include "Shader.h"

const glm::mat4 PROJECTION_TRANSFORM = glm::mat4(1.0f);

class Renderer {
public:
    Renderer() = default;
    Renderer(Shader& lightCastersSP, Shader& lightSourceSP);
    ~Renderer() = default;

    void DrawGameState(const GameState& gameState, const Camera& camera) const;

private:
    Shader m_castersSP;
    Shader m_lightSourceSP;

    void drawGameObject(GameObject& GameObject);
    void drawLightSource(GameObject& GameObject);
};
#endif // RENDERER_H
