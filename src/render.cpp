#include "render.hpp"

unsigned int loadAndCompileShader(std::string source, SHADER_TYPE type) {
	unsigned int currentShader = glCreateShader(type == VERT? GL_VERTEX_SHADER: type == FRAG? GL_FRAGMENT_SHADER: 0);
	assert(currentShader != 0);
	const GLchar* currentShaderCStr = source.c_str();
	glShaderSource(currentShader, 1, &currentShaderCStr, NULL);
	glCompileShader(currentShader);
	int success;
	char infoLog[512];
	glGetShaderiv(currentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(currentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return currentShader;
}

unsigned int loadShaders(std::string name) {
	std::string vertexShaderSource = readFileContent(std::string(SHADER_LOCATION_PREFIX) + name +  ".vert");
	std::string fragShaderSource = readFileContent(std::string(SHADER_LOCATION_PREFIX) + name + ".frag");
	auto vertexShader = loadAndCompileShader(vertexShaderSource, VERT);
	auto fragmentShader = loadAndCompileShader(fragShaderSource, FRAG);


	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

void prepareModel(Model* model, Camera camera) {
	model->shaderProgram = loadShaders(model->shaderName);
	glGenVertexArrays(1, &model->VAO);
	glGenBuffers(1, &model->VBO);
	glGenBuffers(1, &model->EBO);

	glBindVertexArray(model->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, model->getVertexPositions().size() * sizeof(float), model->getVertexPositions().data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indices.size() * sizeof(unsigned int), model->indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void renderModel(Model model, Camera camera) { //ran every frame
	glUseProgram(model.shaderProgram);
	glBindVertexArray(model.VAO);

	int modelLoc = glGetUniformLocation(model.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model.getGlobalViewModel()));

	int projectionLoc = glGetUniformLocation(model.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));

	int viewLoc = glGetUniformLocation(model.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view_matrix));

	int colourLoc = glGetUniformLocation(model.shaderProgram, "vertexColor");
    glUniform4fv(colourLoc, 1, glm::value_ptr(model.colour));

	glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void prepareScene(Scene* scene, Camera camera) {
	for (size_t i = 0; i < scene->models.size(); ++i) {
		auto currentModel = &scene->models[i];
		prepareModel(currentModel, camera);
	}
}


void renderScene(Scene scene, Camera camera) {
	for (size_t i = 0; i < scene.models.size(); ++i) {
		auto currentModel = scene.models[i];
		renderModel(currentModel, camera);
	}
}

Scene* initTestScene(int width, int height) {
	auto scene = new Scene();
	scene->name = "test room";
	float roomWidth = 20.0f;
	float roomHeight = 15.0f;

	auto floor = generateCube("test", "basic", glm::vec3(roomWidth, 0.2f, roomWidth));
	floor.setPosition(glm::vec3(0.0f, -.1f, 0.0f));
	floor.colour = glm::vec4(0.2f, 0.8f, 0.2f, 1.0f); //  green 

	auto wallLeft = generateCube("test", "basic", glm::vec3(0.2f, roomHeight, roomWidth));
	wallLeft.setPosition(glm::vec3(-roomWidth / 2, roomHeight/2, 0.0f));
	wallLeft.colour = glm::vec4(0.8f, 0.1f, 0.1f, 1.0f); //  red 

	auto wallRight = generateCube("test", "basic", glm::vec3(0.2f, roomHeight, roomWidth));
	wallRight.setPosition(glm::vec3(roomWidth / 2, roomHeight / 2, 0.0f));
	wallRight.colour = glm::vec4(0.1f, 0.1f, 0.8f, 1.0f); // blue 

	auto wallBack = generateCube("test", "basic", glm::vec3(roomWidth, roomHeight, 0.2f));
	wallBack.setPosition(glm::vec3(0.0f, roomHeight / 2, -roomWidth / 2));
	wallBack.colour = glm::vec4(0.8f, 0.8f, 0.0f, 1.0f); //yellow 
	auto ball = generateSphere("test", "basic", glm::vec3(0.0f, 3.0f, 0.0f), 1.0f, 30, 20);
	ball.colour = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f); //yellow 

	auto monkey = importModel("asda", "basic", "suzanne.obj");
	monkey.colour = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f); //yellow 
	monkey.setPosition(glm::vec3(3.0f, 2.0f, 0.0f));


	scene->models.push_back(floor);
	scene->models.push_back(wallLeft);
	scene->models.push_back(wallRight);
	scene->models.push_back(wallBack);
	scene->models.push_back(ball);
	scene->models.push_back(monkey);
	auto camera = Camera(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, -1.0f, -7.0f), width, height);
	scene->cameras.push_back(camera);
	prepareScene(scene, scene->cameras[0]);

	return scene;
}


void renderTextLine(std::string text, FontRenderingData* fontRenderingData, float x, float y, float scale, glm::vec3 color, int width, int height){
	glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

	glUseProgram(fontRenderingData->shaderProgram);
	auto projectionLoc = glGetUniformLocation(fontRenderingData->shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glUniform3f(glGetUniformLocation(fontRenderingData->shaderProgram, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(fontRenderingData->VAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = fontRenderingData->characters[*c];

		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		//glUniform1i(glGetUniformLocation(fontRenderingData->shaderProgram, "text"), 0);
		glBindBuffer(GL_ARRAY_BUFFER, fontRenderingData->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.advance >> 6) * scale; 
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void renderTextLines(std::vector<std::string> texts, FontRenderingData* fontRenderingData, float x, float y, float scale, glm::vec3 color, int width, int height, int fontSize) {
	const auto gap = 5;
	for (size_t i = 0; i < texts.size(); i++) {
		renderTextLine(texts[i], fontRenderingData, x, y, scale, color, width, height);
		y -= fontSize + gap;
	}
}

FontRenderingData setupDebugTextRendering(FT_Library ft, int fontSize) {
	FontRenderingData frd = loadFont(ft, std::string(FONT_LOCATION_PREFIX) + std::string("BitterPro-Regular.ttf"), fontSize);
	FT_Done_FreeType(ft);
	frd.shaderProgram = loadShaders("text");
	prepareTextRendering(&frd);
	return frd;
}

void renderDebugText(FontRenderingData* frd, int width, int height, int fontSize, RenderDebugData debugData) {
	std::vector<std::string> texts = {
		"FPS: " + std::to_string(debugData.fpsCounter),
		"Second sentence"
	};
	renderTextLines(texts, frd, 0.0f, (float)height - fontSize, 1.0f, glm::vec3(0.5f, 0.0f, 0.0f), width, height, fontSize);
}