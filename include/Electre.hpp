#ifndef ELECTRE_VETO_METHODS
#define ELECTRE_VETO_METHODS

#include <vector>
#include <array>
#include <cmath>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "OptimizationType.hpp"
#include "Algo.hpp"

class Electre : public Algo
{
private:
    std::vector<std::vector<float>> values;
    std::vector<float> vetos;
    std::vector<float> preferenceThresholds;
    std::vector<OptimizationType> optimizations;
    float concordanceThreshold;

    int nbCandidates;
    int nbCriteria;

    std::vector<std::vector<float>> concordance;
    std::vector<std::vector<bool>> nonDiscordance;
    std::vector<bool> kernel;
    std::vector<std::vector<bool>> dominance;

    void processConcordance();
    void processNondiscordance();
    void processDominance();
    void processKernel();
    std::vector<std::vector<int>> getCycles();
    std::vector<std::vector<int>> getSuccessorCycles(int candidate, std::vector<int> visitedChilds);
    bool hasSameElements(std::vector<int> vec1, std::vector<int> vec2);
    void deleteCycles(std::vector<std::vector<int>> cycles);

public:
    Electre(
        std::vector<std::vector<float>> values,
        std::vector<float> weights,
        std::vector<float> vetos,
        std::vector<OptimizationType> optimizations,
        float concordanceThreshold);

    Electre(
        std::vector<std::vector<float>> values,
        std::vector<float> weights,
        std::vector<float> vetos,
        std::vector<float> preferenceThresholds,
        std::vector<OptimizationType> optimizations,
        float concordanceThreshold);

    Electre() : Algo("Electre", "e", "For Electre algo", "in development") {};

    // Setters
    void setData(const std::vector<std::vector<float>> &newData) override { values = newData; nbCandidates = values.size();}
    void setWeights(const std::vector<float> &newWeights) override { weights = newWeights;  nbCriteria = weights.size();}
    void setVetos(const std::vector<float> &newVetos);
    void setPreferenceThresholds(const std::vector<float> &newThresholds);
    void setOptimizations(const std::vector<OptimizationType> &newOptimizations);
    void setConcordanceThreshold(float newThreshold);
    void setConcordanceMatrix(const std::vector<std::vector<float>> &newConcordance);
    void setNonDiscordanceMatrix(const std::vector<std::vector<bool>> &newNonDiscordance);
    void setDominanceMatrix(const std::vector<std::vector<bool>> &newDominance);
    void setKernel(const std::vector<bool> &newKernel);

    void processMatrixes();
    std::vector<bool> getKernel();
    void printVectors();
    void run() override;
    virtual int save(std::string dirPath) override;
};

#endif
