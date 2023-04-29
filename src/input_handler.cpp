#include <iostream>
#include <string>
#include <vector>

#include "error.hpp"
#include "input_handler.hpp"

namespace InputHandler {
    typedef std::vector<char const *> ArgList;

    // definition of ParameterDescription's functions
    void ParameterDescription::set_configuration(const ArgList &arg_list, Configuration &cfg) const {
        (*function)(arg_list, cfg);
    }

    void ParameterDescription::print(char const line_start[]) const {
        if(short_id[0] != 0) {
            std::cout << line_start << "-" << short_id;
            if(long_id[0] != 0) {
                std::cout << line_start << ", --" << long_id;
            }
        } else if(long_id[0] != 0) {
            std::cout << line_start << "--" << long_id;
        } else {
            std::cout << "when you dont put any command before";
        }
        std::cout << ":\n" << line_start << "  " << description << std::endl;
    }

    const ParameterDescription* tryGettingParameterFromShortID(char const *id) {
        for(const ParameterDescription &param : PARAMETER_LIST) {
            if(strcmp(id, param.short_id) == 0) {
                return &param;
            }
        }
        return nullptr;
    }

    const ParameterDescription* tryGettingParameterFromLongID(char const *id) {
        for(const ParameterDescription &param : PARAMETER_LIST) {
            if(strcmp(id, param.long_id) == 0) {
                return &param;
            }
        }
        return nullptr;
    }

    const ParameterDescription* tryGettingParameterFromAnyID(char const *id) {
        const ParameterDescription* param_ptr = tryGettingParameterFromShortID(id);
        if(param_ptr == nullptr) {
            param_ptr = tryGettingParameterFromLongID(id);
        }
        return param_ptr;
    }

    void updateConfigurationWithParameter(ParameterDescription const *param_ptr, const std::string &param_id_with_prefix, const ArgList &arg_list, Configuration &cfg) {
        try {
            param_ptr -> set_configuration(arg_list, cfg);
        } catch(InputError& except) {
            if(param_id_with_prefix == "") {
                throw InputError("T'as fait de la merde avec le defaultParameter");
            } else {
                throw InputError(
                    param_id_with_prefix + ": " + except.what() +
                    "\n  (type \".\\ggram --help " + param_id_with_prefix + "\" formore information on how to use this Parameter)\n"
                );
            }
        }
    }

    void handleParameters(int argc, char const *argv[], Configuration &cfg) {
        std::string param_id_with_prefix = "";
        ParameterDescription const *param_ptr = &defaultParameter_description;
        ArgList arg_list = {};
        
        for(int i = 1; i != argc; ++i) {
            char const * arg = argv[i];

            if(arg[0] == '-') {
                ParameterDescription const *new_param_ptr;

                if(arg[1] == '-') {
                    new_param_ptr = tryGettingParameterFromLongID(&arg[2]);
                } else {
                    new_param_ptr = tryGettingParameterFromShortID(&arg[1]);
                }

                if(new_param_ptr != nullptr) {
                    updateConfigurationWithParameter(param_ptr, param_id_with_prefix, arg_list, cfg);

                    param_id_with_prefix = std::string(arg);
                    param_ptr = new_param_ptr;
                    arg_list = {};
                } else {
                    throw InputError("Unknown Parameter: " + std::string(arg));
                }
            } else {
                arg_list.push_back(arg);
            }
        }
        updateConfigurationWithParameter(param_ptr, param_id_with_prefix, arg_list, cfg);
    }

    void eddyMalou(const ArgList &arg_list, Configuration &cfg) {
        if(arg_list.size() != 0) {
            throw InputError("Too many arguments");
        } else {
            std::cout << "On ne peut pas parler de politique administrative scientifique, le colloque à l'égard de la complexité doit vanter les encadrés avec la formule 1+(2x5), mais oui. Pour emphysiquer l'animalculisme, la congolexicomatisation par rapport aux diplomaties peut aider le conpemdium autour des gens qui connaissent beaucoup de choses, tu sais ça." << std::endl;
            exit(1);
        }
    }

    void help(const ArgList &arg_list, Configuration &cfg) {
        if(arg_list.size() > 1) {
            throw InputError("Too many arguments");
        } else if(arg_list.size() == 1) {

            ParameterDescription const *param_ptr = tryGettingParameterFromAnyID(arg_list[0]);
            if(param_ptr != nullptr) {
                param_ptr->print();
            } else {
                throw InputError("Unknown Parameter \"" + std::string(arg_list[0]) + "\"");
            }
            exit(1);
        } else {
            for(const ParameterDescription &param : PARAMETER_LIST) {
                param.print();
            }
            exit(1);
        }
    }

    void version(const ArgList &arg_list, Configuration &cfg) {
        if(arg_list.size() != 0) {
            throw InputError("Too many arguments");
        } else {
            std::cout << "version 0.0.1" << std::endl;
            exit(1);
        }
    }

    void inputFile(const ArgList &arg_list, Configuration &cfg) {
        if(arg_list.size() == 0) {
            throw InputError("Missing argument");
        } else if(arg_list.size() == 1) {
            cfg.input_filename = arg_list[0];
        } else {
            throw InputError("Too many arguments");
        }
    }

    void outputFile(const ArgList &arg_list, Configuration &cfg) {
        if(arg_list.size() == 0) {
            throw InputError("Missing argument");
        } else if(arg_list.size() == 1) {
            cfg.output_filename = arg_list[0];
        } else {
            throw InputError("Too many arguments");
        }
    }

    void resultParserType(const ArgList &arg_list, Configuration &cfg){
        if(arg_list.size() == 1){
            if(strcmp(arg_list[0], "TRY_CATCHS") == 0 || strcmp(arg_list[0], "tc") == 0){
                cfg.result_type = ResultType::TRY_CATCHS;
            } else if(strcmp(arg_list[0], "ORS") == 0 || strcmp(arg_list[0], "or") == 0){
                cfg.result_type = ResultType::ORS;
            } else {
                throw InputError("Invalid argument :" + std::string(arg_list[0]));
            }
        } else if(arg_list.size() > 1) {
            throw InputError("Too many arguments : excepted one");
        } else {
            throw InputError("Missing argument");
        } 
    }

    // Called with what is before the fisrt Parameter id
    void defaultParameter(const ArgList &arg_list, Configuration &cfg) {
        if(arg_list.size() == 0) {
            return;
        } else if(arg_list.size() == 1) {
            inputFile(arg_list, cfg);
        } else {
            throw InputError("Too many arguments");
        }
    }

}
