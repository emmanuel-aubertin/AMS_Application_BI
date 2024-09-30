#ifndef ELECTRE_METHODS
#define ELECTRE_METHODS

#include <vector>

class Electre {
    private:
        std::vector<std::vector<int>> values;
        std::vector<float> weights;
        std::vector<std::vector<float>> concordance;
        std::vector<std::vector<bool>> nonDiscordance;
    public:
        Electre(std::vector<std::vector<int>> values, std::vector<double> weights);
        ~Electre();
};

#endif