#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

void Camera::move(const glm::vec3& direction) {
    position += direction;
}

void Camera::lookAt(const glm::vec3& pos) {
    front = glm::normalize(pos - position);
}

void Camera::lockView(const glm::vec3& pos) {
    lock = true;
    target = pos;
}

void Camera::unlockView() {
    lock = false;
}

void Camera::moveForward(float distance) {
    position += distance * front;
}

void Camera::moveBackward(float distance) {
    position -= distance * front;
}
void Camera::moveLeft(float distance) {
    position -= glm::normalize(glm::cross(front, up)) * distance; // Right handed.
}

void Camera::moveRight(float distance) {
    position += glm::normalize(glm::cross(front, up)) * distance; // Right handed.
}

void Camera::moveUp(float distance) {
    position += distance * up;
}
void Camera::moveDown(float distance) {
    position -= distance * up;
}

glm::mat4 Camera::getViewMat() {
    if (lock) {
        front = glm::normalize(target - position);
    }
    return glm::lookAt(position, position + front, up);
}
