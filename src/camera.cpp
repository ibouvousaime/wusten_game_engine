#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 target) {
	this->calculateMatrix(position, target);
}

void Camera::calculateMatrix() {
	this->calculateMatrix(this->position, this->target);
}

void Camera::calculateMatrix(glm::vec3 position, glm::vec3 target) {
	this->position = position;
	this->target = target;
	this->direction = glm::normalize(this->position - target);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->right = glm::normalize(glm::cross(this->up, this->direction));
	this->up = glm::cross(this->direction, this->right);
	this->view_matrix = glm::lookAt(this->position, this->target, this->up);
}