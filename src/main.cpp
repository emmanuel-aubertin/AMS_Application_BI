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
std::string COPYRIGHT = "(c) 2024 " + AUTHOR + " from https://github.com/emmanuel-aubertin/AMS_Application_BI";
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
              << "          -v | --version                  Version" << std::endl;
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
        else
        { // ALL OTHER ARGUMENT
            print_usage();
            std::string arg = argv[i];
            failure("Unknow argument : " + arg);
        }
    }

    std::vector<std::vector<int>> values{
        std::vector<int> {4500, 7, 7, 8},
        std::vector<int> {4000, 7, 3, 8},
        std::vector<int> {4000, 5, 7, 8},
        std::vector<int> {3500, 5, 7, 5},
        std::vector<int> {3500, 5, 7, 8},
        std::vector<int> {3500, 3, 3, 8},
        std::vector<int> {2500, 3, 7, 5},
    };

    std::vector<float> weights{
        5.0, 3.0, 1.0, 1.0
    };

    float concordanceThreshold = 0.7;

    std::cout << "Values matrix:" << std::endl;
    for (std::vector<int> vec : values) {
        for (int val : vec) 
            std::cout << val << " ";
        std::cout << std::endl;
    }

    std::cout << "Weights matrix:" << std::endl;
    for (float val : weights)
        std::cout << val << " ";
    std::cout << std::endl;

    // Electre el(values, weights, concordanceThreshold);

    return 0;
}