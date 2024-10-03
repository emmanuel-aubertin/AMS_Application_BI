#include "../include/Electre.hpp"

/**
 * @brief Constructs an Electre object.
 *
 * Initializes the Electre object with the provided values, weights, and concordance threshold.
 *
 * @param values A 2D vector representing the decision matrix, where each inner vector represents a row.
 * @param weights A vector of floating-point numbers representing the weights for each attribute.
 * @param concordanceThreshold A floating-point number representing the threshold for concordance.
 */
Electre::Electre(std::vector<std::vector<float>> values, std::vector<float> weights, float concordanceThreshold)
{
    this->values = values;
    this->weights = weights;

    int size = values.size();
    concordance = std::vector<std::vector<float>>(size, std::vector<float>(size, 0.0));
    nonDiscordance = std::vector<std::vector<bool>>(size, std::vector<bool>(size, true));

    // process values
    this->processMatrixes();
}

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
Electre::Electre(std::vector<std::vector<float>> values, std::vector<float> weights, std::vector<float> vetos, float concordanceThreshold)
{
    this->values = values;
    this->weights = weights;
    this->vetos = vetos;
    
    int size = values.size();
    concordance = std::vector<std::vector<float>>(size, std::vector<float>(size, 0.0));
    nonDiscordance = std::vector<std::vector<bool>>(size, std::vector<bool>(size, true));

    // process values
    this->processMatrixes();
}

/**
 * @brief Processes the data to calculate the concordance and discordance matrices.
 */
void Electre::processMatrixes()
{
    processConcordance();
    processNondiscordance();
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
            int concordVal1 = 0;
            int concordVal2 = 0;

            for (int criterium = 0; criterium < weights.size(); criterium++)
            {
                float candidateVal1 = values[y][criterium];
                float candidateVal2 = values[x][criterium];

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
    for (std::vector<float> line : concordance)
    {
        for (float val : line)
            std::cout << val << "\t";
        std::cout << std::endl;
    }
    std::cout << " ✅ Concordance done ✅ " << std::endl
              << std::endl;
}

/**
 * @brief Calculates the discordance matrix.
 *
 * This method determines the degree to which attributes disagree on the relative preference of alternatives.
 */
void Electre::processNondiscordance()
{
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
                float candidateVal1 = values[y][criterium];
                float candidateVal2 = values[x][criterium];
                float veto = vetos[criterium];


                if (candidateVal1 - candidateVal2 > veto)
                {
                    nonDiscordance[y][x] = false;
                    break;
                }
            }
        }
    }

    std::cout << "Non discordance matrix: " << std::endl;
    for (std::vector<bool> line : nonDiscordance)
    {
        for (bool val : line)
            std::cout << val << "\t";
        std::cout << std::endl;
    }
    std::cout << " ✅ Concordance done ✅ " << std::endl
            << std::endl;
}

/**
 * @brief Returns the kernel (final ranking) of alternatives.
 *
 * @return A vector of integers representing the ranking of alternatives.
 */
std::vector<float> Electre::getKernel()
{
    return kernel;
}