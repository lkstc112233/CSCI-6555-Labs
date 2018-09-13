#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

void Camera::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Camera::move(const glm::vec3& direction) {
    position += direction;
}

void Camera::lookAt(const glm::vec3& pos) {
    front = glm::normalize(pos - position);
}

// void Camera::lockView(const glm::vec3& pos) {
//     target = pos;
// }

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

// void Camera::moveUp(float distance);
// void Camera::moveDown(float distance);

glm::mat4 Camera::getViewMat() {
    auto ret = glm::lookAt(position, position + front, glm::vec3(0, 1, 0));
    return ret;
}
