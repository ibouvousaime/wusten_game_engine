#include "model.hpp"

Model::Model(std::string name, std::string shaderName, std::vector<float> vertex_positions, std::vector<unsigned int> indices) {
	this->name = name;
	this->vertex_positions = vertex_positions;
	this->indices = indices;
	this->shaderName = shaderName;
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->rotation = glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
}

void Model::setPosition(glm::vec3 position) {
	this->position = position;
}
void Model::setScale(glm::vec3 scale) {
	this->scale = scale;
}
void Model::setRotation(glm::vec3 rotation) {
	this->rotation = rotation;
}

glm::vec3 Model::getPosition() {
	return this->position;
}
glm::vec3 Model::getScale() {
	return this->scale;
}
glm::vec3 Model::getRotation() {
	return this->rotation;
}
std::vector<float> Model::getVertexPositions() {
	return this->vertex_positions;
}

glm::mat4 Model::getGlobalViewModel() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, this->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, this->scale);
	model = glm::translate(model, this->position);

	return model;
}

Model generateCube(std::string name, std::string shaderName, glm::vec3 dimensions) {
	auto halfDimension = dimensions * 0.5f;
	std::vector<float> vertices = {
		 0.5f * dimensions.x,  0.5f * dimensions.y,  0.5f * dimensions.z,
		-0.5f * dimensions.x,  0.5f * dimensions.y,  0.5f * dimensions.z,
		-0.5f * dimensions.x, -0.5f * dimensions.y,  0.5f * dimensions.z,
		 0.5f * dimensions.x, -0.5f * dimensions.y,  0.5f * dimensions.z,

		  0.5f * dimensions.x,  0.5f * dimensions.y, -0.5f * dimensions.z,
		 -0.5f * dimensions.x,  0.5f * dimensions.y, -0.5f * dimensions.z,
		 -0.5f * dimensions.x, -0.5f * dimensions.y, -0.5f * dimensions.z,
		  0.5f * dimensions.x, -0.5f * dimensions.y, -0.5f * dimensions.z,
	};
	std::vector<unsigned int> indices = {
		0, 1, 2,
		2, 3, 0,
		0, 3, 7,
		7, 4, 0,
		2, 6, 7,
		7, 3, 2,
		1, 5, 6,
		6, 2, 1,
		4, 7, 6,
		6, 5, 4,
		5, 1, 0,
		0, 4, 5,
	};

	auto output = Model(name, shaderName, vertices, indices);
	return output;
}