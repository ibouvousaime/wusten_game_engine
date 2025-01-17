#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include"constants.hpp"
struct Camera {
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	int width;
	int height;
	float speed = 0.1f;
	float sensitivity = 100.0f;
	glm::mat4 view_matrix;
	glm::mat4 projection;
	Camera(glm::vec3 position, glm::vec3 orientation, int width, int height);
	void calculateMatrix();
};