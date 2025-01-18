#include "scene.hpp"
#include "fonts.hpp"
enum SHADER_TYPE {
	VERT, FRAG
};

struct RenderDebugData {
	int fpsCounter;
};

unsigned int loadAndCompileShader(std::string source, SHADER_TYPE type);
unsigned int loadShaders(std::string name);
void prepareModel(Model* model, Camera camera);
void renderModel(Model model, Camera camera);
void prepareScene(Scene* scene, Camera camera);
void renderScene(Scene scene, Camera camera);
Scene* initTestScene(int width, int height);
void renderTextLine(std::string text, FontRenderingData* fontRenderingData, float x, float y, float scale, glm::vec3 color, int width, int height);
void renderTextLines(std::vector<std::string> text, FontRenderingData* fontRenderingData, float x, float y, float scale, glm::vec3 color, int width, int height, int fontSize);
FontRenderingData setupDebugTextRendering(FT_Library ft, int fontSize);
void renderDebugText(FontRenderingData* frd, int width, int height, int fontSize, RenderDebugData renderData);
