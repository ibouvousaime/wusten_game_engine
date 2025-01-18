#include <ft2build.h>
#include FT_FREETYPE_H  
#include <iostream>
#include <glm/glm.hpp>
#include <map>
#include <glad/glad.h>

struct Character {
    unsigned int textureID;
    glm::ivec2   size;
    glm::ivec2   bearing;
    unsigned int advance;
};

struct FontRenderingData {
    unsigned int VAO;
    unsigned int VBO;
    std::map<char, Character> characters;
    unsigned int shaderProgram;
};

FontRenderingData loadFont(FT_Library ft, std::string fontPath, int fontSize);
void prepareTextRendering(FontRenderingData* fontRenderingData);