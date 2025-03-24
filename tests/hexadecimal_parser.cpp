/**
 * @file tests/hexadecimal_parser.cpp
 * @brief Used to test the assembly parser that reads hexadecimal and converts it into a neural network input.
 */

#include "parsing/hexadecimal_parser.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <string>
#include <vector>

/**
 * @brief Test to check if the parser can convert a single line of instruction to binary
 */
TEST(BinaryWithSplit_Parser, Single_Instruction) {
	std::string hexa = ";; Cycles: x\n\n"
					   "FF FF\n";
	std::vector<float> expected = {1.0, 1.0};
	std::vector<float> result = HexadecimalParser().parse_in(hexa);
	EXPECT_EQ(result, expected);

	hexa = ";; Cycles: x\n\n"
		   "FF EE 11 0B\n";
	expected = {1.0, (float)0xEE / 255.0, (float)0x11 / 255.0, (float)0x0B / 255.0};
	result = HexadecimalParser().parse_in(hexa);
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

	std::vector<float> expected;
	std::vector<float> inst1 = {(float)0xAA / 255.0, (float)0xBB / 255.0};
	std::vector<float> inst2 = {(float)0xC5 / 255.0, (float)0xE4 / 255.0, (float)0xB8 / 255.0};
	std::vector<float> inst3 = {0.0};
	std::vector<float> inst4 = {(float)0x71 / 255.0, (float)0xD9 / 255.0};
	expected.insert(expected.end(), inst1.begin(), inst1.end());
	expected.insert(expected.end(), inst2.begin(), inst2.end());
	expected.insert(expected.end(), inst3.begin(), inst3.end());
	expected.insert(expected.end(), inst4.begin(), inst4.end());
	std::vector<float> result = HexadecimalParser().parse_in(instructions);
	EXPECT_EQ(result, expected);
}