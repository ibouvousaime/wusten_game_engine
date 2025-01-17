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
		-halfDimension.x, -halfDimension.y, halfDimension.z, //0
		halfDimension.x, -halfDimension.y, halfDimension.z,  //1
		halfDimension.x, -halfDimension.y, -halfDimension.z, //2
		-halfDimension.x, -halfDimension.y, -halfDimension.z,  //3

		-halfDimension.x, halfDimension.y, halfDimension.z, //4
		halfDimension.x, halfDimension.y, halfDimension.z, //5
		halfDimension.x, halfDimension.y, -halfDimension.z,  //6
		-halfDimension.x, halfDimension.y, -halfDimension.z //7
	};
	std::vector<unsigned int> indices = {
		0, 1, 2, 3, 2, 0,
		4, 5, 6, 6, 7, 4,
		1, 2, 6, 6, 5, 1,
		2, 3, 7, 7, 6, 2,
		3, 0, 4, 4, 7, 3,
		0, 1, 5, 5, 4, 0
	};

	auto output = Model(name, shaderName, vertices, indices);
	return output;
}