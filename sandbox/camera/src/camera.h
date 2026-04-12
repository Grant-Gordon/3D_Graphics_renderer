#ifndef CAMERA_H
#define CAMERA_H


#include <glad/glad.h> //TODO: header gaurds
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera {
public:
    enum CameraMovement { FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH, 
    );

    // constructor with scalar values
    Camera(
        float posX,
        float posY,
        float posZ,
        float up,
        float upY,
        float upZ,
        float yaw,
        float pitch, 
    );

    glm::mat4 GetViewMatrix();

    // CameraMovement Enum
    void ProcessKeyBoard(CameraMovement direction, float deltaTime);

    void ProcessMouseMovement(float xoffsset, float yoffset, GLboolean constrainPitch = true);

    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVector();
};
#endif // CAMERA_H
