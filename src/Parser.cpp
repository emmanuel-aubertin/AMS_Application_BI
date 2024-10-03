#include "../include/Parser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

Parser::Parser(char delimiter) : m_delimiter(delimiter) {}

std::vector<u_int16_t> Parser::split(const std::string &s)
{
    std::vector<u_int16_t> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, m_delimiter))
    {
        tokens.push_back(std::stoi(token));
    }
    return tokens;
}

std::vector<float> Parser::splitFloat(const std::string &s)
{
    std::vector<float> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, m_delimiter))
    {
        tokens.push_back(std::stof(token));
    }
    return tokens;
}

void Parser::parseFile(const std::string &filename)
{
    std::vector<std::vector<u_int16_t>> result;
    std::string line;
    std::ifstream file(filename);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            result.push_back(split(line));
        }
        file.close();
    }
    parsedFile = result;
}

void Parser::parseWeightFile(const std::string &filename)
{
    std::vector<std::vector<float>> result;
    std::string line;
    std::ifstream file(filename);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            result.push_back(splitFloat(line));
        }
        file.close();
    }
    parsedWeightFile = result;
}

void Parser::print() const
{
    for (const auto &row : parsedFile)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            std::cout << row[i];
            if (i != row.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<u_int16_t>> Parser::getParsedFile()
{
    return parsedFile;
}