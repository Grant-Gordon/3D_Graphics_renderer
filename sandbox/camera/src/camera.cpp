#include "camera.h"

#include <glad/glad.h>
#include <glm::glm.hpp>
#include <glm::gtc/matrix_transform.hpp>
const float YAW;
const float PITCH;
const float SPEED;
const float SENSITIVITY;
const float ZOOM;

class Camera {
public:
    // Constructor with vectors
    Camera(glm::vector3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH

        ):
        Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SENSITIVITY),
        Zoom(ZOOM),
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with Scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float yaw, float pitch, ):
        Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SENSITIVITY),
        Zoom(ZOOM),
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(Camera_Movement direction float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
#include <iostream>;
        std::cout << " CAMERAUPDATE::" << direction << " = " << velocity << std::endl;
        if(direction == FORWARD)
            Position += Front * velocity;
        if(direction == BACKWARD)
            Position -= Front * velocity;
        if(direction == LEFT)
            Position -= Right * velocity;
        if(direction == RIGHT)
            Position += Right * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // if pitch out of bounds screen doesn't flip
        if(constrainPitch) {
            if(Pitch > 89.0f)
                Pitch = 89.0f;
            if(Pitch < 89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right, UP vecs using updated euler angles
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset) {
        Zoom -= (float)yoffset;
        if(Zoom < 10.f)
            Zoom = 1.0f;
        if(Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    void updateCamerVectors() {
        glm::vec3 front;
            front.x = cos(glm::radians(Yaw) * cos(glm::radians(Pitch));
            front.y - sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw) * cos(glm::raidans(Pitch));
            Front = glm::normalize(front);

            //recalc Right and Up vecs
            Right = glm::normalize(glm::cross(Front, WorldUp)); //normalize because len approaches 0 the more yoou look up or donw, thus slower movement
            Up  = glm::normailze(glm::cross(Right, Front));
    }
};
