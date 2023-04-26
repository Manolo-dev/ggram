#include <options.h>
#include <iostream>
#include <custom_exceptions.h>
#include <string>
#include <vector>

typedef std::vector<char const *> ParamList;

namespace Options {

const OptionDescription* tryGettingOptionFromID(char const *id) {
    for(const OptionDescription &opt : OptionList) {
        for (int i=0; i<opt.id_number; i++) {
            if(strcmp(id, opt.id_list[i]) == 0) {
                return &opt;
            }
        }
    }
    return nullptr;
}

void tryOption(OptionDescription const *option_ptr, const std::string &option_id, const ParamList &param_list, Configuration &cfg) {
    try {
        (*option_ptr)(param_list, cfg);
    } catch (CustomExceptions::OptionError& except) {
        if (option_id == "") {
            throw CustomExceptions::OptionError("T'as fait de la merde avec la default option");
        } else {
            throw CustomExceptions::OptionError("-" + option_id + ": " + except.what() + "\n  (type \".\\ggram --help " + option_id + "\" for more information on how to use this option)\n");
        }
    }
}

// TODO : tweak a few things to make it exception safe
void handleOptions(int argc, char const *argv[], Configuration &cfg) {
    std::string option_id = "";
    OptionDescription const *option_ptr = &defaultOption_description;
    ParamList param_list = {};
    
    for(uint32_t i = 1; i != argc; ++i) { // TODO: why is it an uint32_t ?
        char const * arg = argv[i];
        if (arg[0] == '-') {
            OptionDescription const *new_option_ptr = tryGettingOptionFromID(&arg[1]);

            if(new_option_ptr != nullptr) {
                tryOption(option_ptr, option_id, param_list, cfg);

                option_id = (std::string)(&arg[1]);
                option_ptr = new_option_ptr;
                param_list = {};
            } else {
                throw CustomExceptions::OptionError("Unknown option: " + std::string(arg));
            }
        } else {
            param_list.push_back(arg);
        }
    }
    tryOption(option_ptr, option_id, param_list, cfg);
}


void eddyMalou(const ParamList &param_list, Configuration &cfg) {
    if (param_list.size() != 0) {
        throw CustomExceptions::OptionError("Too many parameters");
    } else {
        std::cout << "On ne peut pas parler de politique administrative scientifique, le colloque à l'égard de la complexité doit vanter les encadrés avec la formule 1+(2x5), mais oui. Pour emphysiquer l'animalculisme, la congolexicomatisation par rapport aux diplomaties peut aider le conpemdium autour des gens qui connaissent beaucoup de choses, tu sais ça." << std::endl;
        exit(1);
    }
}

void help(const ParamList &param_list, Configuration &cfg) {
    if (param_list.size() > 1) {
        throw CustomExceptions::OptionError("Too many parameters");
    } else if (param_list.size() == 1) {

        OptionDescription const *opt = tryGettingOptionFromID(param_list[0]);
        if(opt != nullptr) {
            opt->print();
        } else {
            throw CustomExceptions::OptionError("Unknown option \"" + std::string(param_list[0]) + "\"");
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
        throw CustomExceptions::OptionError("Too many parameters");
    } else {
        std::cout << "version 0.0.1" << std::endl;
        exit(1);
    }
}

void inputFile(const ParamList &param_list, Configuration &cfg) {
    if (param_list.size() == 0) {
        throw CustomExceptions::OptionError("Missing parameter");
    } else if (param_list.size() == 1) {
        cfg.input_file = param_list[0];
    } else {
        throw CustomExceptions::OptionError("Too many parameters");
    }
}

void outputFile(const ParamList &param_list, Configuration &cfg) {
    if (param_list.size() == 0) {
        throw CustomExceptions::OptionError("Missing parameter");
    } else if (param_list.size() == 1) {
        cfg.input_file = param_list[0];
    } else {
        throw CustomExceptions::OptionError("Too many parameters");
    }
}



// Called with what is before the fisrt option id
void defaultOption(const ParamList &param_list, Configuration &cfg) {
    
}

}