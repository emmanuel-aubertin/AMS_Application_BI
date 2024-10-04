#include "../include/Electre.hpp"

/**
 * @brief Constructs an Electre object.
 *
 * Initializes the Electre object with the provided values, weights, and concordance threshold.
 *
 * @param values A 2D vector representing the decision matrix, where each inner vector represents a row.
 * @param weights A vector of floating-point numbers representing the weights for each attribute.
 * @param vetos A vector of floating-point numbers representing the veto thresholds for each attribute.
 * @param concordanceThreshold A floating-point number representing the threshold for concordance.
 */
Electre::Electre(
    std::vector<std::vector<float>> values,
    std::vector<float> weights,
    std::vector<float> vetos,
    float concordanceThreshold
)
{
    this->values = values;
    this->weights = weights;
    this->vetos = vetos;
    
    int nbCandidates = values.size();
    int nbVariables = weights.size();
    preferenceThresholds = std::vector<float>(nbVariables, 0.0);

    concordance = std::vector<std::vector<float>>(nbCandidates, std::vector<float>(nbCandidates, 0.0));
    nonDiscordance = std::vector<std::vector<bool>>(nbCandidates, std::vector<bool>(nbCandidates, true));
    kernel = std::vector<bool>(nbCandidates, true);
}

Electre::Electre(
    std::vector<std::vector<float>> values, 
    std::vector<float> weights, 
    std::vector<float> vetos, 
    std::vector<float> preferenceThresholds, 
    float concordanceThreshold
) : Electre::Electre(values, weights, vetos, concordanceThreshold) 
{
    this->preferenceThresholds = preferenceThresholds;
}


/**
 * @brief Processes the data to calculate the concordance and discordance matrices.
 */
void Electre::processMatrixes()
{
    processConcordance();
    processNondiscordance();
    processKernel();
}


/**
 * @brief Calculates the concordance matrix.
 *
 *  This method iterates through the values matrix and calculates the concordance values for each pair of attributes.
 *  The concordance value represents the degree to which two attributes agree on the relative preference of alternatives.
 */
void Electre::processConcordance()
{
    int size = concordance.size();
    for (int y = 0; y < size - 1; y++)
    {
        for (int x = y + 1; x < size; x++)
        {
            float concordVal1 = 0;
            float concordVal2 = 0;

            for (int criterium = 0; criterium < weights.size(); criterium++)
            {
                float candidateVal1 = values[y][criterium];
                float candidateVal2 = values[x][criterium];
                float threshold = preferenceThresholds[criterium];
                float val = weights[criterium];

                double diff = std::abs(candidateVal1 - candidateVal2);
                double coeff = 1 - std::min(1.0, diff/threshold);

                if (candidateVal1 == candidateVal2) {
                    concordVal1 += val;
                    concordVal2 += val;
                    continue;
                }

                // TODO: replace < with > if MAXIMISE
                if (candidateVal1 < candidateVal2) {
                    concordVal1 += val;
                    concordVal2 += coeff * val;
                    continue;
                }

                concordVal2 += val;
                concordVal1 += coeff * val;
            }

            concordance[y][x] = concordVal1;
            concordance[x][y] = concordVal2;
        }
    }
}
    
void Electre::processNondiscordance() {
    int size = nonDiscordance.size();
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            if (x == y)
            {
                nonDiscordance[x][y] = false;
                continue;
            }


            for (int criterium = 0; criterium < weights.size(); criterium++)
            {
                double candidateVal1 = values[y][criterium];
                double candidateVal2 = values[x][criterium];
                float veto = vetos[criterium];

                if (candidateVal1 - candidateVal2 > veto)
                {
                    nonDiscordance[y][x] = false;
                    break;
                }
            }
        }
    }

    std::cout << "Non discordance: " << std::endl;
    for (std::vector<bool> line : nonDiscordance) {
        for (bool val : line)
            std::cout << val << "\t";
        std::cout << std::endl;
    }
}

void Electre::processKernel() 
{
    // check columns for > concordanceThreshold and nonDiscordance == true
    int size = concordance.size();
    for (int y=0; y<size; y++) {
        for (int x=0; x<size; x++) {
            if (!kernel[x])
                continue;
            
            if (x == y) 
                continue;

            if (concordanceThreshold > concordance[y][x]) {
                kernel[x] = false;
                continue;
            }

            if (!nonDiscordance[y][x]) {
                kernel[x] = false;
                continue;
            }
        }
    }

    for (bool val : kernel)
        std::cout << val << " ";
    std::cout << std::endl;

    // check for cycles

}

std::vector<bool> Electre::getKernel() {
    return kernel;
}