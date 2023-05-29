#pragma once

#include "regex/types.hpp"
#include <string>
#include <vector>
#include <filesystem>

namespace InputHandler {
	// These lists contains the arguments recieved fora given parameter
	using ArgList = std::vector<std::string>;
	using LexerRule = std::tuple<std::string, Matcher, Parser>;

	// The configuration, will contain everything we got from the input arguments
	struct Configuration {
		std::string input_filename;
		std::filesystem::path output_filepath_cpp = "parser.cpp";
		std::filesystem::path output_filepath_hpp = "parser.hpp";
		std::vector<LexerRule> lex_ggram_file;
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
};