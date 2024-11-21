#ifndef ALGO_HPP
#define ALGO_HPP

#include <string>

class Algo {
public:
    std::string getName() {return this->name;};
    std::string getArgName() {return this->argName;};
    std::string getDescription() {return this->description;};
    std::string getAltInfo() {return this->altInfo;};

    explicit Algo(const std::string &name, const std::string &argName, const std::string &description, const std::string &altInfo)
        : name(name), argName(argName), description(description), altInfo(altInfo) {}

    // Pure virtual function to be implemented by subclasses
    virtual void run(){}; 

protected:
    std::string name;
    std::string argName;
    std::string description;
    std::string altInfo;
};

#endif