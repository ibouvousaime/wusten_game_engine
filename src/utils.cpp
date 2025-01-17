#include "utils.hpp"

std::string readFileContent(std::string filePath) {
	std::ifstream file(filePath);
    std::string output = "";
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    std::string line;
    while (getline(file, line)) {
        output += line + "\n";
    }
    file.close();
    std::cout << output << std::endl;
    return output;
}
