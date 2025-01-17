#pragma once
#include <glad/glad.h>
#include "model.hpp"
#include "utils.hpp"

#include "constants.hpp"
#include <iostream>
#include <cassert>
#include "camera.hpp"
struct Scene {
	std::string name;
	std::vector<Model> models;
	std::vector<Camera> cameras;

};

