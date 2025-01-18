#include "camera.hpp"


Camera::Camera(glm::vec3 position, glm::vec3 orientation, int width, int height) {
	this->position = position;
	this->orientation = orientation;
	this->width = width;
	this->height = height;
	this->calculateMatrix();
}

void Camera::calculateMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	this->view_matrix = glm::lookAt(this->position, this->position + this->orientation, this->up);
	this->projection = glm::perspective(glm::radians(FOV), (float)this->width / this->height, CAMERA_CLIP_NEAR, CAMERA_CLIP_FAR);

}

void processInput(GLFWwindow* window, Camera* camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float speed = 0.5f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->position += speed * camera->orientation;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->position -= speed * camera->orientation;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->position -= glm::normalize(glm::cross(camera->orientation, camera->up)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->position += glm::normalize(glm::cross(camera->orientation, camera->up)) * speed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera->position += speed * camera->up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
		camera->position -= speed * camera->up;
	}

}

void Camera::processMouseMovement(float xOffset, float yOffset) {
    xOffset *= this->sensitivity / 100.0f;
    yOffset *= this->sensitivity / 100.0f;

    this->yaw += xOffset;
    this->pitch -= yOffset; 

    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    direction.y = -sin(glm::radians(this->pitch));
    direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->orientation = glm::normalize(direction);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;
    static float lastX = 0.0f;
    static float lastY = 0.0f;

    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    camera->processMouseMovement(xOffset, yOffset);
}