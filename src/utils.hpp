#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cassert>
#include <sstream>
std::string readFileContent(std::string filePath);
std::vector<std::string> getFileLines(std::string filePath);
std::vector<std::string> splitStringBySpaces(const std::string& input);
std::vector<std::string> splitStringByChar(const std::string& input, char delimiter);