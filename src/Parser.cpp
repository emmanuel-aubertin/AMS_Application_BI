#include "../include/Parser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

/**
 * @brief Constructs a new Parser object.
 *
 *  Initializes the parser with a specified delimiter character.
 *
 * @param delimiter The character used to separate values in input strings.
 */
Parser::Parser(char delimiter) : m_delimiter(delimiter) {}

/**
 * @brief Splits a string into a vector of unsigned 16-bit integers.
 *
 *  Uses the specified delimiter to separate values in the input string and converts each
 *  separated value to an unsigned 16-bit integer.
 *
 * @param s The input string to be split.
 * @return A vector of unsigned 16-bit integers representing the split values.
 */
std::vector<float> Parser::split(const std::string &s)
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

std::vector<OptimizationType> Parser::splitOptimization(const std::string &s)
{
    std::vector<OptimizationType> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, m_delimiter))
    {
        if (token == "MIN") 
        {
            tokens.push_back(MIN);
        }
        else if (token == "MAX")
        {
            tokens.push_back(MAX);
        }
        else
        {
            std::cerr << "Unrecognized optimization type." << std::endl;
        }
    }
    return tokens;
}

/**
 * @brief Parses a file containing data separated by a delimiter.
 *
 *  Reads the contents of the specified file, splits each line into a vector of unsigned 16-bit
 *  integers based on the delimiter, and stores the resulting vectors in the `parsedFile`
 *  member variable.
 *
 * @param filename The path to the file to be parsed.
 */
void Parser::parseFile(const std::string &filename)
{
    std::vector<std::vector<float>> result;
    std::string line;
    std::ifstream file(filename);
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            result.push_back(split(line));
        }
        file.close();
    }
    else
    {
        std::cerr << "Error: " << filename << std::endl;
    }

    this->parsedFile = result;
}

/**
 * @brief Parses a file containing weights separated by a delimiter.
 *
 *  Reads the contents of the specified file, splits each line into a vector of floating-point
 *  numbers based on the delimiter, and stores the resulting vectors in the
 *  `parsedWeightFile` member variable.
 *
 * @param filename The path to the file to be parsed.
 */
void Parser::parseWeightFile(const std::string &filename)
{
    std::vector<std::vector<float>> result;
    std::string line;
    std::ifstream file(filename);
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            result.push_back(split(line));
        }
        file.close();
    }
    float total_weight = 0;
    for(int i = 0; i < result[0].size(); i++) {
        total_weight += result[0][i];
    }
    for(int i = 0; i < result[0].size(); i++) {
        result[0][i] = result[0][i] / total_weight;
    }
    this->parsedWeightFile = result[0];
}


void Parser::parseVetosFile(const std::string &filename) 
{
    std::vector<std::vector<float>> result;
    std::string line;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: " << filename << std::endl;
    }
    
    while (std::getline(file, line))
    {
        result.push_back(split(line));
    }
    file.close();

    this->parsedVetosFile = result[0];
}

void Parser::parsePreferencesFile(const std::string &filename) 
{
    std::vector<std::vector<float>> result;
    std::string line;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: " << filename << std::endl;
    }
    
    while (std::getline(file, line))
    {
        result.push_back(split(line));
    }
    file.close();

    this->parsedPreferencesFile = result[0];
}

void Parser::parseOptimizationsFile(const std::string &filename) 
{
    std::vector<std::vector<OptimizationType>> result;
    std::string line;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: " << filename << std::endl;
    }
    
    while (std::getline(file, line))
    {
        result.push_back(splitOptimization(line));
    }
    file.close();

    this->parsedOptimizationsFile = result[0];
}

void Parser::parseConcordanceThresholdFile(const std::string &filename) 
{
    std::vector<float> result;
    std::string line;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: " << filename << std::endl;
    }
    
    while (std::getline(file, line))
    {
        result.push_back(stof(line));
    }
    file.close();

    this->parsedConcordanceThresholdFile = result[0];
}


/**
 * @brief Prints the parsed data to the console.
 *
 *  Iterates through the `parsedFile` member variable and prints each row of data,
 *  separated by commas.
 */
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

/**
 * @brief Returns the parsed file data.
 *
 * @return The `parsedFile` member variable containing the parsed data as a vector of vectors.
 */
std::vector<std::vector<float>> Parser::getParsedFile()
{
    return parsedFile;
}

std::vector<float> Parser::getParsedWeight()
{
    return parsedWeightFile;
}

std::vector<float> Parser::getParsedVetosFile() 
{
    return parsedVetosFile;
}

std::vector<float> Parser::getParsedPreferencesFile() 
{
    return parsedPreferencesFile;
}

std::vector<OptimizationType> Parser::getParsedOptimizationsFile() 
{
    return parsedOptimizationsFile;
}

float Parser::getParsedConcordanceThresholdFile() 
{
    return parsedConcordanceThresholdFile;
}
