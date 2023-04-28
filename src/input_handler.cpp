#include <iostream>
#include <string>
#include <vector>

#include "error.hpp"
#include "input_handler.hpp"


namespace InputHandler {
    typedef std::vector<char const *> ArgList;

    void ParameterDescription::set_configuration(const ArgList &arg_list, Configuration &cfg) const {
        (*function)(arg_list, cfg);
    }

    void ParameterDescription::print(char const line_start[]) const {
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

    const ParameterDescription* tryGettingParameterFromShortID(char const *id) {
        for(const ParameterDescription &opt : ParameterList) {
            if(strcmp(id, opt.short_id) == 0) {
                return &opt;
            }
        }
        return nullptr;
    }

    const ParameterDescription* tryGettingParameterFromLongID(char const *id) {
        for(const ParameterDescription &opt : ParameterList) {
            if(strcmp(id, opt.long_id) == 0) {
                return &opt;
            }
        }
        return nullptr;
    }

    const ParameterDescription* tryGettingParameterFromAnyID(char const *id) {
        const ParameterDescription* Parameter_ptr = tryGettingParameterFromShortID(id);
        if (Parameter_ptr == nullptr) {
            Parameter_ptr = tryGettingParameterFromLongID(id);
        }
        return Parameter_ptr;
    }

    void updateConfigurationWithParameter(ParameterDescription const *Parameter_ptr, const std::string &Parameter_id_with_prefix, const ArgList &arg_list, Configuration &cfg) {
        try {
            Parameter_ptr -> set_configuration(arg_list, cfg);
        } catch (ParameterError& except) {
            if (Parameter_id_with_prefix == "") {
                throw ParameterError("T'as fait de la merde avec la default Parameter");
            } else {
                throw ParameterError(
                    Parameter_id_with_prefix + ": " + except.what() +
                    "\n  (type \".\\ggram --help " + Parameter_id_with_prefix + "\" for more information on how to use this Parameter)\n"
                );
            }
        }
    }


    void handleParameters(int argc, char const *argv[], Configuration &cfg) {
        std::string Parameter_id_with_prefix = "";
        ParameterDescription const *Parameter_ptr = &defaultParameter_description;
        ArgList arg_list = {};
        
        for(int i = 1; i != argc; ++i) {
            char const * arg = argv[i];

            if (arg[0] == '-') {
                ParameterDescription const *new_Parameter_ptr;
                if (arg[1] == '-') {
                    new_Parameter_ptr = tryGettingParameterFromLongID(&arg[2]);
                } else {
                    new_Parameter_ptr = tryGettingParameterFromShortID(&arg[1]);
                }

                if(new_Parameter_ptr != nullptr) {
                    updateConfigurationWithParameter(Parameter_ptr, Parameter_id_with_prefix, arg_list, cfg);

                    Parameter_id_with_prefix = std::string(arg);
                    Parameter_ptr = new_Parameter_ptr;
                    arg_list = {};
                } else {
                    throw ParameterError("Unknown Parameter: " + std::string(arg));
                }
            } else {
                arg_list.push_back(arg);
            }
        }
        updateConfigurationWithParameter(Parameter_ptr, Parameter_id_with_prefix, arg_list, cfg);
    }


    void eddyMalou(const ArgList &arg_list, Configuration &cfg) {
        if (arg_list.size() != 0) {
            throw ParameterError("Too many parameters");
        } else {
            std::cout << "On ne peut pas parler de politique administrative scientifique, le colloque à l'égard de la complexité doit vanter les encadrés avec la formule 1+(2x5), mais oui. Pour emphysiquer l'animalculisme, la congolexicomatisation par rapport aux diplomaties peut aider le conpemdium autour des gens qui connaissent beaucoup de choses, tu sais ça." << std::endl;
            exit(1);
        }
    }

    void help(const ArgList &arg_list, Configuration &cfg) {
        if (arg_list.size() > 1) {
            throw ParameterError("Too many parameters");
        } else if (arg_list.size() == 1) {

            ParameterDescription const *opt = tryGettingParameterFromAnyID(arg_list[0]);
            if(opt != nullptr) {
                opt->print();
            } else {
                throw ParameterError("Unknown Parameter \"" + std::string(arg_list[0]) + "\"");
            }
            exit(1);
        } else {
            for(const ParameterDescription &opt : ParameterList) {
                opt.print();
            }
            exit(1);
        }
    }

    void version(const ArgList &arg_list, Configuration &cfg) {
        if (arg_list.size() != 0) {
            throw ParameterError("Too many parameters");
        } else {
            std::cout << "version 0.0.1" << std::endl;
            exit(1);
        }
    }

    void inputFile(const ArgList &arg_list, Configuration &cfg) {
        if (arg_list.size() == 0) {
            throw ParameterError("Missing parameter");
        } else if (arg_list.size() == 1) {
            cfg.input_filename = arg_list[0];
        } else {
            throw ParameterError("Too many parameters");
        }
    }

    void outputFile(const ArgList &arg_list, Configuration &cfg) {
        if (arg_list.size() == 0) {
            throw ParameterError("Missing parameter");
        } else if (arg_list.size() == 1) {
            cfg.output_filename = arg_list[0];
        } else {
            throw ParameterError("Too many parameters");
        }
    }



    // Called with what is before the fisrt Parameter id
    void defaultParameter(const ArgList &arg_list, Configuration &cfg) {
        if (arg_list.size() == 0) {
            return;
        } else if (arg_list.size() == 1) {
            inputFile(arg_list, cfg);
        } else {
            throw ParameterError("Too many parameters");
        }
    }

}
