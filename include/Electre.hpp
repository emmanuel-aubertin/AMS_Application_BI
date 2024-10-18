#ifndef ELECTRE_VETO_METHODS
#define ELECTRE_VETO_METHODS

#include <vector>
#include <cmath>
#include <iostream>

class Electre {
    private:
        std::vector<std::vector<float>> values;
        std::vector<float> weights;
        std::vector<float> vetos;
        std::vector<float> preferenceThresholds;
        float concordanceThreshold;

        std::vector<std::vector<float>> concordance;
        std::vector<std::vector<bool>> nonDiscordance;
        std::vector<bool> kernel;
        std::vector<std::vector<bool>> dominanceMatrix;
    
        void processConcordance();
        void processNondiscordance();
        void processDominance();
        void processKernel();
        std::vector<std::vector<int>> getCycles();
        std::vector<std::vector<int>> getSuccessorCycles(int candidate, std::vector<int> visitedChilds);
        bool hasSameElements(std::vector<int> vec1, std::vector<int> vec2);

    public:
        Electre(
            std::vector<std::vector<float>> values, 
            std::vector<float> weights, 
            std::vector<float> vetos, 
            float concordanceThreshold
        );

        Electre(
            std::vector<std::vector<float>> values, 
            std::vector<float> weights, 
            std::vector<float> vetos, 
            std::vector<float> preferenceThresholds, 
            float concordanceThreshold
        );
        
        void processMatrixes();
        std::vector<bool> getKernel();
};

#endif