#include <iostream>
#include <string>
#include <vector>

#include "error.hpp"
#include "options.hpp"

typedef std::vector<char const *> ParamList;

namespace Options {

const OptionDescription* tryGettingOptionFromShortID(char const *id) {
    for(const OptionDescription &opt : OptionList) {
        if(strcmp(id, opt.short_id) == 0) {
            return &opt;
        }
    }
    return nullptr;
}

const OptionDescription* tryGettingOptionFromLongID(char const *id) {
    for(const OptionDescription &opt : OptionList) {
        if(strcmp(id, opt.long_id) == 0) {
            return &opt;
        }
    }
    return nullptr;
}

const OptionDescription* tryGettingOptionFromAnyID(char const *id) {
    const OptionDescription* option_ptr = tryGettingOptionFromShortID(id);
    if (option_ptr == nullptr) {
        tryGettingOptionFromLongID(id);
    }
    return option_ptr;
}

void tryOption(OptionDescription const *option_ptr, const std::string &option_id_with_prefix, const ParamList &param_list, Configuration &cfg) {
    try {
        (*option_ptr)(param_list, cfg);
    } catch (OptionError& except) {
        if (option_id_with_prefix == "") {
            throw OptionError("T'as fait de la merde avec la default option");
        } else {
            throw OptionError(
                option_id_with_prefix + ": " + except.what() +
                "\n  (type \".\\ggram --help " + option_id_with_prefix + "\" for more information on how to use this option)\n"
            );
        }
    }
}


void handleOptions(int argc, char const *argv[], Configuration &cfg) {
    std::string option_id_with_prefix = "";
    OptionDescription const *option_ptr = &defaultOption_description;
    ParamList param_list = {};
    
    for(int i = 1; i != argc; ++i) { // TODO: why is it an uint32_t ?
        char const * arg = argv[i];

        if (arg[0] == '-') {
            OptionDescription const *new_option_ptr;
            if (arg[1] == '-') {
                new_option_ptr = tryGettingOptionFromLongID(&arg[2]);
            } else {
                new_option_ptr = tryGettingOptionFromShortID(&arg[1]);
            }

            if(new_option_ptr != nullptr) {
                tryOption(option_ptr, option_id_with_prefix, param_list, cfg);

                option_id_with_prefix = (std::string)(arg);
                option_ptr = new_option_ptr;
                param_list = {};
            } else {
                throw OptionError("Unknown option: " + std::string(arg));
            }
        } else {
            param_list.push_back(arg);
        }
    }
    tryOption(option_ptr, option_id_with_prefix, param_list, cfg);
}


void eddyMalou(const ParamList &param_list, Configuration &cfg) {
    if (param_list.size() != 0) {
        throw OptionError("Too many parameters");
    } else {
        std::cout << "On ne peut pas parler de politique administrative scientifique, le colloque à l'égard de la complexité doit vanter les encadrés avec la formule 1+(2x5), mais oui. Pour emphysiquer l'animalculisme, la congolexicomatisation par rapport aux diplomaties peut aider le conpemdium autour des gens qui connaissent beaucoup de choses, tu sais ça." << std::endl;
        exit(1);
    }
}

void help(const ParamList &param_list, Configuration &cfg) {
    if (param_list.size() > 1) {
        throw OptionError("Too many parameters");
    } else if (param_list.size() == 1) {

        OptionDescription const *opt = tryGettingOptionFromAnyID(param_list[0]);
        if(opt != nullptr) {
            opt->print();
        } else {
            throw OptionError("Unknown option \"" + std::string(param_list[0]) + "\"");
        }
        exit(1);
    } else {
        for(const OptionDescription &opt : OptionList) {
            opt.print();
        }
        exit(1);
    }
}

void version(const ParamList &param_list, Configuration &cfg) {
    if (param_list.size() != 0) {
        throw OptionError("Too many parameters");
    } else {
        std::cout << "version 0.0.1" << std::endl;
        exit(1);
    }
}

void inputFile(const ParamList &param_list, Configuration &cfg) {
    if (param_list.size() == 0) {
        throw OptionError("Missing parameter");
    } else if (param_list.size() == 1) {
        cfg.input_filename = param_list[0];
    } else {
        throw OptionError("Too many parameters");
    }
}

void outputFile(const ParamList &param_list, Configuration &cfg) {
    if (param_list.size() == 0) {
        throw OptionError("Missing parameter");
    } else if (param_list.size() == 1) {
        cfg.output_filename = param_list[0];
    } else {
        throw OptionError("Too many parameters");
    }
}



// Called with what is before the fisrt option id
void defaultOption(const ParamList &param_list, Configuration &cfg) {
    if (param_list.size() == 0) {
        return;
    } else if (param_list.size() == 1) {
        inputFile(param_list, cfg);
    } else {
        throw OptionError("Too many parameters");
    }
}

}
