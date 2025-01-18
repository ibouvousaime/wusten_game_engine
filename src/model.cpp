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

void Model::unloadModel() {
	if (this->prepared) {
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->VAO);
		glDeleteBuffers(1, &this->EBO);
		glDeleteProgram(this->shaderProgram);
		this->prepared = false;
	}
}

Model::~Model() {
	this->unloadModel();
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
	model = glm::rotate(model, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
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

Model generateSphere(std::string name, std::string shaderName, glm::vec3 position, float radius, int stackCount, int sectorCount) {
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	const auto PI = glm::pi<float>();
	float x, y, z, xy;                             
	float nx, ny, nz, lengthInv = 1.0f / radius;   
	float s, t;

	float sectorStep = 2.0f * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        
		xy = radius * cosf(stackAngle);             
		z = radius * sinf(stackAngle);              

		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;          

			x = xy * cosf(sectorAngle);             
			y = xy * sinf(sectorAngle);             
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}
	}
	std::vector<unsigned int> indices;
	std::vector<int> lineIndices;
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     
		k2 = k1 + sectorCount + 1;      

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}


			lineIndices.push_back(k1);
			lineIndices.push_back(k2);
			if (i != 0)  
			{
				lineIndices.push_back(k1);
				lineIndices.push_back(k1 + 1);
			}
		}
	}
	auto output = Model(name, shaderName, vertices, indices);
	output.setPosition(position);
	return output;
}
