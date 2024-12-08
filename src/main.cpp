#include <iostream>
#include <algorithm>
#include <memory>
#include <string.h>
#include <filesystem>
#include "../include/Parser.hpp"
#include "../include/Electre.hpp"
#include "../include/Promethee.hpp"
#include "../include/Algo.hpp"
// #include "../include/OptimizationType.hpp"

// ANSI escape codes for text color
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

std::string PROGNAME = "ams-BI";
std::string RELEASE = "Revision 0.2 | Last update 2 Dec 2024";
std::string AUTHOR = "\033[1mAubertin Emmanuel, Ange Cure, Jerome Chen\033[0m";
std::string COPYRIGHT = "(c) 2024 " + AUTHOR + "\nFrom https://github.com/emmanuel-aubertin/AMS_Application_BI";
bool VERBOSE = false;

auto print_release = []
{
    std::cout << RELEASE << '\n'
              << COPYRIGHT << '\n';
};

auto failure = [](std::string_view message)
{
    std::cerr << "❌ Error: " << message << " ❌\n";
};

auto warning = [](std::string_view message)
{
    std::cerr << "⚠️ Warning: " << message << " ⚠️\n";
};

void print_usage(const std::vector<std::unique_ptr<Algo>> &algo)
{
    std::cout << std::endl
              << PROGNAME << " by " << AUTHOR << std::endl
              << "\033[1mUsage: \033[0m" << PROGNAME << " & [-d | --data] | [-h | --help] | [-v | --version] " << std::endl
              << "          -h | --help                     Help" << std::endl
              << "          -v | --version                  Version" << std::endl
              << "          -s | --save                     Path to the output file" << std::endl
              << "          -d | --data                     Path to data CSV file" << std::endl
              << "          -w | --weight                   Path to weight CSV file" << std::endl
              << "          -a | --algo                     Choose the algorithm that you want to run : " << std::endl;
    for (const auto &a : algo)
    {
        std::cout << "                                              " << a->getArgName() << "       " << a->getDescription() << " (" << a->getAltInfo() << ")" << std::endl;
    }
    std::cout << std::endl
              << "\033[1mExample to run Electre:\033[0m " << std::endl
              << PROGNAME << " -a e -d data/recycle/donnees.csv -w data/recycle/poids.csv" << std::endl;
};

