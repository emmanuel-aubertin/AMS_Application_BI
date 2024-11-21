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
    std::vector<OptimizationType> optimizations,
    float concordanceThreshold) : Electre()
{
    this->values = values;
    this->weights = weights;
    this->vetos = vetos;
    this->concordanceThreshold = concordanceThreshold;
    this->optimizations = optimizations;

    nbCandidates = values.size();
    nbCriteria = weights.size();
    preferenceThresholds = std::vector<float>(nbCriteria, 0.0);

    concordance = std::vector<std::vector<float>>(nbCandidates, std::vector<float>(nbCandidates, 0.0));
    nonDiscordance = std::vector<std::vector<bool>>(nbCandidates, std::vector<bool>(nbCandidates, true));
    kernel = std::vector<bool>(nbCandidates, true);
    dominance = std::vector<std::vector<bool>>(nbCandidates, std::vector<bool>(nbCandidates, false));
}

Electre::Electre(
    std::vector<std::vector<float>> values,
    std::vector<float> weights,
    std::vector<float> vetos,
    std::vector<float> preferenceThresholds,
    std::vector<OptimizationType> optimizations,
    float concordanceThreshold) : Electre::Electre(values, weights, vetos, optimizations, concordanceThreshold) 
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
    processDominance();
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
    for (int y = 0; y < nbCandidates - 1; y++)
    {
        for (int x = y + 1; x < nbCandidates; x++)
        {
            float concordVal1 = 0;
            float concordVal2 = 0;

            for (int criterium = 0; criterium < nbCriteria; criterium++)
            {
                float candidateVal1 = values[y][criterium];
                float candidateVal2 = values[x][criterium];
                float threshold = preferenceThresholds[criterium];
                float val = weights[criterium];

                double diff = std::abs(candidateVal1 - candidateVal2);
                double coeff = 1 - std::min(1.0, diff / threshold);

                if (candidateVal1 == candidateVal2)
                {
                    concordVal1 += val;
                    concordVal2 += val;
                    continue;
                }

                if (optimizations[criterium] == MIN)
                {
                    if (candidateVal1 < candidateVal2)
                    {
                        concordVal1 += val;
                        concordVal2 += coeff * val;
                        continue;
                    }

                    concordVal2 += val;
                    concordVal1 += coeff * val;
                }
                else
                {
                    if (candidateVal1 > candidateVal2)
                    {
                        concordVal1 += val;
                        concordVal2 += coeff * val;
                        continue;
                    }

                    concordVal2 += val;
                    concordVal1 += coeff * val;
                }
            }

            concordance[y][x] = concordVal1;
            concordance[x][y] = concordVal2;
        }
    }
}

void Electre::processNondiscordance()
{
    for (int criterium = 0; criterium < nbCriteria; criterium++)
    {
        for (int y = 0; y < nbCandidates; y++)
        {
            for (int x = 0; x < nbCandidates; x++)
            {
                if (y == x)
                {
                    nonDiscordance[y][x] = false;
                    continue;
                }

                if (!nonDiscordance[y][x])
                    continue;

                double candidateVal1 = values[y][criterium];
                double candidateVal2 = values[x][criterium];
                float veto = vetos[criterium];

                if (optimizations[criterium] == MAX)
                {
                    candidateVal1 = -candidateVal1;
                    candidateVal2 = -candidateVal2;
                }

                double diff = candidateVal1 - candidateVal2;

                if (diff > veto)
                {
                    nonDiscordance[y][x] = false;
                    continue;
                }
            }
        }
    }
}

void Electre::processDominance()
{
    for (int y = 0; y < nbCandidates; y++)
    {
        for (int x = 0; x < nbCandidates; x++)
        {
            if (concordance[y][x] < concordanceThreshold)
                continue;

            if (!nonDiscordance[y][x])
                continue;

            dominance[y][x] = true;
        }
    }
}

