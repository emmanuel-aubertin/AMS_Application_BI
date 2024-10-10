#ifndef WEIGHTED_METHODS
#define WEIGHTED_METHODS

#include <vector>
#include <iostream>

class Weighted
{
private:
    std::vector<std::vector<float>> data;
    std::vector<float> weights;
    std::vector<float> results;

public:
    Weighted(std::vector<std::vector<float>> data, std::vector<float> weights);
    void calculateWeight();
    void printWeighted();
};

#endif