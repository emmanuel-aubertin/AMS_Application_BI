#ifndef ELECTRE_METHODS
#define ELECTRE_METHODS

#include <vector>
#include <iostream>

class Electre {
    private:
        std::vector<std::vector<int>> values;
        std::vector<float> weights;
        std::vector<float> vetos;

        std::vector<std::vector<float>> concordance;
        std::vector<std::vector<bool>> nonDiscordance;
        std::vector<int> kernel;
    
        void processMatrixes();
        void processConcordance();
        void processNondiscordance();
    public:
        Electre(std::vector<std::vector<int>> values, std::vector<float> weights, std::vector<float> vetos, float concordanceThreshold);
        Electre(std::vector<std::vector<int>> values, std::vector<float> weights, float concordanceThreshold);
        
        std::vector<int> getKernel();
};

#endif