void Electre::processKernel()
{
    std::vector<std::vector<int>> cycles = getCycles();
    if (cycles.size() != 0)
        deleteCycles(cycles);

    // get kernel
    for (int y = 0; y < nbCandidates; y++)
    {
        for (int x = 0; x < nbCandidates; x++)
        {
            // if (!kernel[x])
            //     continue;

            if (!dominance[y][x])
                continue;

            kernel[x] = false;
        }
    }

    std::cout << "Kernel" << std::endl;
    for (bool val : kernel)
        std::cout << val << " ";
    std::cout << std::endl;
}

std::vector<std::vector<int>> Electre::getCycles()
{
    std::vector<std::vector<int>> cycles;

    for (int candidate = 0; candidate < nbCandidates; candidate++)
    {
        std::vector<std::vector<int>> returnedVector = getSuccessorCycles(candidate, std::vector<int>());

        if (returnedVector.empty())
            continue;

        // filtering the identical cycles; keeping only unique ones
        for (int retInd = 0; retInd < returnedVector.size(); retInd++)
        {
            bool cyclePresent = false;
            std::vector<int> retVec = returnedVector[retInd];

            for (int cycleInd = 0; cycleInd < cycles.size(); cycleInd++)
            {
                std::vector<int> cycleVec = cycles[cycleInd];

                // compare the selected cycle to the others that are getting returned
                if (hasSameElements(retVec, cycleVec))
                {
                    cyclePresent = true;
                    break;
                }
            }

            if (!cyclePresent)
                cycles.push_back(retVec);
        }
    }

    return cycles;
}

std::vector<std::vector<int>> Electre::getSuccessorCycles(int candidate, std::vector<int> visitedChilds)
{
    for (int i = 0; i < visitedChilds.size(); i++)
    {
        if (candidate == visitedChilds[i])
            // return the sliced vector containing the cycle
            return std::vector<std::vector<int>>{std::vector(visitedChilds.begin() + i, visitedChilds.end())};
        ;
    }

    visitedChilds.push_back(candidate);

    std::vector<std::vector<int>> returnVector{};

    for (int i = 0; i < nbCandidates; i++)
    {
        if (dominance[candidate][i] == 1)
        {
            std::vector<std::vector<int>> cycleVector = getSuccessorCycles(i, visitedChilds);

            // flattening into vector<vector<int>>
            for (std::vector<int> vec : cycleVector)
            {
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

    for (int el1 : vec1)
    {
        for (int el2 : vec2)
        {
            if (el1 == el2)
            {
                tempVec.push_back(el1);
                break;
            }
        }
    }

    return (tempVec.size() == vec1.size());
}

void Electre::deleteCycles(std::vector<std::vector<int>> cycles)
{
    for (std::vector<int> cycle : cycles)
    {
        std::vector<std::array<int, 2>> links;

        // init links
        for (int linkIndex = 0; linkIndex < cycle.size(); linkIndex++)
        {
            int start = cycle[linkIndex];
            int end = (linkIndex + 1 < cycle.size()) ? cycle[linkIndex + 1] : cycle[0];
            std::array<int, 2> link{start, end};
            links.push_back(link);
        }

        // find the highest concordance values, and the link index
        int highestConcordanceLinkIndex = 0;
        float highestConcordanceValue = 0.0;
        for (int i = 0; i < links.size(); i++)
        {
            std::array<int, 2> link = links[i];
            float concorValue = concordance[link[0]][link[1]];
            if (concorValue > highestConcordanceValue)
            {
                highestConcordanceValue = concorValue;
                highestConcordanceLinkIndex = i;
            }
        }

        // delete the links
        for (int i = 0; i < links.size(); i++)
        {
            if (i == highestConcordanceLinkIndex)
                continue;

            std::array<int, 2> link = links[i];
            dominance[link[0]][link[1]] = 0;
        }
    }
}

std::vector<bool> Electre::getKernel()
{
    return kernel;
}

void Electre::run() {
        // Implement Electre's logic here
        std::cout << "Electre is running!" << std::endl; 
    }