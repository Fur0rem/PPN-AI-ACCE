/**
 * @file src/parsing/binary_with_split_parser.cpp
 * @brief Implementation of the BinaryWithSplitParser class
 */

#include "parsing/binary_with_split_parser.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<double> BinaryWithSplitParser::parse_out(std::string& input) {
	std::string substring = "";

	// finding first line break
	size_t found = input.find_first_of("\n");

	if (found == std::string::npos) {
		throw std::runtime_error("Failed to assemble the input string");
	}
	substring = input.substr(0, found);

	found = substring.find("Cycles");

	if (found == std::string::npos) {
		throw std::runtime_error("Failed to assemble the input string");
	}

	size_t col_pos = substring.find_first_of(":", found);

	if (col_pos == std::string::npos) {
		throw std::runtime_error("Failed to assemble the input string");
	}

	substring = substring.substr(col_pos + 1, substring.length());

	double cycles = std::stod(substring);

	return {cycles};
}

std::vector<double> BinaryWithSplitParser::parse_in(std::string& input_ref) {
	// Convert it to raw binary but split each instruction with a 0.5
	std::string input = input_ref;
	std::vector<double> result;

	// Split the input string into instructions
	std::vector<std::string> instructions;
	std::string current_instruction;
	for (char c : input) {
		if (c == '\n') {
			// printf("current_instruction : %s\n", current_instruction.c_str());
			if (current_instruction == "BITS 64") {
				current_instruction.clear();
				continue;
			}
			if (current_instruction[0] == ';') {
				current_instruction.clear();
				continue;
			}
			// if it's a label
			if (current_instruction[current_instruction.size() - 1] == ':') {
				current_instruction.clear();
				continue;
			}
			if (current_instruction.empty()) {
				continue;
			}
			instructions.push_back(current_instruction);
			current_instruction.clear();
		}
		else {
			current_instruction += c;
		}
	}
	if (!current_instruction.empty()) {
		instructions.push_back(current_instruction);
	}

	// Convert each instruction to binary
	for (const std::string& instruction : instructions) {
		// Create a command that will call nasm to assemble the input string

		// Add BITS 64 to the input string to assemble it as a 64-bit binary
		std::string full_instruction = "BITS 64\n" + instruction;
		std::cout << "instruction : " << full_instruction << std::endl;
		std::string command = "echo '" + full_instruction + "' > /tmp/input.asm && nasm -f bin /tmp/input.asm -o /tmp/input.bin";
		int cmd_res = system(command.c_str());
		if (cmd_res != 0) {
			throw std::runtime_error("Failed to assemble the input string");
		}

		// Read the assembled binary file
		std::ifstream file("/tmp/input.bin", std::ios::binary);
		std::vector<uint8_t> binary((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// Use xxd to convert the binary file to a hexadecimal string
		std::string xxd_command = "xxd -p /tmp/input.bin";
		FILE* xxd_pipe = popen(xxd_command.c_str(), "r");
		if (xxd_pipe == nullptr) {
			throw std::runtime_error("Failed to run xxd");
		}

		std::string hex;
		char buffer[1024];
		while (fgets(buffer, sizeof(buffer), xxd_pipe) != nullptr) {
			hex += buffer;
		}
		pclose(xxd_pipe);

		std::cout << hex << std::endl;

		// pop the newline in the end
		hex.pop_back();

		// Convert the hexadecimal string into a vec of uint8_t (pair of 2 hex characters)
		std::vector<uint8_t> hex_bytes;
		for (size_t i = 0; i < hex.size(); i += 2) {
			std::cout << hex.substr(i, 2) << std::endl;
			uint8_t byte = std::stoi(hex.substr(i, 2), nullptr, 16);
			hex_bytes.push_back(byte);
		}

		// Convert the hexadecimal bytes into a vector of doubles per bit
		for (uint8_t hex_byte : hex_bytes) {
			for (int j = 7; j >= 0; j--) {
				result.push_back((hex_byte >> j) & 1);
			}
		}

		// Add a split
		result.push_back(0.5);
	}

	return result;
}
