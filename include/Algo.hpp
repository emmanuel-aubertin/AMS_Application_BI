#ifndef ALGO_HPP
#define ALGO_HPP

#include <string>
#include <iostream>
#include <vector>

class Algo
{
public:
    std::string getName() { return this->name; };
    std::string getArgName() { return this->argName; };
    std::string getDescription() { return this->description; };
    std::string getAltInfo() { return this->altInfo; };

    virtual void setData(const std::vector<std::vector<float>> &newData) { data = newData; }
    virtual void setWeights(const std::vector<float> &newWeights) { weights = newWeights; }

    explicit Algo(const std::string &name, const std::string &argName, const std::string &description, const std::string &altInfo)
        : name(name), argName(argName), description(description), altInfo(altInfo) {}

    // Pure virtual function to be implemented by subclasses
    virtual void run() { std::cout << "Pure virtual function, must be implemented by subclasses" << std::endl; };
    virtual int save(std::string dirPath);

protected:
    std::string name;
    std::string argName;
    std::string description;
    std::string altInfo;

    std::vector<std::vector<float>> data;
    std::vector<float> weights;
};

#endif