#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include"constants.hpp"
struct Camera {
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	int width;
	int height;
	float speed = 0.1f;
	glm::mat4 view_matrix;
	glm::mat4 projection;
	float sensitivity = 10.0f;
	float pitch = 0.0f;
	float yaw = -90.0f;
	Camera(glm::vec3 position, glm::vec3 orientation, int width, int height);
	void calculateMatrix();
	void processMouseMovement(float xOffset, float yOffset);
};

void processInput(GLFWwindow* window, Camera* camera);
void mouseCallback(GLFWwindow* window, double xpos, double ypos); 