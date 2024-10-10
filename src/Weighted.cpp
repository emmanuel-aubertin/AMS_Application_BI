#include "../include/Weighted.hpp"

/**
 * @brief Constructor for the Weighted class.
 *
 * Initializes the `data` and `weights` members with the provided arguments and
 * resizes the `results` vector to match the number of alternatives (i.e., the number of columns in the data).
 *
 * @param data A 2D vector where each row represents the criteria, and each column represents the alternatives.
 * @param weights A vector of weights corresponding to the importance of each criterion.
 */
Weighted::Weighted(std::vector<std::vector<float>> data, std::vector<float> weights)
{
    this->data = data;
    this->weights = weights;
    // Resizing results to have the same number of elements as the number of alternatives (columns).
    results.resize(data[0].size());
}

/**
 * @brief Calculates the weighted sum for each alternative.
 *
 * Iterates through the matrix of `data` and multiplies each criterion by its respective weight.
 * The weighted sums for each alternative are stored in the `results` vector.
 *
 * This method assumes that `data` is a matrix where rows represent criteria and columns represent alternatives,
 * and that the size of `weights` matches the number of criteria.
 */
void Weighted::calculateWeight()
{
    int n = data.size();    // Number of criteria (rows)
    int m = data[0].size(); // Number of alternatives (columns)
    
    // Iterate over each alternative (column)
    for (int i = 0; i < m; ++i)
    {
        // Iterate over each criterion (row)
        for (int j = 0; j < n; ++j)
        {
            // Add the weighted value for each criterion to the result for this alternative
            results[i] += weights[j] * data[j][i];
        }
    }
}

/**
 * @brief Prints the calculated weighted results for each alternative.
 *
 * Iterates through the `results` vector and outputs each value to the console, separated by spaces.
 */
void Weighted::printWeighted()
{
    // Print each result value
    for (float val : results)
        std::cout << val << " ";
}
