/**
 * @file src/parsing/xor_parser.cpp
 * @brief Implementation of the XORParser class
 */

#include <vector>

#include "parsing/xor_parser.hpp"

std::vector<float> XORParser::parse_in(std::string& input) {
	std::vector<float> res = {0.0, 0.0};
	if (input.length() < 3) {
		return res;
	}
	if (input[0] == '1') {
		res[0] = 1.0;
	}
	if (input[2] == '1') {
		res[1] = 1.0;
	}
	return res;
}

std::vector<float> XORParser::parse_out(std::string& input) {
	std::vector<float> res = {0.0};
	if (input.length() < 5) {
		return res;
	}
	if (input[4] == '1') {
		res[0] = 1.0;
	}
	return res;
}