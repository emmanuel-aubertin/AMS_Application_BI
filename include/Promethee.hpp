#ifndef PROMETHEE_METHODS
#define PROMETHEE_METHODS

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include "OptimizationType.hpp"
#include "Algo.hpp"

class Promethee : public Algo
{
private:
    std::vector<std::vector<float>> multicriteriaPreferenceMatrix; // Multicriteria Preference Matrix
    std::vector<float> flows;                                      // Diff btw φ+ & φ-
    std::vector<float> positiveFlow;                               // Positive outranking flow (φ+)
    std::vector<float> negativeFlow;                               // Negative outranking flow (φ−)
    std::vector<int> bestAlternativesPositive;
    std::vector<int> bestAlternativesNegative;
    std::vector<int> bestAlternativesOverall;

    float calculatePreference(float value1, float value2);
    std::vector<int> calculatePosition(std::vector<float> valuesVec, OptimizationType order);

public:
    Promethee(std::vector<std::vector<float>> data, std::vector<float> weights);
    Promethee() : Algo("Promethee", "p", "For Promethee algo", "in development") {};

    // Setters
    void setMulticriteriaPreferenceMatrix(const std::vector<std::vector<float>> &newMatrix) { multicriteriaPreferenceMatrix = newMatrix; }
    void setFlows(const std::vector<float> &newFlows) { flows = newFlows; }
    void setPositiveFlow(const std::vector<float> &newPositiveFlow) { positiveFlow = newPositiveFlow; }
    void setNegativeFlow(const std::vector<float> &newNegativeFlow) { negativeFlow = newNegativeFlow; }

    void calculatePreferenceMatrix();
    void printPreferenceMatrix();
    void calculateFlows();
    void calculateBestCandidates();
    void printFlows();
    void printLatexOutput();
    void run() override;
    virtual int save(std::string dirPath) override;
};

#endif
