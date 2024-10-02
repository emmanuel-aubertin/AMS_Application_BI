#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

class Parser
{
public:
    Parser(char delimiter = ',');
    std::vector<int> split(const std::string &s);
    void parseFile(const std::string &filename);
    void print() const;
    std::vector<std::vector<int>> getParsedFile();

private:
    char m_delimiter;
    std::vector<std::vector<int>> parsedFile;
};

#endif // PARSER_H