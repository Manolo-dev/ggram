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
        if(short_id != '\0') {
            os << "-" << short_id;
            if(long_id != nullptr) {
                os << ", --" << long_id;
            }
        } else if(long_id != nullptr) {
            os << "--" << long_id;
        } else {
            os << "Not accessible";
        }
        return os << ":\n  " << description << std::endl;
    }

    std::ostream& operator<<( std::ostream & os, const ParameterHandler& handler ){
        return handler.print(os);
    }

    //------------------------------------------------------------------------//
    //------------------------ Parameter getters -----------------------------//
    //------------------------------------------------------------------------//
    const ParameterHandler* getHandlerFromParam(const char * const param_name,  std::string& remaining) {
        assert(param_name != nullptr);
        assert(param_name[0] == '-');
        if(param_name[1] == '-'){
            remaining = "";
            return getHandlerFromLongID(&param_name[2]);
        } else {
            remaining = std::string(&param_name[2]);
            return getHandlerFromShortID(param_name[1]);
        }
    }

    const ParameterHandler* getHandlerFromShortID(const char short_id) {
        for(const ParameterHandler &param : PARAMETER_LIST) {
            if(param.short_id != '\0' && short_id == param.short_id) {
                return &param;
            }
        }
        return nullptr;
    }

    const ParameterHandler* getHandlerFromLongID(const char * const long_id) {
        if(long_id == nullptr){ return nullptr; }
        for(const ParameterHandler &param : PARAMETER_LIST) {
            if(param.long_id != nullptr && strcmp(long_id, param.long_id) == 0) {
                return &param;
            }
        }
        return nullptr;
    }

    const ParameterHandler* getHandlerFromAnyID(const std::string& id) {

        const ParameterHandler* param_ptr = nullptr;
        if(id.size() == 1){
            param_ptr = getHandlerFromShortID(id[0]);
        }
        if(param_ptr == nullptr) {
            param_ptr = getHandlerFromLongID(id.c_str());
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
            arg_list.emplace_back(argv[i]);
            i++;
        }
        defaultParameterHandler(arg_list, cfg);

        // Handles named parameters' arguments
        const ParameterHandler * handler_ptr;
        while(i < argc) {
            std::string remaining;
            // argv[i] is a parameter name, because if not
            // it would have been added to the precedent arg_list
            handler_ptr = getHandlerFromParam(argv[i], remaining);

            if( handler_ptr == nullptr )
                throw InputError("Unknown Parameter : " + std::string(argv[i]));
            
            arg_list.clear();
            if( remaining != "" ){
                arg_list.emplace_back(remaining);
            }
            i++;
            while (i < argc && argv[i][0] != '-'){
                arg_list.emplace_back(argv[i]);
                i++;
            }

            handler_ptr -> update_configuration(arg_list, cfg);
        }
    }

    void check_arg_list_size(const ArgList& list, const size_t min_val, const size_t max_val ){
        if(list.size() > max_val){
            throw InputError("Too many arguments");
        }
        if(list.size() < min_val){
            throw InputError("Missing arguments");
        }
    }

    bool ends_with(const std::string& s , const std::string& end){
        if(end.size() > s.size()){
            return false;
        }
        for (size_t i = 0; i < end.size(); ++i){
            if(s[s.size() - i - 1] != end[end.size() - i - 1]){
                return false;
            }
        }
        return true;
    }
    // ----------------- Default Parameter Handler ----------------- //
    // Special Handler ( doesn't need a structure ): called with what is before the first named parameter
    void defaultParameterHandler(const ArgList &arg_list, Configuration &cfg) {
        check_arg_list_size(arg_list, 0, 2);
        if(arg_list.size() == 0) {
            return;
        } else if(arg_list.size() == 1) {
            inputFile(arg_list, cfg);
        } else if(arg_list.size() == 2) {
            inputFile({arg_list[0]}, cfg);
            outputFile({arg_list[1]}, cfg);
        }
    }

    // ------------------------------------------------------------------ //
    // -------------- Here come all the parameter handlers -------------- //
    // ------------------------------------------------------------------ //

    void eddyMalou(const ArgList &arg_list, Configuration &cfg) {
        check_arg_list_size(arg_list, 0, 0);
        std::cout << "On ne peut pas parler de politique administrative scientifique,"
            << " le colloque à l'égard de la complexité doit vanter les encadrés avec la formule 1+(2x5), mais oui."
            << " Pour emphysiquer l'animalculisme, la congolexicomatisation par rapport aux diplomaties peut aider"
            << " le conpemdium autour des gens qui connaissent beaucoup de choses, tu sais ça." << std::endl;
        exit(0);
    }

    void help(const ArgList &arg_list, Configuration &cfg) {
        check_arg_list_size(arg_list, 0, 1);

        if(arg_list.size() == 1) {
            const ParameterHandler * param_ptr = getHandlerFromAnyID(arg_list[0]);
            if(param_ptr == nullptr)
                throw InputError("Unknown Parameter :\"" + arg_list[0] + "\"");
            std::cout << *param_ptr;
        } else if(arg_list.size() == 0) {
            for(const ParameterHandler &param : PARAMETER_LIST) {
                std::cout << param;
            }
        }
        exit(0);
    }

    void version(const ArgList &arg_list, Configuration &cfg) {
        check_arg_list_size(arg_list, 0, 0);
        std::cout << "version 0.0.1" << std::endl;
        exit(0);
    }

    void inputFile(const ArgList &arg_list, Configuration &cfg) {
        check_arg_list_size(arg_list, 1, 1);
        cfg.input_filename = arg_list[0];
    }

    void outputFile(const ArgList &arg_list, Configuration &cfg) {
        check_arg_list_size(arg_list, 1, 2);
        if(arg_list.size() == 1){
            std::filesystem::path filepath = arg_list[0];
            if(filepath.has_extension() && filepath.extension() != ".cpp" 
                && filepath.extension() != ".hpp" && filepath.extension() != ".h" ){
                throw InputError("Invalid file extention for an output file : '" + filepath.extension().string() + "'");
            }
            const std::string header_extension =  (filepath.extension() == "h") ? "h" : "hpp";
            
            cfg.output_filepath_hpp = filepath.replace_extension(header_extension);
            cfg.output_filepath_cpp = filepath.replace_extension("cpp");

        } else if(arg_list.size() == 2){
            std::filesystem::path filepath0 = arg_list[0] ; 
            std::filesystem::path filepath1 = arg_list[1] ; 
            if(filepath0.extension() != ".cpp" && filepath1.extension() != ".cpp"  ){
                throw InputError("Neither of the two output files specified is a .cpp file !");
            }
            if(filepath0.extension() == ".cpp"){
                cfg.output_filepath_cpp = filepath0;
                cfg.output_filepath_hpp = filepath1;
            } else {
                cfg.output_filepath_cpp = filepath1;
                cfg.output_filepath_hpp = filepath0;
            }
            if(cfg.output_filepath_hpp.extension() != ".hpp" && cfg.output_filepath_hpp.extension() != ".h" ){
                throw InputError("Neither of the two output files specified is a .hpp/.h file !");
            }
        }
    }


    void resultParserType(const ArgList &arg_list, Configuration &cfg){
        check_arg_list_size(arg_list, 1, 1);
        const std::string arg = arg_list[0];
        if(arg == "TRY_CATCHS" || arg == "tc"){
            cfg.result_type = ResultType::TRY_CATCHS;
        } else if(arg == "ORS" || arg == "or"){
            cfg.result_type = ResultType::ORS;
        } else if(arg == "ERROR_TOKEN" || arg == "etk"){
            cfg.result_type = ResultType::ORS;
        } else {
            throw InputError("Invalid argument :" + arg);
        }
    }
}
