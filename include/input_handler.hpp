#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cstring>


namespace InputHandler {
    enum class ResultType {
        ORS,
        TRY_CATCHS
    };

    typedef std::vector<char const *> ArgList;

    struct Configuration {
        std::string input_filename = "";
        std::string output_filename = "parser.cpp";
        ResultType result_type = ResultType::TRY_CATCHS;
    };

    typedef void (*set_config_function)(const ArgList &, Configuration &);
    // I do not use strings and vectors to make it literal so we can have everything constexpr
    struct ParameterDescription {
        char const *short_id;
        char const *long_id;
        char const *description;
        set_config_function function; // takes the list of parameters and sets the configuration
        
        void set_configuration(const ArgList &, Configuration &) const;

        void print(char const line_start[] = "") const;
    };


    const ParameterDescription* tryGettingParameterFromShortID(char const *id);
    const ParameterDescription* tryGettingParameterFromLongID(char const *id);
    const ParameterDescription* tryGettingParameterFromAnyID(char const *id);

    void handleParameters(int argc, char const *argv[], Configuration& cfg);



    // ---------------------------------- //
    // -- Here come all the Parameters -- //
    // ---------------------------------- //


    // ------------ Eddy Malou Parameter ------------ //
    void eddyMalou(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterDescription eddyMalou_description = {
        "eddyMalou",
        "congolexicomatisation",
        "Lorsque l'on parle des végétaliens, du végétalisme, le savoir purement technique paraît soutenir l'estime du savoir provenant d'une dynamique syncronique, je vous en prie. Une semaine passée sans parler du peuple c’est errer sans abri, autrement dit la compétence autour de l'ergonométrie se résume à gérer le panafricanisme comparé(e)(s) la rénaque, c’est clair.",
        &eddyMalou
    };

    // ------------ Help Parameter ------------ //
    void help(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterDescription help_description = {
        "h",
        "help",
        "Show all possible Parameters",
        &help
    };

    // ------------ Version Parameter ------------ //
    void version(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterDescription version_description = {
        "v",
        "version",
        "Show which version of ggram you're using",
        &version
    };

    // ------------ Input Parameter ------------ //
    void inputFile(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterDescription inputFile_description = {
        "f",
        "",
        "Description yet to be written",
        &inputFile
    };

    // ------------ Output Parameter ------------ //
    void outputFile(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterDescription outputFile_description = {
        "o",
        "",
        "Description yet to be written",
        &outputFile
    };

    // ------------ Result Type Parameter ------------ //
    void resultParserType(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterDescription resultParserType_description = {
        "r",
        "restype",
        "Control the type of code you want for the parser . Values : ORS / or, TRY_CATCHS / tc",
        &resultParserType
    };



    constexpr ParameterDescription PARAMETER_LIST[] = {
        eddyMalou_description,
        help_description,
        version_description,
        inputFile_description,
        outputFile_description,
        resultParserType_description
    };


    // ------------ Default Parameter ------------ //
    // Called with what is before the fisrt id
    void defaultParameter(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterDescription defaultParameter_description = {
        "",
        "",
        "Same as -f",
        &defaultParameter
    };

}
