#include "rawbinary_parser.hpp"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>

std::vector<double> RawBinaryParser::parse_from_string(std::string& input) {
	// Create a command that will call nasm to assemble the input string

	// Add BITS 64 to the input string to assemble it as a 64-bit binary
	input = "BITS 64\n" + input;
	std::string command = "echo '" + input + "' > /tmp/input.asm && nasm -f bin /tmp/input.asm -o /tmp/input.bin";
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

	// pop the newline in the end
	hex.pop_back();

	// Convert the hexadecimal string into a vec of uint8_t (pair of 2 hex characters)
	std::vector<uint8_t> hex_bytes;
	for (size_t i = 0; i < hex.size(); i += 2) {
		uint8_t byte = std::stoi(hex.substr(i, 2), nullptr, 16);
		hex_bytes.push_back(byte);
	}

	// Convert the hexadecimal bytes into a vector of doubles per bit
	std::vector<double> result;
	for (uint8_t hex_byte : hex_bytes) {
		for (int j = 7; j >= 0; j--) {
			result.push_back((hex_byte >> j) & 1);
		}
	}

	return result;
}
