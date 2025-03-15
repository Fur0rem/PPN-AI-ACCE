/**
 * @file src/parsing/iparser.cpp
 * @brief Implementation of the IParser interface class.
 */

#include "parsing/iparser.hpp"
#include <fstream>
#include <string>
#include <tuple>
#include <vector>

std::tuple<std::vector<float>, std::vector<float>> IParser::parse_from_string(std::string& input) {
	return {parse_in(input), parse_out(input)};
}

std::tuple<std::vector<float>, std::vector<float>> IParser::parse_from_file(std::string& filename) {
	std::ifstream file(filename);
	std::string str;
	std::string input;
	while (std::getline(file, str)) {
		input += str;
		input += "\n";
	}
	return parse_from_string(input);
}
