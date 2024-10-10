// ANSI escape codes for text color
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

#include <iostream>
#include <string.h>
#include "../include/Parser.hpp"
#include "../include/Electre.hpp"
#include "../include/Promethee.hpp"

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

    Parser parser = Parser();

    if (isFile)
    {
        parser.parseFile(filename);
    }
    if (isWeightFile)
    {
        parser.parseWeightFile(filenameWeight);
    }


    std::vector<std::vector<float>> data = parser.getParsedFile();
    std::vector<float> weightsProm = parser.getParsedWeight();

    for(auto const &weight : weightsProm) {
        std::cout << weight << ", " << std::endl;
    }

    Promethee promethee(data, weightsProm);
    promethee.calculatePreferenceMatrix();
    //promethee.printPreferenceMatrix();

    std::cout << std::endl
              << " ✅ Preference done ✅ " << std::endl
              << std::endl;

    promethee.calculateFlows();
    promethee.printLatexOutput();

    return 0;
}