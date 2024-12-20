#include "../include/Electre.hpp"

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

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
 * @brief Calculates the concordance matrix.
 *
 *  This method iterates through the values matrix and calculates the concordance values for each pair of attributes.
 *  The concordance value represents the degree to which two attributes agree on the relative preference of alternatives.
 */
void Electre::processConcordance()
{
    for (int y = 0; y < values.size() - 1; y++)
    {
        for (int x = y + 1; x < values.size(); x++)
        {
            float concordVal1 = 0;
            float concordVal2 = 0;

            for (int criterium = 0; criterium < weights.size(); criterium++)
            {
                float candidateVal1 = values[y][criterium];
                float candidateVal2 = values[x][criterium];

                float threshold = 0;
                if (!preferenceThresholds.empty())
                    threshold = preferenceThresholds[criterium];

                float val = weights[criterium];

                double diff = std::abs(candidateVal1 - candidateVal2);
                double coeff = 0;
                if (threshold != 0)
                    coeff = 1 - std::min(1.0, diff / threshold);

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
    for (int criterium = 0; criterium < weights.size(); criterium++)
    {
        for (int y = 0; y < values.size(); y++)
        {
            for (int x = 0; x < values.size(); x++)
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
    for (int y = 0; y < values.size(); y++)
    {
        for (int x = 0; x < values.size(); x++)
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
    for (int y = 0; y < values.size(); y++)
    {
        for (int x = 0; x < values.size(); x++)
        {
            if (!kernel[x])
                continue;

            if (!dominance[y][x])
                continue;

            if (concordance[y][x] < concordanceThreshold)
                continue;

            kernel[x] = false;
        }
    }
}

std::vector<std::vector<int>> Electre::getCycles()
{
    std::vector<std::vector<int>> cycles;

    for (int candidate = 0; candidate < values.size(); candidate++)
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

    for (int i = 0; i < values.size(); i++)
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

/**
 * @brief Set the veto thresholds for each criterion.
 *
 * @param newVetos A vector representing the veto thresholds.
 */
void Electre::setVetos(const std::vector<float> &newVetos)
{
    vetos = newVetos;
}

/**
 * @brief Set the preference thresholds for each criterion.
 *
 * @param newThresholds A vector representing the preference thresholds.
 */
void Electre::setPreferenceThresholds(const std::vector<float> &newThresholds)
{
    if(newThresholds.empty()){
        preferenceThresholds = std::vector<float>(nbCriteria, 0.0);
        return;
    }
    preferenceThresholds = newThresholds;
}

/**
 * @brief Set the optimization types for the criteria.
 *
 * @param newOptimizations A vector of optimization types (e.g., Minimize, Maximize).
 */
void Electre::setOptimizations(const std::vector<OptimizationType> &newOptimizations)
{
    optimizations = newOptimizations;
}

/**
 * @brief Set the concordance threshold.
 *
 * @param newThreshold The new concordance threshold.
 */
void Electre::setConcordanceThreshold(float newThreshold)
{
    concordanceThreshold = newThreshold;
}

/**
 * @brief Set the concordance matrix.
 *
 * @param newConcordance A 2D vector representing the concordance matrix.
 */
void Electre::setConcordanceMatrix(const std::vector<std::vector<float>> &newConcordance)
{
    concordance = newConcordance;
}

/**
 * @brief Set the non-discordance matrix.
 *
 * @param newNonDiscordance A 2D vector of booleans representing the non-discordance matrix.
 */
void Electre::setNonDiscordanceMatrix(const std::vector<std::vector<bool>> &newNonDiscordance)
{
    nonDiscordance = newNonDiscordance;
}

/**
 * @brief Set the dominance matrix.
 *
 * @param newDominance A 2D vector of booleans representing the dominance matrix.
 */
void Electre::setDominanceMatrix(const std::vector<std::vector<bool>> &newDominance)
{
    dominance = newDominance;
}

/**
 * @brief Set the kernel.
 *
 * @param newKernel A vector of booleans representing the kernel.
 */
void Electre::setKernel(const std::vector<bool> &newKernel)
{
    kernel = newKernel;
}

void Electre::printVectors()
{
    std::cout << "Printing corcordance: " << std::endl;
    for (std::vector<float> row : concordance) 
    {
        for (float val : row) 
        {
            std::cout << val << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << "Printing non-discordance: " << std::endl;
    for (std::vector<bool> row : nonDiscordance) 
    {
        for (bool val : row) 
        {
            std::cout << val << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << "Printing dominance: " << std::endl;
    for (std::vector<bool> row : dominance) 
    {
        for (bool val : row) 
        {
            std::cout << val << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << "Printing kernel: " << std::endl;
    for (bool val : kernel) 
    {
        std::cout << val << "\t";
    }
    std::cout << std::endl;
}


int Electre::save(std::string dirPath)
{
    std::filesystem::path folderPath = std::filesystem::path(dirPath) / "Electre";

    if (!std::filesystem::exists(folderPath)) {
        if (!std::filesystem::create_directories(folderPath)) {
            std::cerr << "Error creating directory: " << folderPath << std::endl;
            return -1;
        }
    }


    std::filesystem::path filePath = folderPath / "kernel.csv";
    std::ofstream outputFileKernel(filePath);

    if (!outputFileKernel) {
        std::cerr << "Error creating file for writing" << std::endl;
        return -1;
    }

    for (const auto& row : this->kernel) {
        outputFileKernel << row << ",";
    }

    outputFileKernel.close();

    return 1;
}

void Electre::run()
{
    std::cout << GREEN << "========== Starting Electre Algorithm ==========" << RESET << "\n";

    // Step 1: Process concordance matrix
    std::cout << BLUE << "[Step 1/4]" << RESET << " Processing the concordance matrix..." << std::endl;
    concordance.resize(values.size(), std::vector<float>(values.size(), 0.0));
    processConcordance();
    std::cout << GREEN << "✔ Concordance matrix processed successfully." << RESET << "\n";

    // Step 2: Process nondiscordance matrix
    std::cout << BLUE << "[Step 2/4]" << RESET << " Processing the nondiscordance matrix..." << std::endl;
    nonDiscordance.resize(values.size(), std::vector<bool>(values.size(), true));
    processNondiscordance();
    std::cout << GREEN << "✔ Nondiscordance matrix processed successfully." << RESET << "\n";

    // Step 3: Compute dominance relations
    std::cout << BLUE << "[Step 3/4]" << RESET << " Computing dominance relations..." << std::endl;
    dominance.resize(values.size(), std::vector<bool>(values.size(), false));
    processDominance();
    std::cout << GREEN << "✔ Dominance relations computed successfully." << RESET << "\n";

    // Step 4: Identify the kernel
    std::cout << BLUE << "[Step 4/4]" << RESET << " Identifying the kernel (final decision set)..." << std::endl;
    kernel.resize(values.size(), true);
    processKernel();
    std::cout << GREEN << "✔ Kernel identified successfully." << RESET << "\n";

    std::cout << GREEN << "========== Electre Algorithm Completed ==========" << RESET << "\n";
}
