#include "model.hpp"


Vertex::Vertex(glm::vec3 position) {
	this->position = position;
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal) {
	this->position = position;
	this->normal = normal;
}


Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 texCoords) {
	this->position = position;
	this->normal = normal;
	this->texCoords = texCoords;
}


Model::Model(std::string name, std::string shaderName, std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
	this->name = name;
	this->vertices = vertices;
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
	std::vector<float> output;
	for (Vertex vertex : this->vertices) {
		output.push_back(vertex.position.x);
		output.push_back(vertex.position.y);
		output.push_back(vertex.position.z);
	}
	return output;
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
	std::vector<Vertex> vertices = {
		 Vertex(glm::vec3(0.5f * dimensions.x,  0.5f * dimensions.y,  0.5f * dimensions.z)),
		 Vertex(glm::vec3(-0.5f * dimensions.x,  0.5f * dimensions.y,  0.5f * dimensions.z)),
		 Vertex(glm::vec3(-0.5f * dimensions.x, -0.5f * dimensions.y,  0.5f * dimensions.z)),
		 Vertex(glm::vec3(0.5f * dimensions.x, -0.5f * dimensions.y,  0.5f * dimensions.z)),
		 Vertex(glm::vec3(0.5f * dimensions.x,  0.5f * dimensions.y, -0.5f * dimensions.z)),
		 Vertex(glm::vec3(-0.5f * dimensions.x,  0.5f * dimensions.y, -0.5f * dimensions.z)),
		 Vertex(glm::vec3(-0.5f * dimensions.x, -0.5f * dimensions.y, -0.5f * dimensions.z)),
		 Vertex(glm::vec3(0.5f * dimensions.x, -0.5f * dimensions.y, -0.5f * dimensions.z)),
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
	std::vector<Vertex> vertices;
	const auto PI = glm::pi<float>();
	float z, xy;                             
	float lengthInv = 1.0f / radius;   

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
			Vertex currentVertex = Vertex();
			currentVertex.position.x = xy * cosf(sectorAngle);             
			currentVertex.position.y = xy * sinf(sectorAngle);
			currentVertex.position.z = z;
			currentVertex.normal.x = currentVertex.position.x * lengthInv;
			currentVertex.normal.y = currentVertex.position.y * lengthInv;
			currentVertex.normal.z = currentVertex.position.z * lengthInv;

			currentVertex.texCoords.s = (float)j / sectorCount;
			currentVertex.texCoords.t = (float)i / stackCount;
			vertices.push_back(currentVertex);
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

struct objFaceVertex {
	int vertexIndex;
	int normalIndex;
	int textCoordsIndex;
};

Model importModel(std::string name, std::string shaderName, std::string filename) {
	std::vector<std::string> modelFileLines = getFileLines(std::string(MODEL_LOCATION_PREFIX) + filename);
	std::vector<glm::vec3> positions = {};
	std::vector<glm::vec3> normals = {};
	std::vector<glm::vec2> texCoords = {};
	std::vector<std::vector<objFaceVertex >> faces = {};
	for (std::string line : modelFileLines) {
		std::string lineToParse = "";
		if (line.rfind("#") == 0) {
		}
		else if (line.rfind("v ") == 0) 
		{
			lineToParse = line.substr(2, line.length() - 1);
			glm::vec3 position = glm::vec3();
			auto positionsString = splitStringBySpaces(lineToParse);
			position.x = std::atof(positionsString[0].c_str());
			position.y = std::atof(positionsString[1].c_str());
			position.z = std::atof(positionsString[2].c_str());
			positions.push_back(position);
		}
		else if (line.rfind("vt") == 0)
		{
			lineToParse = line.substr(3, line.length() - 1);
			glm::vec2 texCoord = glm::vec2();
			auto texCoordsString = splitStringBySpaces(lineToParse);
			texCoord.s = std::atof(texCoordsString[0].c_str());
			texCoord.t = std::atof(texCoordsString[1].c_str());
			texCoords.push_back(texCoord);
		}
		else if (line.rfind("vn") == 0)
		{
			lineToParse = line.substr(3, line.length() - 1);
			glm::vec3 normal = glm::vec3();
			auto texCoordsString = splitStringBySpaces(lineToParse);
			normal.x = std::atof(texCoordsString[0].c_str());
			normal.y = std::atof(texCoordsString[1].c_str());
			normal.z = std::atof(texCoordsString[2].c_str());
			normals.push_back(normal);
		}
		else if (line.rfind("f ") == 0)
		{
			lineToParse = line.substr(2, line.length() - 1);
			std::vector<objFaceVertex> currentFace = {};
			auto faceVertices = splitStringBySpaces(lineToParse);
			for (auto vertex : faceVertices) {
				auto currentVertexStr = splitStringByChar(vertex, '/');
				auto vertex = objFaceVertex();
				vertex.vertexIndex = std::atoi(currentVertexStr[0].c_str());
				vertex.normalIndex = std::atoi(currentVertexStr[0].c_str());
				vertex.textCoordsIndex = std::atoi(currentVertexStr[0].c_str());
				currentFace.push_back(vertex);
			}
			faces.push_back(currentFace);
		}
	}
	std::vector<Vertex> vertices;
	vertices.resize(positions.size());
	std::vector<unsigned int> indices;

	for (auto face : faces) {
		for (auto vert : face) {
			auto currentPositionIndex = vert.vertexIndex - 1;
			auto currentNormalIndex = vert.normalIndex - 1;
			auto currentTextCoordsIndex = vert.textCoordsIndex - 1;
			if (!vertices[currentPositionIndex].initialized) {
				vertices[currentPositionIndex].position = positions[currentPositionIndex];
				vertices[currentPositionIndex].normal = normals[currentNormalIndex];
				vertices[currentPositionIndex].texCoords = normals[currentTextCoordsIndex];

				vertices[currentPositionIndex].initialized = true;
			}
			indices.push_back(currentPositionIndex);
		}
	}
	Model output = Model("Test", shaderName, vertices, indices);
	return output;
}