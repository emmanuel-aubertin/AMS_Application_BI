// ANSI escape codes for text color
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

#include <iostream>
#include <string.h>
#include "../include/Electre.hpp"

std::string PROGNAME = "TBA";
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

void print_usage()
{
    std::cout << std::endl
              << PROGNAME << " by " << AUTHOR << std::endl
              << "\033[1mUsage: \033[0m" << PROGNAME << " | [-h | --help] | [-v | --version] " << std::endl
              << "          -h | --help                     Help" << std::endl
              << "          -v | --version                  Version" << std::endl
              << "          -d | --data                     Path to data CSV file" << std::endl
              << "          -w | --weight                   Path to weight CSV file" << std::endl;
};

auto print_help = []()
{
    print_release();
    std::cout << std::endl;
    print_usage();
    std::cout << std::endl
              << std::endl;
    exit(0);
};

int main(int argc, char **argv)
{
    std::cout << "🤗  | Welcome in \033[1m" << PROGNAME << "\033[0m | 🤗" << std::endl;
    print_release();
    std::cout << std::endl
              << std::endl;
  
    std::string filename = "";
    bool isFile = false;

    std::string filenameWeight = "";
    bool isWeightFile = false;


    // Arg parser
    if (argc < 0) // number of arg minimum
        failure("One argument required. \n\t-h for help");

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            print_usage();
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
        else
        { // ALL OTHER ARGUMENT
            print_usage();
            std::string arg = argv[i];
            failure("Unknow argument : " + arg);
        }
    }

    // ELECTRE
    std::vector<std::vector<float>> values {
        std::vector<float> {80, -90, 600,  5.4, 8, -5},
        std::vector<float> {65, -58, 200,  9.7, 1, -1},
        std::vector<float> {83, -60, 400,  7.2, 4, -7},
        std::vector<float> {40, -80, 1000, 7.5, 7, -10},
        std::vector<float> {52, -72, 600,  2.0, 3, -8},
        std::vector<float> {94, -96, 700,  3.6, 5, -6},
    };

    std::vector<float> weights {
        0.1, 0.2, 0.2, 0.1, 0.2, 0.2
    };

    std::vector<float> preferenceThresholds {
        20, 10, 200, 4, 2, 2    
    };

    std::vector<float> vetos {
        750.0, 3.0, 3.5, 3.5
    };

    float concordanceThreshold = 0.6;

    Electre elV(values, weights, vetos, concordanceThreshold);
    elV.processMatrixes();

    Electre elS(values, weights, preferenceThresholds, vetos, concordanceThreshold);
    elS.processMatrixes();

    return 0;
}