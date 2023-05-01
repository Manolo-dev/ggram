#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <filesystem>

/**
 * This is the part that handles inputs from the user, these are the steps to add a parameter : 
 *  - if the parameter adds a new information, you have to add this information 
 *      to the structure : Configuration , to make it usable in the main program
 *  - declare a update_config_function in this file 
 *  - create a constexpr ParameterHandler structure just below
 *  - DON'T FORGET TO : add the structure tu the PARAMETER_LIST
 *  - finally define the update_config_function in the .cpp file 
 *      ( and don't forget to verify that length of the ArgList is correct  )
 */
namespace InputHandler {
    // These lists contains the arguments recieved for a given parameter
    typedef std::vector<std::string> ArgList;

    enum class ResultType { ORS, TRY_CATCHS };
    // The configuration, will contain everything we got from the input arguments
    struct Configuration {
        std::string input_filename = "";
        std::filesystem::path output_filepath_cpp = "parser.cpp";
        std::filesystem::path output_filepath_hpp = "parser.hpp";
        ResultType result_type = ResultType::TRY_CATCHS;
    };

    // update_config_function is intended to update the configuration
    // given the arguments of the parameter she has responsability over
    typedef void (*update_config_function)(const ArgList &, Configuration &);
    
    /**
     * ParameterHandler takes responsability over a parameter namely -short_id and --long_id
     * and handles the arguments of this parameter to update the configuration
     * he also stores a description of the parameter that can be displayed with the --help for example
     */
    struct ParameterHandler {
        char const short_id;
        char const *long_id;
        char const *description;
        update_config_function configuration_updater;

        void update_configuration(const ArgList &, Configuration &) const;
        std::ostream& print(std::ostream&) const;
    };
    std::ostream& operator<<( std::ostream&, const ParameterHandler&);
    
    //----------------------------- Handler getters ----------------------------------//
    /** 
     * They try to search for a handler in PARAMETER_LIST that deals with the parameter
     * corresponding to the given id, if it finds it, it returns the parameter handler
     * otherwise it returns a nullptr
    */
    const ParameterHandler* getHandlerFromParam(const char * const param, std::string& remaining); // param is either "-short_id" or "--long_id"
    const ParameterHandler* getHandlerFromShortID(const char short_id);
    const ParameterHandler* getHandlerFromLongID(const char * const long_id);
    const ParameterHandler* getHandlerFromAnyID(const std::string& id);

    
    /******************************************************************************************/
    /*   This is the main function, that detects the parameters and the argument list that    */
    /* corresponds to each one, and applies all possible parameter handlers in PARAMETER_LIST */
    /*                    in order to update the default configuration                        */
    /******************************************************************************************/
    void handleParameters(int argc, char const *argv[], Configuration& cfg);

    // ----------------- Default Parameter Handler ----------------- //
    // Special Handler ( doesn't need a structure ): called with what is before the first named parameter
    void defaultParameterHandler(const ArgList &arg_list, Configuration &cfg);

    // ------------------------------------------------------------------ //
    // -------------- Here come all the parameter handlers -------------- //
    // ------------------------------------------------------------------ //

    // ------------ Eddy Malou Parameter ------------ //
    void eddyMalou(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterHandler eddyMalou_description = {
        'e',
        "congolexicomatisation",
        "Lorsque l'on parle des végétaliens, du végétalisme, "
        "le savoir purement technique paraît soutenir l'estime du savoir provenant d'une dynamique syncronique, "
        "je vous en prie. Une semaine passée sans parler du peuple c’est errer sans abri, "
        "autrement dit la compétence autour de l'ergonométrie se résume à "
        "gérer le panafricanisme comparé(e)(s) la rénaque, c’est clair.",
        &eddyMalou
    };

    // ------------ Help Parameter ------------ //
    void help(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterHandler help_description = {
        'h',
        "help",
        "Show all possible parameters with their description",
        &help
    };

    // ------------ Version Parameter ------------ //
    void version(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterHandler version_description = {
        'v',
        "version",
        "Show which version of ggram you're using",
        &version
    };

    // ------------ Input Parameter ------------ //
    void inputFile(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterHandler inputFile_description = {
        'f',
        nullptr,
        "takes the input .gg file, to be treated",
        &inputFile
    };

    // ------------ Output Parameter ------------ //
    void outputFile(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterHandler outputFile_description = {
        'o',
        nullptr,
        "takes the output .cpp file, for the result (default : parser.cpp)",
        &outputFile
    };

    // ------------ Result Type Parameter ------------ //
    void resultParserType(const ArgList &arg_list, Configuration &cfg);
    constexpr ParameterHandler resultParserType_description = {
        'r',
        "restype",
        "Control the type of code you want for the parser.\n"
        "    Values : ORS / or, TRY_CATCHS / tc (default)",
        &resultParserType
    };


    // This is the list of all parameter handlers that will be tested on the input
    constexpr ParameterHandler PARAMETER_LIST[] = {
        eddyMalou_description,
        help_description,
        version_description,
        inputFile_description,
        outputFile_description,
        resultParserType_description
    };
}
