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
    this->concordanceThreshold = concordanceThreshold;
    
    int nbCandidates = values.size();
    int nbVariables = weights.size();
    preferenceThresholds = std::vector<float>(nbVariables, 0.0);

    concordance = std::vector<std::vector<float>>(nbCandidates, std::vector<float>(nbCandidates, 0.0));
    nonDiscordance = std::vector<std::vector<bool>>(nbCandidates, std::vector<bool>(nbCandidates, true));
    kernel = std::vector<bool>(nbCandidates, true);
    dominanceMatrix = std::vector<std::vector<bool>>(nbCandidates, std::vector<bool>(nbCandidates, false));
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

                double diff = candidateVal1 - candidateVal2;

                if (diff > veto)
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

void Electre::processDominance() 
{
    for (int y=0; y < values.size(); y++) {
        for (int x=0; x < values.size(); x++) {
            if (concordance[y][x] < concordanceThreshold)
                continue;

            if (!nonDiscordance[y][x])
                continue;

            dominanceMatrix[y][x] = true;
        }
    }
} 


void Electre::processKernel() 
{
    int size = concordance.size();
    for (int y=0; y<size; y++) {
        for (int x=0; x<size; x++) {
            if (!kernel[x])
                continue;

            if (!dominanceMatrix[y][x])
                continue;

            std::cout << y << " domine " << x << std::endl;

            kernel[x] = false;
        }
    }

    // check for cycles
    // if cycles found
    // keep the strongest link

    std::cout << "Kernel" << std::endl;
    for (bool val : kernel)
        std::cout << val << " ";
    std::cout << std::endl;
}

std::vector<std::vector<int>> Electre::getCycles()
{
    std::vector<std::vector<int>> cycles;

    for (int candidate=0; candidate < dominanceMatrix.size(); candidate++) {
        std::vector<std::vector<int>> returnedVector = getSuccessorCycles(candidate, std::vector<int>());
        
        if (returnedVector.empty()) 
            continue;
        

        if (cycles.empty())
            cycles.push_back(returnedVector[0]);

        // filtering the identical cycles, keep the longest ones
        for (int retInd = 0; retInd < returnedVector.size(); retInd++) {
            bool vectorInCycles = false;
            std::vector<int> retVec = returnedVector[retInd];

            for (int cycleInd = 0; cycleInd < cycles.size(); cycleInd++) {
                std::vector<int> cycleVec = cycles[cycleInd];


                // COMPARE TO ALL THE ELEMENTS 
                if (hasSameElements(retVec, cycleVec)) {
                    vectorInCycles = true;
                    break;
                }
            }

            if (!vectorInCycles)
                cycles.push_back(retVec);
        }    
    }

    return cycles;
}

std::vector<std::vector<int>> Electre::getSuccessorCycles(int candidate, std::vector<int> visitedChilds) 
{
    for (int i=0; i<visitedChilds.size(); i++) {
        if (candidate == visitedChilds[i])
            // return the sliced vector containing the cycle
            return std::vector<std::vector<int>> {std::vector(visitedChilds.begin()+i, visitedChilds.end())};;
    }

    visitedChilds.push_back(candidate);

    std::vector<std::vector<int>> returnVector{};

    for (int i=0; i<dominanceMatrix.size(); i++) {
        if (dominanceMatrix[candidate][i] == 1) {
            std::vector<std::vector<int>> cycleVector = getSuccessorCycles(i, visitedChilds);

            // flattening into vector<vector<int>>
            for (std::vector<int> vec : cycleVector) {
                returnVector.push_back(vec);
            }    
        }
    }

    return returnVector;
}

bool Electre::hasSameElements(std::vector<int> vec1, std::vector<int> vec2) 
{
    if (vec1.size() != vec2.size())
        return false;

    std::vector<int> tempVec;
    
    for (int el1 : vec1) {
        for (int el2 : vec2) {
            if (el1 == el2) {
                tempVec.push_back(el1);
                break;         
            }
        }
    }

    return (tempVec.size() == vec1.size());
}


std::vector<bool> Electre::getKernel() {
    return kernel;
}