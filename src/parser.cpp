#include "parser.hpp"
#include <fstream>
#include <sstream>

Parser::Parser(char delimiter) : m_delimiter(delimiter) {}

std::vector<int> Parser::split(const std::string& s) const {
    std::vector<int> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, m_delimiter)) {
        tokens.push_back(std::stoi(token));
    }
    return tokens;
}

std::vector<std::vector<int>> Parser::parseFile(const std::string& filename) const {
    std::vector<std::vector<int>> result;
    std::string line;
    std::ifstream file(filename);
    if (file.is_open()) {
        while (getline(file, line)) {
            result.push_back(split(line));
        }
        file.close();
    }
    return result;
}
