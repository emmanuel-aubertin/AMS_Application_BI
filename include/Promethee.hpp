#ifndef PROMETHEE_METHODS
#define PROMETHEE_METHODS

#include <vector>
#include <iostream>
#include <iomanip>
#include "Algo.hpp"

class Promethee : public Algo
{
private:
    std::vector<std::vector<float>> data;                          // Alternatives and criteria
    std::vector<float> weights;                                    // Weights for each criterion
    std::vector<std::vector<float>> multicriteriaPreferenceMatrix; // Multicriteria Preference Matrix
    std::vector<float> flows;                                      // Diff btw φ+ & φ-
    std::vector<float> positiveFlow;                               // Positive outranking flow (φ+)
    std::vector<float> negativeFlow;                               // Negative outranking flow (φ−)

    float calculatePreference(float value1, float value2);

public:
    Promethee(std::vector<std::vector<float>> data, std::vector<float> weights);
    Promethee() : Algo("Promethee", "p", "For Promethee algo", "in development") {};
    void calculatePreferenceMatrix();
    void printPreferenceMatrix();

    void calculateFlows();
    void printFlows();
    void printLatexOutput();
};

#endif