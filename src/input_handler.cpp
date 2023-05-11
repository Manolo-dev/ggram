#include "input_handler.hpp"
#include "error.hpp"
#include "version.hpp"

namespace InputHandler {

//------------------------------------------------------------------------//
//------------- definition of ParameterHandler's functions ---------------//
//------------------------------------------------------------------------//

void ParameterHandler::update_configuration(const ArgList &arg_list, Configuration &cfg) const {
    (*configuration_updater)(arg_list, cfg);
}

// prints the parameter ids the description associated
std::ostream &ParameterHandler::print(std::ostream &os) const {
    if (short_id != '\0') {
        os << "-" << short_id;
        if (long_id != nullptr) {
            os << ", --" << long_id;
        }
    } else if (long_id != nullptr) {
        os << "--" << long_id;
    } else {
        os << "Not accessible";
    }
    return os << ":\n  " << description << std::endl;
}

std::ostream &operator<<(std::ostream &os, const ParameterHandler &handler) {
    return handler.print(os);
}

//------------------------------------------------------------------------//
//------------------------ Parameter getters -----------------------------//
//------------------------------------------------------------------------//
const ParameterHandler *getHandlerFromParam(const std::string_view &param, std::string &remaining) {
    if (param.size() < 2 || param[0] != '-') {
        remaining = "";
        return nullptr;
    }
    if (param[1] == '-') {
        remaining = "";
        return getHandlerFromLongID(param.substr(2));
    }
    remaining = param.substr(2);
    return getHandlerFromShortID(param[1]);
}

const ParameterHandler *getHandlerFromShortID(const char short_id) {
    for (const ParameterHandler &param : PARAMETER_LIST) {
        if (param.short_id != '\0' && short_id == param.short_id) {
            return &param;
        }
    }
    return nullptr;
}

const ParameterHandler *getHandlerFromLongID(const std::string_view &long_id) {
    for (const ParameterHandler &param : PARAMETER_LIST) {
        if (param.long_id != nullptr && long_id == param.long_id) {
            return &param;
        }
    }
    return nullptr;
}

const ParameterHandler *getHandlerFromAnyID(const std::string_view &id) {

    const ParameterHandler *param_ptr = nullptr;
    if (id.size() == 1) {
        param_ptr = getHandlerFromShortID(id[0]);
    }
    if (param_ptr == nullptr) {
        param_ptr = getHandlerFromLongID(id);
    }
    return param_ptr;
}

/******************************************************************************************/
/*   This is the main function, that detects the parameters and the argument
 * list that    */
/* corresponds to each one, and applies all possible parameter handlers in
 * PARAMETER_LIST */
/*                    in order to update the default configuration */
/******************************************************************************************/
void handleParameters(const std::vector<std::string> &args, Configuration &cfg) {
    if (args.size() == 1) {
        help({}, cfg);
    }
    uint i = 1;
    // Handles default arguments
    while (i < args.size() && args.at(i)[0] != '-') {
        i++;
    }
    ArgList arg_list = {args.begin() + 1, args.begin() + i};
    defaultParameterHandler(arg_list, cfg);

    // Handles named parameters' arguments
    const ParameterHandler *handler_ptr = nullptr;
    std::string remaining;
    while (i < args.size()) {
        // argv[i] is a parameter name, because if not
        // it would have been added to the precedent arg_list
        handler_ptr = getHandlerFromParam(args.at(i), remaining);

        if (handler_ptr == nullptr) {
            throw InputError("Unknown Parameter : " + std::string(args.at(i)));
        }

        arg_list.clear();
        if (!remaining.empty()) {
            arg_list.emplace_back(remaining);
        }
        i++;
        while (i < args.size() && args.at(i)[0] != '-') {
            arg_list.emplace_back(args.at(i));
            i++;
        }

        handler_ptr->update_configuration(arg_list, cfg);
    }
}

void check_arg_list_size(const ArgList &list, const size_t min_val, const size_t max_val) {
    if (list.size() > max_val) {
        throw InputError("Too many arguments");
    }
    if (list.size() < min_val) {
        throw InputError("Missing arguments");
    }
}
// ----------------- Default Parameter Handler ----------------- //
// Special Handler ( doesn't need a structure): called with what is before the
// first named parameter
void defaultParameterHandler(const ArgList &arg_list, Configuration &cfg) {
    check_arg_list_size(arg_list, 0, 3);
    if (arg_list.empty()) {
        return;
    }
    inputFile({arg_list[0]}, cfg);
    if (arg_list.size() == 2) {
        outputFile({arg_list[1]}, cfg);
    } else if (arg_list.size() == 3) {
        outputFile({arg_list[1], arg_list[2]}, cfg);
    }
}

// ------------------------------------------------------------------ //
// -------------- Here come all the parameter handlers -------------- //
// ------------------------------------------------------------------ //

[[noreturn]] void eddyMalou(const ArgList &arg_list, Configuration & /*unused*/) {
    check_arg_list_size(arg_list, 0, 0);
    std::cout << "On ne peut pas parler de politique administrative scientifique,"
              << " le colloque à l'égard de la complexité doit vanter les "
                 "encadrés "
                 "avec la formule 1+(2x5), mais oui."
              << " Pour emphysiquer l'animalculisme, la congolexicomatisation "
                 "par "
                 "rapport aux diplomaties peut aider"
              << " le conpemdium autour des gens qui connaissent beaucoup de "
                 "choses, "
                 "tu sais ça."
              << std::endl;
    exit(0);
}

[[noreturn]] void help(const ArgList &arg_list, Configuration & /*unused*/) {
    check_arg_list_size(arg_list, 0, 1);

    if (arg_list.size() == 1) {
        const ParameterHandler *param_ptr = getHandlerFromAnyID(arg_list[0]);
        if (param_ptr == nullptr) {
            throw InputError("Unknown Parameter :\"" + arg_list[0] + "\"");
        }
        std::cout << *param_ptr;
    } else if (arg_list.empty()) {
        for (const ParameterHandler &param : PARAMETER_LIST) {
            std::cout << param;
        }
    }
    exit(0);
}

[[noreturn]] void version(const ArgList &arg_list, Configuration & /*unused*/) {
    check_arg_list_size(arg_list, 0, 0);
    std::cout << "version" << GGRAM_VERSION << std::endl;
    exit(0);
}

void inputFile(const ArgList &arg_list, Configuration &cfg) {
    check_arg_list_size(arg_list, 1, 1);
    cfg.input_filename = arg_list[0];
}

void outputFile(const ArgList &arg_list, Configuration &cfg) {
    check_arg_list_size(arg_list, 1, 2);
    if (arg_list.size() == 1) {
        std::filesystem::path filepath = arg_list[0];
        if (filepath.has_extension() && filepath.extension() != ".cpp" &&
            filepath.extension() != ".hpp" && filepath.extension() != ".h") {
            throw InputError("Invalid file extention for an output file : '" +
                             filepath.extension().string() + "'");
        }
        const std::string header_extension = (filepath.extension() == "h") ? "h" : "hpp";

        cfg.output_filepath_hpp = filepath.replace_extension(header_extension);
        cfg.output_filepath_cpp = filepath.replace_extension("cpp");

    } else if (arg_list.size() == 2) {
        std::filesystem::path const filepath0 = arg_list[0];
        std::filesystem::path const filepath1 = arg_list[1];
        if (filepath0.extension() != ".cpp" && filepath1.extension() != ".cpp") {
            throw InputError("Neither of the two output files "
                             "specified is a .cpp file !");
        }
        if (filepath0.extension() == ".cpp") {
            cfg.output_filepath_cpp = filepath0;
            cfg.output_filepath_hpp = filepath1;
        } else {
            cfg.output_filepath_cpp = filepath1;
            cfg.output_filepath_hpp = filepath0;
        }
        if (cfg.output_filepath_hpp.extension() != ".hpp" &&
            cfg.output_filepath_hpp.extension() != ".h") {
            throw InputError("Neither of the two output files specified is a "
                             ".hpp/.h file !");
        }
    }
}

void resultParserType(const ArgList &arg_list, Configuration &cfg) {
    check_arg_list_size(arg_list, 1, 1);
    const std::string arg = arg_list[0];
    if (arg == "TRY_CATCHS" || arg == "tc") {
        cfg.result_type = ResultType::TRY_CATCHS;
    } else if (arg == "ORS" || arg == "or") {
        cfg.result_type = ResultType::ORS;
    } else if (arg == "ERROR_TOKEN" || arg == "etk") {
        cfg.result_type = ResultType::ERROR_TOKEN;
    } else {
        throw InputError("Invalid argument :" + arg);
    }
}
} // namespace InputHandler