int main(int argc, char **argv)
{
    std::cout << "🤗  | Welcome in \033[1m" << PROGNAME << "\033[0m | 🤗" << std::endl;
    print_release();
    std::cout << std::endl
              << std::endl;

    std::vector<std::unique_ptr<Algo>> availableAlgos;
    availableAlgos.push_back(std::make_unique<Electre>());
    availableAlgos.push_back(std::make_unique<Promethee>());

    std::string outputFile = "";

    std::string filename = "";
    bool isFile = false;

    std::string filenameWeight = "";
    bool isWeightFile = false;

    std::string algoToRun = "a";

    std::string preferencesFile = "";
    bool isPreferencesFile = false;

    std::string vetosFile = "";
    bool isVetosFile = false;

    std::string optimizationsFile = "";
    bool isOptimizationsFile = false;

    std::string concordanceThresholdFile = "";
    bool isConcordanceThresholdFile = false;

    // Arg parser
    if (argc < 0)
    {
        failure("One argument required. \n\t-h for help");
    }

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            print_usage(availableAlgos);
            return 0;
        }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
        {
            print_release();
            return 0;
        }
        else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--save"))
        {
            outputFile = argv[++i];
        }
        else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--data"))
        {
            filename = argv[++i];
            isFile = true;
        }
        else if (!strcmp(argv[i], "-w") || !strcmp(argv[i], "--weight"))
        {
            filenameWeight = argv[++i];
            isWeightFile = true;
        }
        else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--preferences"))
        {
            preferencesFile = argv[++i];
            isPreferencesFile = true;
        }
        else if (!strcmp(argv[i], "-V") || !strcmp(argv[i], "--vetos"))
        {
            vetosFile = argv[++i];
            isVetosFile = true;
        }
        else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--optimization"))
        {
            optimizationsFile = argv[++i];
            isOptimizationsFile = true;
        }
        else if (!strcmp(argv[i], "-ct") || !strcmp(argv[i], "--concordance_threshold"))
        {
            concordanceThresholdFile = argv[++i];
            isConcordanceThresholdFile = true;
        }
        else if (!strcmp(argv[i], "-a") || !strcmp(argv[i], "--algo"))
        {
            if (++i < argc)
            {
                algoToRun = argv[i];

                algoToRun.erase(std::remove_if(algoToRun.begin(), algoToRun.end(),
                                               [&availableAlgos](char c)
                                               {
                                                   bool isValid = std::any_of(availableAlgos.begin(), availableAlgos.end(),
                                                                              [c](const std::unique_ptr<Algo> &a)
                                                                              { return a->getArgName() == std::string(1, c); });
                                                   if (!isValid)
                                                   {
                                                       warning(std::string(1, c) + " is not a valid algorithm");
                                                   }
                                                   return !isValid;
                                               }),
                                algoToRun.end());

                if (algoToRun.empty())
                {
                    failure("No valid algorithm choices provided.");
                    return 1;
                }
            }
            else
            {
                warning("Missing algorithm name after -a or --algo argument.");
                std::cout << "Running all algorithms." << std::endl;
                algoToRun = "a";
            }
        }
        else
        {
            print_usage(availableAlgos);
            failure("Unknown argument: " + std::string(argv[i]));
            return 1;
        }
    }

    Parser parser;
    if (!isFile && !isWeightFile)
    {
        failure("You must specify a dataset file and/or weight file.");
        return 1;
    }

    if (isFile)
    {
        parser.parseFile(filename);
    }
    if (isWeightFile)
    {
        parser.parseWeightFile(filenameWeight);
    }

    std::vector<std::vector<float>> data = parser.getParsedFile();
    std::vector<float> weights = parser.getParsedWeight();

    for (const char c : algoToRun)
    {
        auto it = std::find_if(availableAlgos.begin(), availableAlgos.end(),
                               [c](const std::unique_ptr<Algo> &algo)
                               { return algo->getArgName() == std::string(1, c); });

        if (it != availableAlgos.end())
        {
            std::cout << BLUE << "\033[1mExecuting: " << (*it)->getName() << RESET << std::endl;

            if (auto *electre = dynamic_cast<Electre *>(it->get()))
            {
                if (!isVetosFile)
                {
                    std::cerr << "You must specify a veto file when using the Electre method." << std::endl; 
                    continue;
                }
                parser.parseVetosFile(vetosFile);
                std::vector<float> vetos = parser.getParsedVetosFile();

                if (!isOptimizationsFile)
                {
                    std::cerr << "You must specify an optimization file when using the Electre method." << std::endl; 
                    continue;
                }
                parser.parseOptimizationsFile(optimizationsFile);
                std::vector<OptimizationType> optimizations = parser.getParsedOptimizationsFile();

                if (!isConcordanceThresholdFile)
                {
                    std::cerr << "You must specify a concordance threshold file when using the Electre method." << std::endl; 
                    continue;
                }                
                parser.parseConcordanceThresholdFile(concordanceThresholdFile);
                float concordanceThreshold = parser.getParsedConcordanceThresholdFile();

                electre->setData(data);
                electre->setWeights(weights);
                electre->setVetos(vetos);
                electre->setConcordanceThreshold(concordanceThreshold);
                if (isPreferencesFile)
                {
                    parser.parsePreferencesFile(preferencesFile);
                    std::vector<float> preferences = parser.getParsedPreferencesFile();
                    electre->setPreferenceThresholds(preferences);
                }
                electre->setOptimizations(optimizations);
                electre->run();
                if (outputFile != "")
                {
                    electre->save(outputFile);
                }
            }
            else if (auto *promethee = dynamic_cast<Promethee *>(it->get()))
            {
                promethee->setData(data);
                promethee->setWeights(weights);
                promethee->run();
                if (outputFile != "")
                {
                promethee->save(outputFile);
                }
            }
        }
        else
        {
            warning("No matching algorithm found for '" + std::string(1, c) + "'.");
        }
    }

    return 0;
}
