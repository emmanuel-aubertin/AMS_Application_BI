// ANSI escape codes for text color
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

#include <iostream>
#include <algorithm>
#include <string.h>
#include "../include/Parser.hpp"
#include "../include/Electre.hpp"
#include "../include/Promethee.hpp"
#include "../include/Algo.hpp"

std::string PROGNAME = "ams-BI";
std::string RELEASE = "Revision 0.1 | Last update 30 Sept 2024";
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

void print_usage(std::vector<Algo> &algo)
{
    std::cout << std::endl
              << PROGNAME << " by " << AUTHOR << std::endl
              << "\033[1mUsage: \033[0m" << PROGNAME << " & [-d | --data] | [-h | --help] | [-v | --version] " << std::endl
              << "          -h | --help                     Help" << std::endl
              << "          -v | --version                  Version" << std::endl
              << "          -a | --algo                     Choose the algorithm that you want to run : " << std::endl;
    for (Algo a : algo)
    {
        std::cout << "                                              " << a.getArgName() << "       " << a.getDescription() << " (" << a.getAltInfo() << ")" << std::endl;
    }
    std::cout << "          -d | --data                     Path to data CSV file" << std::endl
              << "          -w | --weight                   Path to weight CSV file" << std::endl
              << std::endl
              << "\033[1mExample to run Electre:\033[0m " << std::endl
              << PROGNAME << " -a e -d data.csv -w weights.csv" << std::endl;
};

int main(int argc, char **argv)
{
    std::cout << "🤗  | Welcome in \033[1m" << PROGNAME << "\033[0m | 🤗" << std::endl;
    print_release();
    std::cout << std::endl
              << std::endl;

    std::vector<Algo> availableAlgos = {
          Electre()
        };

    std::string filename = "";
    bool isFile = false;

    std::string filenameWeight = "";
    bool isWeightFile = false;

    std::string algoToRun = "a";

    // Arg parser
    if (argc < 0) // number of arg minimum
        failure("One argument required. \n\t-h for help");

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            print_usage(availableAlgos);
            exit(0);
        }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
        {
            print_release();
            exit(0);
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
        else if (!strcmp(argv[i], "-a") || !strcmp(argv[i], "--algo"))
        {
            if (++i < argc)
            {
                algoToRun = argv[i];
                if (algoToRun.find('a') != std::string::npos)
                {
                    warning("Algorithm 'a' (All) overrides all other selections.");
                    algoToRun = "a";
                    continue;
                }

                algoToRun.erase(std::remove_if(algoToRun.begin(), algoToRun.end(),
                                               [&availableAlgos](char c)
                                               {
                                                   bool isValid = std::any_of(availableAlgos.begin(), availableAlgos.end(),
                                                                              [c](Algo &a)
                                                                              { return a.getArgName() == std::string(1, c); });
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
                    exit(1);
                }
            }
            else
            {
                warning("Missing algorithm name after -a or --algo argument.");
                std::cout << "Running all algorithm: " << std::endl;
                algoToRun = "a";
            }
        }

        else
        { // ALL OTHER ARGUMENT
            print_usage(availableAlgos);
            std::string arg = argv[i];
            failure("Unknow argument : " + arg);
        }
    }
    // Test parser
    Parser parser = Parser();

    if (isFile)
    {
        parser.parseFile(filename);
    }
    if (isWeightFile)
    {
        parser.parseWeightFile(filenameWeight);
    }

    

    return 0;
}