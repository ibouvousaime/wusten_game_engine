#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>     
#include <iostream>
#include <glad/glad.h>
#include "utils.hpp"
#include "constants.hpp"
#include <cassert>
struct Vertex {
	bool initialized = false;
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	Vertex() : position(0.0f), normal(0.0f), texCoords(0.0f) {};
	Vertex(glm::vec3 position);
	Vertex(glm::vec3 position, glm::vec3 normal);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 texCoords);

};

struct Model {
private:
	std::string name;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	//std::vector<float> vertex_positions;
	std::vector<Vertex> vertices;
public:
	bool prepared = false;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int shaderProgram;
	std::string shaderName;
	std::vector<unsigned int> indices;
	glm::vec4 colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Model(std::string name, std::string shaderName, std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Model();
	void setPosition(glm::vec3 position);
	void setScale(glm::vec3 scale);
	void setRotation(glm::vec3 rotation);
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	std::vector<float> getVertexPositions();
	glm::mat4 getGlobalViewModel();
	void unloadModel();
};

Model generateCube(std::string name, std::string shaderName, glm::vec3 dimensions);
Model generateSphere(std::string name, std::string shaderName, glm::vec3 position, float radius, int stacks, int sectors);
Model importModel(std::string name, std::string shaderName, std::string filename);
