#include "Renderer.h"

#include "GameObject.h"
#include "GameState.h"
#include "LevelMap.h"
#include "LightTypes.h"
#include "Shader.h"

#include <string>
#include <vector>


Renderer::Renderer(Shader& lightCastersSP, Shader& lightSourceSP):
    m_castersSP(lightCastersSP),
    m_lightSourceSP(lightSourceSP) {};


void Renderer::DrawState(const GameState& gameState) {

    // TODO:: optimize with move or something
    LevelMap levelMap = gameState.getLevelMap();
    std::vector<PointLight> pointLights = levelMap.getPointLights();
    std::vector<SpotLight> spotLights = levelMap.getSpotLights();
    DirectionalLight directionalLight = leveMap.getDirectionalLight();
    std::vector<GameObject> mapObjects = levelMap.getStaticObjects();

    m_castersSP.use();
    setCastersSPUniforms(go.getTransform().getModelTransform, camera.getViewMatrix, PROJECTION_TRANSFORM,
        camera.position, pointLights, spotLights, directionalLight);
    for(GameObject go: mapObjects) { go.draw(); }

    // draw level map
    for(PointLight p: pointLights) {
        m_lightSourceSP.use();
        setLightSourceSPUniforms(p.getTransform().getModelTransform, camera.getViewMatrix, PROJECTION_TRANSFORM,
            p.color);
        p.draw();
    }
    for(SpotLight s: spotLights) {
        m_lightSourceSP.use();
        setLightSourceSPUniforms(s.getTransform().getModelTransform, camera.getViewMatrix, PROJECTION_TRANSFORM,
            s.color);
        s.draw();
    }


    // draw dynamic objects
};

void setCastersSPUniforms(const glm::mat4& modelTransform,
    const glm::mat4& viewTransform,
    const glm::mat4& projectionTransform,
    const glm::vec3& viewPos,
    const std::vector<PointLight>& spotLights,
    const std::vector<SpotLight>& spotLights,
    const DirectionalLight& directionalLight) {
    // Vertex Shader
    m_castersSP.setMat4("modelTransform", modelTransform);
    m_castersSP.setMat4("viewTransform", viewTransform);
    m_castersSP.setMat4("projectionTransform", projectionTransform);

    // Fragment Shader uniforms
    const int numPointLights = pointLights.size();
    const int numSpotLights = spotLights.size();
    m_castersSP.setInt("numPointLights", numPointLights);
    m_castersSP.setInt("numSpotLights", numSpotLights);
    m_castersSP.setVec3("viewPos", viewPos);

    // set lights attribs
    setDirectionalLightUniforms(m_castersSP, directionalLight);
    for(size_t i{0}; i < numPointLights; ++i) { setPointLightUniforms(m_castersSP, pointLights[i], i); }
    for(size_t i{0}; i < numspotLights; ++i) { setSpotLightUniforms(m_castersSP, spotLights[i], i); }
}

void setLightSourceSPUniforms(const glm::mat4& modelTransform,
    const glm::mat4& viewTransform,
    const glm::mat4& projectionTransform,
    glm::vec3 lightColor;) {

    // Vertex Shader
    m_lightSourceSP.setMat4("modelTransform", modelTransform);
    m_lightSourceSP.setMat4("viewTransform", viewTransform);
    m_lightSourceSP.setMat4("projectionTransform", projectionTransform);

    // Fragment shader
    m_lightSourceSP.setVec3("lightColor", lightColor);
}

void setDirectionalLightUniforms(Shader& shader, const Directionallight& d) {
    // directional Light
    shader.setVec3("directionalLight.direction", d.direction);
    shader.setVec3("directionalLight.color", d.color);

    shader.setVec3("directionalLight.ambient", d.phong.ambient);
    shader.setVec3("directionalLight.diffuse", d.phong.diffuse);
    shader.setVec3("directionalLight.specular", d.phong.specular);
}

void setPointLightUniforms(Shader& shader, const PointLight& pointLight, int idx) {
    // point Light 3
    shader.setVec3("pointLights[" + std::to_string(idx) + "].position", pointLightPositions[3]);
    shader.setVec3("pointLights[" + std::to_string(idx) + "].color", pointLightColors[3]);
    shader.setVec3("pointLights[" + std::to_string(idx) + "].ambient", ambientLight);
    shader.setVec3("pointLights[" + std::to_string(idx) + "].diffuse", pointLightDiffuse);
    shader.setVec3("pointLights[" + std::to_string(idx) + "].specular", pointLightSpecular);
    shader.setFloat("pointLights[" + std::to_string(idx) + "].constant", pointLightConstant);
    shader.setFloat("pointLights[" + std::to_string(idx) + "].linear", pointLightLinear);
    shader.setFloat("pointLights[" + std::to_string(idx) + "].quadratic", pointLightQuadratic);
}
void setSpotLightUniforms(Shader& shader, const SpotLight&, int idx) {
    // spotlight (flashlight)
    shader.setVec3("spotLights[" + std::to_string(idx) + "].position", s.position);
    shader.setVec3("spotLights[" + std::to_string(idx) + "].color", s.color);

    shader.setVec3("spotLights[" + std::to_string(idx) + "].ambient", s.phong.ambient);
    shader.setVec3("spotLights[" + std::to_string(idx) + "].diffuse", s.phong.diffuse);
    shader.setVec3("spotLights[" + std::to_string(idx) + "].specular", s.phong.specular);

    shader.setFloat("spotLights[" + std::to_string(idx) + "].constant", s.attenuation.constant);
    shader.setFloat("spotLights[" + std::to_string(idx) + "].linear", s.attenuation.linear);
    shader.setFloat("spotLights[" + std::to_string(idx) + "].quadratic", s.attenuation.quadratic);

    shader.setVec3("spotLights[" + std::to_string(idx) + "].direction", s.direction);
    shader.setFloat("spotLights[" + std::to_string(idx) + "].innerCutOff", s.innerCutoff.);
    shader.setFloat("spotLights[" + std::to_string(idx) + "].outerCutOff", s.outerCutoff);
}
