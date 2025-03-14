/**
 * @file src/parsing/hexadecimal_parser.cpp
 * @brief Implementation of the HexadecimalParser class
 */

#include "parsing/hexadecimal_parser.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

constexpr char* RED = "\033[31m";
constexpr char* RESET = "\033[0m";

std::vector<double> HexadecimalParser::parse_out(std::string& input) {
	std::string substring = "";

	// finding first line break
	size_t found = input.find_first_of("\n");

	if (found == std::string::npos) {
		throw std::runtime_error("Failed to read the input string : no newline");
	}
	substring = input.substr(0, found);

	found = substring.find("Cycles");

	if (found == std::string::npos) {
		throw std::runtime_error("Failed to read the input string : could not find the Cycles header");
	}

	size_t col_pos = substring.find_first_of(":", found);

	if (col_pos == std::string::npos) {
		throw std::runtime_error("Failed to read the input string : could not read the cycles");
	}

	substring = substring.substr(col_pos + 1, substring.length());

	double cycles = std::stod(substring);
	if (cycles > MAX_CYCLES) {
		// throw std::runtime_error("Failed to read the input string : cycles are too high");
		std::cerr << RED << "Warning: cycles are too high : " << cycles << " > " << MAX_CYCLES << RESET << '\n';
		return {1.0};
	}
	return {cycles / MAX_CYCLES};
}

std::vector<double> HexadecimalParser::parse_in(std::string& input_ref) {
	// Parse each line as hexadecimal
	std::string input = input_ref;
	std::vector<double> result;

	// Skip the cycles line
	size_t found = input.find_first_of("\n");
	if (found == std::string::npos) {
		throw std::runtime_error("Failed to read the input string : could not find newline");
	}

	input = input.substr(found + 1);

	// Skip the next line
	input = input.substr(input.find_first_of("\n") + 1);

	// For each line, convert each pair of hex characters into a double
	std::istringstream iss(input);
	std::string line;
	while (std::getline(iss, line)) {
		// if line is empty, skip it
		if (line.empty()) {
			continue;
		}

		// Read each pair of hex characters
		for (size_t i = 0; i < line.size(); i += 3) {
			std::string hex_byte = line.substr(i, 2);
			uint8_t byte = std::stoi(hex_byte, nullptr, 16);
			result.push_back(byte / 255.0);
		}
	}

	return result;
}
