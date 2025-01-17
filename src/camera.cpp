#include "camera.hpp"


Camera::Camera(glm::vec3 position, glm::vec3 orientation, int width, int height) {
	this->position = position;
	this->orientation = orientation;
	this->width = width;
	this->height = height;
	this->calculateMatrix();
}

//void Camera::calculateMatrix(glm::vec3 position, glm::vec3 orientation) {
//    this->position = position;
//    this->orientation = orientation;
//    this->up = glm::vec3(0.0f, 1.0f, 0.0f); 
//    this->right = glm::normalize(glm::cross(this->orientation, world_up));
//    this->up = glm::normalize(glm::cross(this->right, this->orientation));
//    this->view_matrix = glm::lookAt(this->position, this->target, world_up);
//}	

void Camera::calculateMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	this->view_matrix = glm::lookAt(this->position, this->position + this->orientation, this->up);
	this->projection = glm::perspective(glm::radians(FOV), (float)this->width / this->height, CAMERA_CLIP_NEAR, CAMERA_CLIP_FAR);

}

