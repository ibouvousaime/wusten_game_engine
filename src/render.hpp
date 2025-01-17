
#include "scene.hpp"

enum SHADER_TYPE {
	VERT, FRAG
};

unsigned int loadAndCompileShader(std::string source, SHADER_TYPE type);
unsigned int loadShaders(std::string name);
void prepareModel(Model* model, Camera camera);
void renderModel(Model model, Camera camera);
void prepareScene(Scene* scene, Camera* camera);
void renderScene(Scene scene, Camera camera);
Scene* initTestScene();