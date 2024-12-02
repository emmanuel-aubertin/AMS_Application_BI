#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include "OptimizationType.hpp"

class Parser
{
public:
    Parser(char delimiter = ',');
    std::vector<float> split(const std::string &s);
    std::vector<OptimizationType> splitOptimization(const std::string &s);

    void parseFile(const std::string &filename);
    void parseWeightFile(const std::string &filename);
    void parseVetosFile(const std::string &filename);
    void parsePreferencesFile(const std::string &filename);
    void parseOptimizationsFile(const std::string &filename);
    void parseConcordanceThresholdFile(const std::string &filename);

    void print() const;
    std::vector<std::vector<float>> getParsedFile();
    std::vector<float> getParsedWeight();
    std::vector<float> getParsedVetosFile();
    std::vector<float> getParsedPreferencesFile();
    std::vector<OptimizationType> getParsedOptimizationsFile();
    float getParsedConcordanceThresholdFile();

private:
    char m_delimiter;
    std::vector<std::vector<float>> parsedFile;
    std::vector<float> parsedWeightFile;
    std::vector<float> parsedVetosFile;
    std::vector<float> parsedPreferencesFile;
    std::vector<OptimizationType> parsedOptimizationsFile;
    float parsedConcordanceThresholdFile;
};

#endif // PARSER_H