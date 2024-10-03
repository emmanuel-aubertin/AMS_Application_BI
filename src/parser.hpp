#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

class Parser
{
public:
    Parser(char delimiter = ',');
    std::vector<u_int16_t> split(const std::string &s);
    std::vector<float> splitFloat(const std::string &s);
    
    void parseFile(const std::string &filename);
    void print() const;
    std::vector<std::vector<u_int16_t>> getParsedFile();
    void parseWeightFile(const std::string &filename);

private:
    char m_delimiter;
    std::vector<std::vector<u_int16_t>> parsedFile;
    std::vector<std::vector<float>> parsedWeightFile;
};

#endif // PARSER_H