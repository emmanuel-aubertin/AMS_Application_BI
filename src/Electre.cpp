#include "../include/Electre.hpp"

Electre::Electre(std::vector<std::vector<int>> values, std::vector<float> weights, std::vector<float> vetos,float concordanceThreshold) {
    this->values = values;
    this->weights = weights;
    this->vetos = vetos;

    int size = values.size();
    concordance = std::vector<std::vector<float>>(size, std::vector<float>(size, 0.0));
    nonDiscordance = std::vector<std::vector<bool>>(size, std::vector<bool>(size, true));

    // process values
    this->processMatrixes();
}

void Electre::processMatrixes() {
    processConcordance();
    processNondiscordance();
}

void Electre::processConcordance() {
    int size = concordance.size();
    for (int y=0; y<size-1; y++) {
        for (int x=y+1; x<size; x++) {
            int concordVal1 = 0; 
            int concordVal2 = 0;

            for (int criterium=0; criterium<weights.size(); criterium++) {
                int candidateVal1 = values[y][criterium]; 
                int candidateVal2 = values[x][criterium]; 

                if (candidateVal1 <= candidateVal2) 
                    concordVal1 += weights[criterium];

                if (candidateVal2 <= candidateVal1)
                    concordVal2 += weights[criterium];
            }

            concordance[y][x] = concordVal1;
            concordance[x][y] = concordVal2;
        }
    }
    
    std::cout << "Concordance matrix: " << std::endl;
    for (std::vector<float> line : concordance) {
        for (float val : line) 
            std::cout << val << "\t";
        std::cout << std::endl;
    }
}

void Electre::processNondiscordance() {
    int size = nonDiscordance.size();
    for (int y=0; y<size; y++) {
        for (int x=0; x<size; x++) {
            if (x == y) {
                nonDiscordance[x][y] = false;
                continue;
            }

            for (int criterium=0; criterium<weights.size(); criterium++) {
                int candidateVal1 = values[y][criterium]; 
                int candidateVal2 = values[x][criterium]; 
                float veto = vetos[criterium];

                if (candidateVal1 - candidateVal2 > veto) {
                    nonDiscordance[y][x] = false;
                    break;
                }
            }
        }
    }

    std::cout << "Non discordance matrix: " << std::endl;
    for (std::vector<bool> line : nonDiscordance) {
        for (bool val : line) 
            std::cout << val << "\t";
        std::cout << std::endl;
    }
}

std::vector<int> Electre::getKernel() {
    return kernel;
}