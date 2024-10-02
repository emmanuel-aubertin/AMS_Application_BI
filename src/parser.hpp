#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

class Parser {
public:
    Parser(char delimiter = ',');
    std::vector<int> split(const std::string& s) const;
    std::vector<std::vector<int>> parseFile(const std::string& filename) const;

private:
    char m_delimiter;
};

#endif // PARSER_H