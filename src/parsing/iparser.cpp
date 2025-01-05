/**
 * @file src/parsing/iparser.cpp
 * @brief Implementation of the IParser interface class.
 */

#include "parsing/iparser.hpp"
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>

std::tuple<std::vector<double>, std::vector<double>> IParser::parse_from_string(std::string& input) {
	return {parse_in(input), parse_out(input)};
}

std::tuple<std::vector<double>, std::vector<double>> IParser::parse_from_file(std::string& filename) {
	std::ifstream file(filename);
	std::string str;
	std::string input;
	while (std::getline(file, str)) {
		input += str;
		input += "\n";
	}
	return parse_from_string(input);
}

std::array<double, MAX_TOKENS_NN> IParser::into_neural_network_input(std::vector<double>& parsed) {
	assert(parsed.size() + 1 <= MAX_TOKENS_NN);
	std::array<double, MAX_TOKENS_NN> result;
	result[0] = static_cast<double>(parsed.size()) / static_cast<double>(MAX_TOKENS_NN);
	for (size_t i = 0; i < parsed.size(); i++) {
		result[i + 1] = parsed[i];
	}
	for (size_t i = parsed.size() + 1; i < MAX_TOKENS_NN; i++) {
		result[i] = NO_MORE_VALUE;
	}
	return result;
}
