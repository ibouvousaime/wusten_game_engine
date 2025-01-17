#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
struct Model {
private:
	std::string name;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	std::vector<float> vertex_positions;

public:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int shaderProgram;
	std::string shaderName;
	std::vector<unsigned int> indices;
	glm::vec4 colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Model(std::string name, std::string shaderName, std::vector<float> vertex_positions, std::vector<unsigned int> indices);
	void setPosition(glm::vec3 position);
	void setScale(glm::vec3 scale);
	void setRotation(glm::vec3 rotation);
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	std::vector<float> getVertexPositions();
	glm::mat4 getGlobalViewModel();
};

Model generateCube(std::string name, std::string shaderName, glm::vec3 dimensions);