/**
 * @file tests/binary_parser.cpp
 * @brief Used to test the assembly parser that reads hexadecimal and converts it to binary directly.
 * It assumes the rawbinary_parser is working correctly.
 */

#include "parsing/binary_parser.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <string>
#include <vector>

/**
 * @brief Convert a hexadecimal vector to a binary vector
 * @param hex The hexadecimal vector
 * @return The binary vector
 */
std::vector<double> binary_from_hexadecimal(std::vector<uint8_t>&& hex) {
	std::vector<double> result;
	for (uint8_t byte : hex) {
		for (int j = 7; j >= 0; j--) {
			result.push_back((byte >> j) & 1);
		}
	}
	return result;
}

/**
 * @brief Test to check if the parser can convert a single line of instruction to binary
 */
TEST(BinaryWithSplit_Parser, Single_Instruction) {
	std::string hexa = ";; Cycles: x\n\n"
					   "FF FF\n";
	std::vector<double> expected = binary_from_hexadecimal({0xFF, 0xFF});
	std::vector<double> result = BinaryParser().parse_in(hexa);
	EXPECT_EQ(result, expected);

	hexa = ";; Cycles: x\n\n"
		   "FF EE 11 0B\n";
	expected = binary_from_hexadecimal({0xFF, 0xEE, 0x11, 0x0B});
	result = BinaryParser().parse_in(hexa);
	EXPECT_EQ(result, expected);
}

/**
 * @brief Test to check if the parser can convert multiple instructions to binary
 */
TEST(BinaryWithSplit_Parser, Multiple_Instructions) {
	std::string instructions = ";; Cycles: x\n\n"
							   "AA BB\n"
							   "C5 E4 B8\n"
							   "00\n"
							   "71 D9\n";

	std::vector<double> expected;
	std::vector<double> inst1 = binary_from_hexadecimal({0xAA, 0xBB});
	std::vector<double> inst2 = binary_from_hexadecimal({0xC5, 0xE4, 0xB8});
	std::vector<double> inst3 = binary_from_hexadecimal({0x00});
	std::vector<double> inst4 = binary_from_hexadecimal({0x71, 0xD9});
	expected.insert(expected.end(), inst1.begin(), inst1.end());
	expected.insert(expected.end(), inst2.begin(), inst2.end());
	expected.insert(expected.end(), inst3.begin(), inst3.end());
	expected.insert(expected.end(), inst4.begin(), inst4.end());
	std::vector<double> result = BinaryParser().parse_in(instructions);
	EXPECT_EQ(result, expected);
}