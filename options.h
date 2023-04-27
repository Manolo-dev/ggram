#pragma once

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <cstring>

typedef std::vector<char const *> ParamList;

namespace Options {

struct Configuration {
    std::string input_filename = "";
    std::string output_filename = "parser.cpp";
};

// I do not use strings and vectors to make it literal so we can have everything constexpr
struct OptionDescription {
    char const *short_id;
    char const *long_id;
    char const *description;
    void (*function)(const ParamList &, Configuration &); // takes the list of parameters and the actual configuration
    
    void operator()(const ParamList &param_list, Configuration &cfg) const {
        (*function)(param_list, cfg);
    }

    void print(char const line_start[] = "") const {
        if (short_id[0] != 0) {
            std::cout << line_start << "-" << short_id;
            if (long_id[0] != 0) {
                std::cout << line_start << ", --" << long_id;
            }
        } else if (long_id[0] != 0) {
            std::cout << line_start << "--" << long_id;
        } else {
            std::cout << "when you dont put any command before";
        }
        std::cout << ":\n" << line_start << "  " << description << std::endl;
    }
};


const OptionDescription* tryGettingOptionFromShortID(char const *id);
const OptionDescription* tryGettingOptionFromLongID(char const *id);
const OptionDescription* tryGettingOptionFromAnyID(char const *id);

void handleOptions(int argc, char const *argv[], Configuration& cfg);



// ------------------------------- //
// -- Here come all the options -- //
// ------------------------------- //


// ------------ Eddy Malou option ------------ //
void eddyMalou(const ParamList &param_list, Configuration &cfg);
constexpr OptionDescription eddyMalou_description = {
    "eddyMalou",
    "congolexicomatisation",
    "Lorsque l'on parle des végétaliens, du végétalisme, le savoir purement technique paraît soutenir l'estime du savoir provenant d'une dynamique syncronique, je vous en prie. Une semaine passée sans parler du peuple c’est errer sans abri, autrement dit la compétence autour de l'ergonométrie se résume à gérer le panafricanisme comparé(e)(s) la rénaque, c’est clair.",
    &eddyMalou
};

// ------------ Help option ------------ //
void help(const ParamList &param_list, Configuration &cfg);
constexpr OptionDescription help_description = {
    "h",
    "help",
    "Show all possible options",
    &help
};

// ------------ Version option ------------ //
void version(const ParamList &param_list, Configuration &cfg);
constexpr OptionDescription version_description = {
    "v",
    "version",
    "Show which version of ggram you're using",
    &version
};

// ------------ Input option ------------ //
void inputFile(const ParamList &param_list, Configuration &cfg);
constexpr OptionDescription inputFile_description = {
    "f",
    "",
    "Description yet to be written",
    &inputFile
};

// ------------ Output option ------------ //
void outputFile(const ParamList &param_list, Configuration &cfg);
constexpr OptionDescription outputFile_description = {
    "o",
    "",
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
    "",
    "",
    "Same as -f",
    &defaultOption
};

}