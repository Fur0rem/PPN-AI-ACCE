/**
 * @file src/parsing/hexadecimal_with_split_parser.cpp
 * @brief Implementation of the HexadecimalWithSplitParser class
 */

#include "parsing/hexadecimal_with_split_parser.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

constexpr const char* RED = "\033[0;31m";
constexpr const char* RESET = "\033[0m";

std::vector<float> HexadecimalWithSplitParser::parse_out(std::string& input) {
	std::string substring = "";

	// finding first line break
	size_t found = input.find_first_of('\n');

	if (found == std::string::npos) {
		throw std::runtime_error("Failed to assemble the input string");
	}
	substring = input.substr(0, found);

	found = substring.find("Cycles");

	if (found == std::string::npos) {
		throw std::runtime_error("Failed to assemble the input string");
	}

	size_t col_pos = substring.find_first_of(':', found);

	if (col_pos == std::string::npos) {
		throw std::runtime_error("Failed to assemble the input string");
	}

	substring = substring.substr(col_pos + 1, substring.length());

	float cycles = std::stof(substring);
	return {cycles};
}

std::vector<float> HexadecimalWithSplitParser::parse_in(std::string& input_ref) {
	// Parse each line as hexadecimal
	std::string input = input_ref;
	std::vector<float> result;

	// Skip the cycles line
	size_t found = input.find_first_of('\n');
	if (found == std::string::npos) {
		throw std::runtime_error("Failed to read the input string");
	}

	input = input.substr(found + 1);

	// Skip the next line
	input = input.substr(input.find_first_of('\n') + 1);

	// For each line, convert each pair of hex characters into a float
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
			result.push_back(static_cast<float>(byte) / 280.0F);
		}

		// Split each instruction with a 1.0
		result.push_back(1.0);
	}

	return result;
}
