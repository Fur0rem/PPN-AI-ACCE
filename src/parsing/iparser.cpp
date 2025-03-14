/**
 * @file src/parsing/iparser.cpp
 * @brief Implementation of the IParser interface class.
 */

#include "parsing/iparser.hpp"
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
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

constexpr char* RED = "\033[0;31m";
constexpr char* RESET = "\033[0m";

std::vector<double> IParser::into_neural_network_input(std::vector<double>& parsed, std::vector<size_t>& topology) {
	std::vector<double> result(topology[0], NO_MORE_VALUE);

	// Check for the number of tokens compared to the number of neurons in the first layer (input layer)
	if (parsed.size() + 1 > topology[0]) {
		std::cout << RED << "Warning : Kernel too big to analyse : " << parsed.size() << " > " << topology[0] << RESET << '\n';
		return result;
		// throw std::runtime_error(std::string("Number of tokens (") + std::to_string(parsed.size()) +
		//  ") is greater than the number of neurons in the first layer (" + std::to_string(topology[0]) + ")");
	}
	// Store the number of tokens in the first element
	result[0] = static_cast<double>(parsed.size()) / static_cast<double>(topology[0]);

	for (size_t i = 0; i < parsed.size(); i++) {
		result[i + 1] = parsed[i];
	}
	return result;
}
