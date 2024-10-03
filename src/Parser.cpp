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

/**
 * @brief Splits a string into a vector of floating-point numbers.
 *
 *  Uses the specified delimiter to separate values in the input string and converts each
 *  separated value to a floating-point number.
 *
 * @param s The input string to be split.
 * @return A vector of floating-point numbers representing the split values.
 */
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
    std::vector<std::vector<u_int16_t>> result;
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
    parsedFile = result;
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
            result.push_back(splitFloat(line));
        }
        file.close();
    }
    parsedWeightFile = result;
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
std::vector<std::vector<u_int16_t>> Parser::getParsedFile()
{
    return parsedFile;
}
