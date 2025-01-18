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

std::vector<std::string> getFileLines(std::string filePath) {
    std::ifstream file(filePath);
    std::vector<std::string> output = {};
    if (!file.is_open()) {
        return {};
    }
    std::string line;
    while (getline(file, line)) {
        output.push_back(line);
    }
    file.close();
    return output;
}

std::vector<std::string> splitStringBySpaces(const std::string& input) {
    std::vector<std::string> result;
    std::istringstream stream(input);
    std::string word;

    while (stream >> word) {
        result.push_back(word);
    }

    return result;
}

std::vector<std::string> splitStringByChar(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}