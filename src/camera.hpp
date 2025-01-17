#include<glm/glm.hpp>

struct Camera {
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 direction;
	glm::mat4 view_matrix;

	Camera(glm::vec3 position, glm::vec3 target);
	void calculateMatrix(glm::vec3 position, glm::vec3 target);
	void calculateMatrix();
};