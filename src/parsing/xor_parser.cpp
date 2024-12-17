#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

#include "parsing/xor_parser.hpp"

std::vector<double> XORParser::parse_in(std::string& input) {
	std::vector<double> res = {0.0, 0.0};
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

std::vector<double> XORParser::parse_out(std::string& input) {
	std::vector<double> res = {0.0};
	if (input.length() < 5) {
		return res;
	}
	if (input[4] == '1') {
		res[0] = 1.0;
	}
	return res;
}