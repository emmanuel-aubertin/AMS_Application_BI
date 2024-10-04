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
    
        void processConcordance();
        void processNondiscordance();
        void processKernel();

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