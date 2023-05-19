#pragma once

#include "error/argument_error.hpp"
#include <array>
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

/**
 * This is the part that handles inputs from the user, these are the steps to
 * add a parameter :
 *  - ifthe parameter adds a new information, you have to add this information
 *      to the structure : Configuration , to make it usable in the main program
 *  - declare a UpdateConfigFunction in this file
 *  - create a constexpr ParameterHandler structure just below
 *  - DON'T FORGET TO : add the structure tu the PARAMETER_LIST
 *  - finally define the UpdateConfigFunction in the .cpp file
 *      (and don't forget to verify that length of the ArgList is correct)
 */
namespace InputHandler {
// These lists contains the arguments recieved fora given parameter
using ArgList = std::vector<std::string>;

// The configuration, will contain everything we got from the input arguments
struct Configuration {
    std::string input_filename;
    std::filesystem::path output_filepath_cpp = "parser.cpp";
    std::filesystem::path output_filepath_hpp = "parser.hpp";
};

// UpdateConfigFunction is intended to update the configuration
// given the arguments of the parameter she has responsability over
using UpdateConfigFunction = void (*)(const ArgList &, Configuration &);

/**
 * ParameterHandler takes responsability over a parameter namely -short_id and
 * --long_id and handles the arguments of this parameter to update the
 * configuration he also stores a description of the parameter that can be
 * displayed with the --help forexample
 */
struct ParameterHandler {
    char const short_id;
    char const *long_id;
    char const *description;
    UpdateConfigFunction configuration_updater;

    void update_configuration(const ArgList & /*arg_list*/, Configuration & /*cfg*/) const;
    std::ostream &print(std::ostream & /*os*/) const;
};
std::ostream &operator<<(std::ostream & /*os*/, const ParameterHandler & /*handler*/);

//----------------------------- Handler getters
//----------------------------------//
/**
 * They try to search fora handler in PARAMETER_LIST that deals with the
 * parameter corresponding to the given id, ifit finds it, it returns the
 * parameter handler otherwise it returns a nullptr
 */
const ParameterHandler *getHandlerFromParam(const std::string_view &param, std::string &remaining);
// param is either "-short_id" or "--long_id"
const ParameterHandler *getHandlerFromShortID(char short_id);
const ParameterHandler *getHandlerFromLongID(const std::string_view &long_id);
const ParameterHandler *getHandlerFromAnyID(const std::string_view &id);

/******************************************************************************************/
/*   This is the main function, that detects the parameters and the argument
 * list that    */
/* corresponds to each one, and applies all possible parameter handlers in
 * PARAMETER_LIST */
/*                    in order to update the default configuration */
/******************************************************************************************/
bool handleParameters(const std::vector<std::string> &args, Configuration &cfg) noexcept;

// ----------------- Default Parameter Handler ----------------- //
// Special Handler (doesn't need a structure): called with what is before the
// first named parameter
void defaultParameterHandler(const ArgList &arg_list, Configuration &cfg);

// ------------------------------------------------------------------ //
// -------------- Here come all the parameter handlers -------------- //
// ------------------------------------------------------------------ //

// ------------ Eddy Malou Parameter ------------ //
[[noreturn]] void eddyMalou(const ArgList &arg_list, Configuration & /*unused*/);
constexpr ParameterHandler eddyMalou_description = {
    'e', "congolexicomatisation",
    "Lorsque l'on parle des végétaliens, du végétalisme, "
    "le savoir purement technique paraît soutenir l'estime du savoir provenant "
    "d'une dynamique syncronique, "
    "je vous en prie. Une semaine passée sans parler du peuple c’est errer "
    "sans abri, "
    "autrement dit la compétence autour de l'ergonométrie se résume à "
    "gérer le panafricanisme comparé(e)(s) la rénaque, c’est clair.",
    &eddyMalou};

// ------------ Help Parameter ------------ //
[[noreturn]] void help(const ArgList &arg_list, Configuration & /*unused*/);
constexpr ParameterHandler help_description = {
    'h', "help", "Show all possible parameters with their description", &help};

// ------------ Version Parameter ------------ //
[[noreturn]] void version(const ArgList &arg_list, Configuration & /*unused*/);
constexpr ParameterHandler version_description = {
    'v', "version", "Show which version of ggram you're using", &version};

// ------------ Input Parameter ------------ //
void inputFile(const ArgList &arg_list, Configuration &cfg);
constexpr ParameterHandler inputFile_description = {
    'f', nullptr, "takes the input .gg file, to be treated", &inputFile};

// ------------ Output Parameter ------------ //
void outputFile(const ArgList &arg_list, Configuration &cfg);
constexpr ParameterHandler outputFile_description = {
    'o', nullptr, "takes the output .cpp file, forthe result (default : parser.cpp)", &outputFile};

// This is the list of all parameter handlers that will be tested on the input
constexpr std::array<ParameterHandler, 6> PARAMETER_LIST = {
    eddyMalou_description, help_description, version_description, inputFile_description,
    outputFile_description};
} // namespace InputHandler
