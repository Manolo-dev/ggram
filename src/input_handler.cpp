#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "error.hpp"
#include "input_handler.hpp"

namespace InputHandler {

    //------------------------------------------------------------------------//
    //------------- definition of ParameterHandler's functions ---------------//
    //------------------------------------------------------------------------//

    void ParameterHandler::update_configuration(const ArgList &arg_list, Configuration &cfg) const {
        (*configuration_updater)(arg_list, cfg);
    }

    // prints the parameter ids the description associated 
    std::ostream& ParameterHandler::print(std::ostream& os) const {
        if(short_id != nullptr) {
            os << "-" << short_id;
            if(long_id != nullptr) {
                os << ", --" << long_id;
            }
        } else if(long_id != nullptr) {
            os << "--" << long_id;
        } else {
            if(this == &defaultParameterHandler){
                os << "First arguments";
            } else {
                os << "Not accessible";
            }
        }
        return os << ":\n  " << description << std::endl;
    }

    std::ostream& operator<<( std::ostream & os, const ParameterHandler& handler ){
        return handler.print(os);
    }

    //------------------------------------------------------------------------//
    //------------------------ Parameter getters -----------------------------//
    //------------------------------------------------------------------------//
    const ParameterHandler* getHandlerFromParam(const char * param_name) {
        assert(param_name != nullptr);
        assert(param_name[0] == '-');
        if(param_name[1] == '-'){
            return getHandlerFromLongID(&param_name[2]);
        } else {
            return getHandlerFromShortID(&param_name[1]);
        }
    }

    const ParameterHandler* getHandlerFromShortID(const char * short_id) {
        if(short_id == nullptr){ return nullptr; }
        for(const ParameterHandler &param : PARAMETER_LIST) {
            if(param.short_id != nullptr && strcmp(short_id, param.short_id) == 0) {
                return &param;
            }
        }
        return nullptr;
    }

    const ParameterHandler* getHandlerFromLongID(const char * long_id) {
        if(long_id == nullptr){ return nullptr; }
        for(const ParameterHandler &param : PARAMETER_LIST) {
            if(param.long_id != nullptr && strcmp(long_id, param.long_id) == 0) {
                return &param;
            }
        }
        return nullptr;
    }

    const ParameterHandler* getHandlerFromAnyID(const char * id) {
        const ParameterHandler* param_ptr = getHandlerFromShortID(id);
        if(param_ptr == nullptr) {
            param_ptr = getHandlerFromLongID(id);
        }
        return param_ptr;
    }

    /******************************************************************************************/
    /*   This is the main function, that detects the parameters and the argument list that    */
    /* corresponds to each one, and applies all possible parameter handlers in PARAMETER_LIST */
    /*                    in order to update the default configuration                        */
    /******************************************************************************************/
    void handleParameters(int argc, const char *argv[], Configuration &cfg) {
        ArgList arg_list = {};        
        int i = 1;
        // Handles default arguments
        while (i < argc && argv[i][0] != '-'){
            arg_list.push_back(argv[i]);
            i++;
        }
        defaultParameterHandler.update_configuration(arg_list, cfg);

        // Handles named parameters' arguments
        const ParameterHandler * handler_ptr;
        while(i < argc) {
            // argv[i] is a parameter name, because if not
            // it would have been added to the precedent arg_list
            handler_ptr = getHandlerFromParam(argv[i]);
            
            if( handler_ptr == nullptr )
                throw InputError("Unknown Parameter: " + std::string(argv[i]));
            
            i++;
            arg_list.clear();
            while (i < argc && argv[i][0] != '-'){
                arg_list.push_back(argv[i]);
                i++;
            }

            handler_ptr -> update_configuration(arg_list, cfg);
        }
    }

    // ------------------------------------------------------------------ //
    // -------------- Here come all the parameter handlers -------------- //
    // ------------------------------------------------------------------ //

    void eddyMalou(const ArgList &arg_list, Configuration &cfg) {
        if(arg_list.size() != 0) {
            throw InputError("Too many arguments");
        } else {
            std::cout << "On ne peut pas parler de politique administrative scientifique,"
                << " le colloque à l'égard de la complexité doit vanter les encadrés avec la formule 1+(2x5), mais oui."
                << " Pour emphysiquer l'animalculisme, la congolexicomatisation par rapport aux diplomaties peut aider"
                << " le conpemdium autour des gens qui connaissent beaucoup de choses, tu sais ça." << std::endl;
            exit(0);
        }
    }

    void help(const ArgList &arg_list, Configuration &cfg) {
        if(arg_list.size() > 1) {
            throw InputError("Too many arguments");
        } else if(arg_list.size() == 1) {

            ParameterHandler const *param_ptr = getHandlerFromAnyID(arg_list[0]);
            if(param_ptr != nullptr) {
                std::cout << *param_ptr;
            } else {
                throw InputError("Unknown Parameter \"" + std::string(arg_list[0]) + "\"");
            }
            exit(0);
        } else {
            for(const ParameterHandler &param : PARAMETER_LIST) {
                std::cout << param;
            }
            exit(0);
        }
    }

    void version(const ArgList &arg_list, Configuration &cfg) {
        if(arg_list.size() != 0) {
            throw InputError("Too many arguments");
        } else {
            std::cout << "version 0.0.1" << std::endl;
            exit(0);
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

    void defaultParameter(const ArgList &arg_list, Configuration &cfg) {
        if(arg_list.size() == 0) {
            return;
        } else if(arg_list.size() == 1) {
            inputFile(arg_list, cfg);
        } else if(arg_list.size() == 2) {
            inputFile({arg_list[0]}, cfg);
            outputFile({arg_list[1]}, cfg);
        } else {
            throw InputError("Too many arguments");
        }
    }

}
