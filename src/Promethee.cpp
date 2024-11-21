#include "../include/Promethee.hpp"

/**
 * @brief Construct a new Promethee object.
 *
 * Initializes the data and weights for the Promethee method and resizes
 * the multicriteria preference matrix based on the size of the input data.
 *
 * @param data A 2D vector where each row represents an alternative and each column represents a criterion.
 * @param weights A vector representing the weights for each criterion.
 */
Promethee::Promethee(std::vector<std::vector<float>> data, std::vector<float> weights) : Promethee()
{
    this->data = data;
    this->weights = weights;
    multicriteriaPreferenceMatrix.resize(data.size(), std::vector<float>(data.size(), -1)); // Initialize the matrix
}

/**
 * @brief Calculate the preference of one alternative over another for a given criterion.
 *
 * This function compares two values (corresponding to two alternatives for a specific criterion)
 * and returns a preference value. The preference is 1 if value1 is greater than value2, otherwise 0.
 *
 * @param value1 The value of the first alternative for a specific criterion.
 * @param value2 The value of the second alternative for a specific criterion.
 * @return float Returns 1.0 if value1 is preferred over value2, otherwise returns 0.0.
 */
float Promethee::calculatePreference(float value1, float value2)
{
    return (value1 > value2) ? 0.0 : 1.0;
}

/**
 * @brief Calculate the multicriteria preference matrix for all alternatives.
 *
 * This function compares all pairs of alternatives using multiple criteria. For each pair (i, j),
 * it calculates the weighted sum of the preferences for each criterion. The results are stored
 * in the `multicriteriaPreferenceMatrix`.
 *
 * @note Diagonal elements (where i == j) are set to -1, indicating no comparison between the same alternative.
 */
void Promethee::calculatePreferenceMatrix()
{
    int n = data.size();    // Number of alternatives
    int m = data[0].size(); // Number of criteria

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (i == j)
            {
                multicriteriaPreferenceMatrix[i][j] = 1; // Diagonal element
            }
            else
            {
                float preferenceSum = 0.0f;
                for (int k = 0; k < m; ++k)
                {
                    preferenceSum += calculatePreference(data[k][i], data[k][j]) * weights[k];
                }
                multicriteriaPreferenceMatrix[i][j] = preferenceSum; // Store the final weighted preference in the matrix
            }
        }
    }
}

/**
 * @brief Print the multicriteria preference matrix.
 *
 * This function prints the matrix that was computed in the `calculatePreferenceMatrix` method.
 * The matrix displays the preference of one alternative over another for all pairs of alternatives.
 * The diagonal elements, where i == j, are represented by -1 to indicate that no comparison was made.
 */
void Promethee::printPreferenceMatrix()
{
    std::cout << "Preference Matrix:\n";
    for (int i = 0; i < multicriteriaPreferenceMatrix.size(); ++i)
    {
        for (int j = 0; j < multicriteriaPreferenceMatrix[i].size(); ++j)
        {
            if (multicriteriaPreferenceMatrix[i][j] == 0)
                std::cout << std::setw(4) << "-1" << "\t"; // Diagonal element
            else
                std::cout << std::setw(4) << multicriteriaPreferenceMatrix[i][j] << "\t"; // Other elements
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Calculate the positive (φ+) and negative (φ−) outranking flows for each alternative.
 *
 * Positive flow (φ+) represents how much an alternative outranks other alternatives.
 * Negative flow (φ−) represents how much an alternative is outranked by others.
 */
void Promethee::calculateFlows()
{
    int n = multicriteriaPreferenceMatrix.size(); // Number of alternatives

    // Resize the positive and negative flow vectors to store values for each alternative
    positiveFlow.resize(n, 0.0);
    negativeFlow.resize(n, 0.0);
    flows.resize(n, 0.0);

    // Calculate the positive and negative flows
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (i != j)
            {
                positiveFlow[i] += multicriteriaPreferenceMatrix[i][j]; // Sum of how much i outranks others
                negativeFlow[i] += multicriteriaPreferenceMatrix[j][i]; // Sum of how much i is outranked by others
            }
        }
        flows[i] = positiveFlow[i] - negativeFlow[i];
    }
}

/**
 * @brief Print the positive and negative outranking flows for each alternative.
 *
 * The positive flow (φ+) indicates the strength of an alternative in outranking others.
 * The negative flow (φ−) indicates the degree to which an alternative is outranked by others.
 */
void Promethee::printFlows()
{
    std::cout << "Positive Flow (φ+):\n";
    for (int i = 0; i < positiveFlow.size(); ++i)
    {
        std::cout << "Alternative " << i + 1 << ": " << std::setw(6) << positiveFlow[i] << "\n";
    }

    std::cout << "\nNegative Flow (φ−):\n";
    for (int i = 0; i < negativeFlow.size(); ++i)
    {
        std::cout << "Alternative " << i + 1 << ": " << std::setw(6) << negativeFlow[i] << "\n";
    }

    std::cout << "\nFlows (φ):\n";
    for (int i = 0; i < flows.size(); ++i)
    {
        std::cout << "Alternative " << i + 1 << ": " << std::setw(6) << flows[i] << "\n";
    }
}

void Promethee::printLatexOutput()
{
    // TODO: add the preference matrix to latex output
    std::cout << "\\begin{table}[h] \n";
    std::cout << "\\centering \n";
    std::cout << "\\begin{tabular}{|c|c|c|c|} \n";
    std::cout << "\\hline \n";
    std::cout << "\\textbf{Alternative} & \\textbf{Positive Flow (\\varphi+)} & \\textbf{Negative Flow (\\varphi−)} & \\textbf{Flows (\\varphi)} \\\\ \n";
    std::cout << "\\hline \n";
    for (int i = 0; i < flows.size(); ++i)
    {
        std::cout << "Alternative " << i + 1 << ": & " << positiveFlow[i] << " & " << negativeFlow[i] << " & " << flows[i] << " \\\\ \n";
    }
    std::cout << "\\hline \n";
    std::cout << "\\end{tabular} \n";
    std::cout << "\\caption{Promethee Flow Data} \n";
    std::cout << "\\label{tab:promethee_flows} \n";
    std::cout << "\\end{table} \n";
}

void Promethee::run() {
    // Implement Electre's logic here
    std::cout << "Promethee is running!" << std::endl; 
}