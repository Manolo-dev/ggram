#pragma once

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <cstring>

typedef std::vector<char const *> ParamList;

namespace Options {

struct Configuration {
    std::string input_file = "";
    std::string output_file = "output.cpp";
};

// I do not use strings and vectors to make it literal so we can have everything constexpr
struct OptionDescription {
    static constexpr int max_id_number = 2;
    const int id_number;
    char const *id_list[max_id_number];
    char const *description;
    void (*function)(const ParamList &, Configuration &); // takes the list of parameters and the actual configuration
    
    void operator()(const ParamList &param_list, Configuration &cfg) const {
        (*function)(param_list, cfg);
    }

    void print(char const line_start[] = "") const {
        std::cout << line_start << "-" << id_list[0];
        for (int i=1; i<id_number; i++) {
            std::cout << ", -" << id_list[i];
        }
        std::cout << ":\n" << line_start << "  " << description << std::endl;
    }
};


const OptionDescription* tryGettingOptionFromID(char const *id);

void handleOptions(int argc, char const *argv[], Configuration& cfg);



// ------------------------------- //
// -- Here come all the options -- //
// ------------------------------- //


// ------------ Eddy Malou option ------------ //
void eddyMalou(const ParamList &param_list, Configuration &cfg);
constexpr OptionDescription eddyMalou_description = {
    2,
    {"EddyMalou", "-congolexicomatisation"},
    "Lorsque l'on parle des végétaliens, du végétalisme, le savoir purement technique paraît soutenir l'estime du savoir provenant d'une dynamique syncronique, je vous en prie. Une semaine passée sans parler du peuple c’est errer sans abri, autrement dit la compétence autour de l'ergonométrie se résume à gérer le panafricanisme comparé(e)(s) la rénaque, c’est clair.",
    &eddyMalou
};

// ------------ Help option ------------ //
void help(const ParamList &param_list, Configuration &cfg);
constexpr OptionDescription help_description = {
    2,
    {"h", "-help"},
    "Show all possible options",
    &help
};

// ------------ Version option ------------ //
void version(const ParamList &param_list, Configuration &cfg);
constexpr OptionDescription version_description = {
    2,
    {"-v", "-version"},
    "Show which version of ggram you're using",
    &version
};

// ------------ Input option ------------ //
void inputFile(const ParamList &param_list, Configuration &cfg);
constexpr OptionDescription inputFile_description = {
    1,
    {"f"},
    "Description yet to be written",
    &inputFile
};

// ------------ Output option ------------ //
void outputFile(const ParamList &param_list, Configuration &cfg);
constexpr OptionDescription outputFile_description = {
    1,
    {"o"},
    "Description yet to be written",
    &outputFile
};


// ------------ Output command ------------ //
constexpr OptionDescription OptionList[] = {
    eddyMalou_description,
    help_description,
    version_description,
    inputFile_description,
    outputFile_description
};



// ------------ Output option ------------ //
// Called with what is before the fisrt id
void defaultOption(const ParamList &param_list, Configuration &cfg);
constexpr OptionDescription defaultOption_description = {
    0,
    {},
    "Description yet to be written",
    &defaultOption
};

}