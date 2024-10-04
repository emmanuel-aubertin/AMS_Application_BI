#ifndef PROMETHEE_METHODS
#define PROMETHEE_METHODS

#include <vector>
#include <iostream>
#include <iomanip>

class Promethee
{
private:
    std::vector<std::vector<float>> data;                          // Alternatives and criteria
    std::vector<float> weights;                                    // Weights for each criterion
    std::vector<std::vector<float>> multicriteriaPreferenceMatrix; // Multicriteria Preference Matrix

    float calculatePreference(float value1, float value2);

public:
    Promethee(std::vector<std::vector<float>> data, std::vector<float> weights);
    void calculatePreferenceMatrix();
    void printPreferenceMatrix();
};

#endif