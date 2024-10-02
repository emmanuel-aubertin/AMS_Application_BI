#include "../include/Electre.hpp"

Electre::Electre(std::vector<std::vector<int>> values, std::vector<float> weights, float concordanceThreshold) {
    this->values = values;
    this->weights = weights;

    int size = weights.size();
    std::vector<std::vector<float>> concordance(size, std::vector<float>(size, 0.0));
    std::vector<bool> nonDiscordance(size, true);

    // process values
    this->processMatrixes();
}

// Electre::~Electre() {
    
// }

void Electre::processMatrixes() {
    processConcordance();
    processNondiscordance();
}

void Electre::processConcordance() {
    int size = concordance.size();
    for (int y=0; y<size-1; y++)
        for (int x=y+1; x<size; x++) {
            int concordVal1 = 0; 
            int concordVal2 = 0;

            for (int criteriaIndex=0; criteriaIndex<weights.size(); criteriaIndex++) {
                int candidateVal1 = values[y][criteriaIndex]; 
                int candidateVal2 = values[x][criteriaIndex]; 

                if (candidateVal1 <= candidateVal2) 
                    concordVal1 += weights[criteriaIndex];

                if (candidateVal2 <= candidateVal1)
                    concordVal2 += weights[criteriaIndex];
            }

            concordance[y][x] = concordVal1;
            concordance[x][y] = concordVal2;
        }
}

void Electre::processNondiscordance() {
    int size = concordance.size();
    for (int y=0; y<size; y++) {
        for (int x=0; x<size; x++) {
            if (x == y)
                continue;

            
        }
    }
}




std::vector<int> Electre::getKernel() {
    return kernel;
}