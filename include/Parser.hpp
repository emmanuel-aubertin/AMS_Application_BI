#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

class Parser
{
public:
    Parser(char delimiter = ',');
    std::vector<float> split(const std::string &s);
    std::vector<float> splitFloat(const std::string &s);

    void parseFile(const std::string &filename);
    void print() const;
    std::vector<std::vector<float>> getParsedFile();
    std::vector<float> getParsedWeight();
    void parseWeightFile(const std::string &filename);

private:
    char m_delimiter;
    std::vector<std::vector<float>> parsedFile;
    std::vector<float> parsedWeightFile;
};

#endif // PARSER_H