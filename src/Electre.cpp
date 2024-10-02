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
    for (std::vector<int> line : values) {
        for (int el : line)
            std::cout << el << " ";
        std::cout << std::endl;
    }
}

void Electre::processNondiscordance() {

}




std::vector<int> Electre::getKernel() {
    return kernel;
}