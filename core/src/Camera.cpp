#include "Camera.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch):
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    movementSpeed(SPEED),
    mouseSensitivity(SENSITIVITY),
    zoom(ZOOM) {

    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    Camera::updateCameraVector();
}
// constructor with Scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch):
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    movementSpeed(SPEED),
    mouseSensitivity(SENSITIVITY),
    zoom(ZOOM) {

    this->position = glm::vec3(posX, posY, posZ);
    this->worldUp = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;
    Camera::updateCameraVector();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    //    std::cout << " CAMERAUPDATE::" << direction << " = " << velocity << std::endl;
    if(direction == FORWARD)
        position += front * velocity;
    if(direction == BACKWARD)
        position -= front * velocity;
    if(direction == LEFT)
        position -= right * velocity;
    if(direction == RIGHT)
        position += right * velocity;
    if(direction == UP)
        position += worldUp * velocity;
    if(direction == DOWN)
        position -= worldUp * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    // std::cout << "xOffset, yOffset: (" << xoffset << "," << yoffset << ")" << std::endl;
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // if pitch out of bounds screen doesn't flip
    if(constrainPitch) {
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
    }

    // std::cout << "Yaw, Pitch: (" << Yaw << "," << Pitch << ")" << std::endl;
    //  update Front, Right, UP vecs using updated euler angles
    Camera::updateCameraVector();
}

void Camera::processMouseScroll(float yoffset) {
    zoom -= (float)yoffset;
    if(zoom < 10.f)
        zoom = 1.0f;
    if(zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::updateCameraVector() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // recalc Right and Up vecs
    right = glm::normalize(glm::cross(front,
        worldUp)); // normalize because len approaches 0 the more yoou look up or donw, thus slower movement
    up = glm::normalize(glm::cross(right, front));
}
