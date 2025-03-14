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
	std::vector<double> expected = {1.0, 1.0};
	std::vector<double> result = HexadecimalParser().parse_in(hexa);
	EXPECT_EQ(result, expected);

	hexa = ";; Cycles: x\n\n"
		   "FF EE 11 0B\n";
	expected = {1.0, (double)0xEE / 255.0, (double)0x11 / 255.0, (double)0x0B / 255.0};
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

	std::vector<double> expected;
	std::vector<double> inst1 = {(double)0xAA / 255.0, (double)0xBB / 255.0};
	std::vector<double> inst2 = {(double)0xC5 / 255.0, (double)0xE4 / 255.0, (double)0xB8 / 255.0};
	std::vector<double> inst3 = {0.0};
	std::vector<double> inst4 = {(double)0x71 / 255.0, (double)0xD9 / 255.0};
	expected.insert(expected.end(), inst1.begin(), inst1.end());
	expected.insert(expected.end(), inst2.begin(), inst2.end());
	expected.insert(expected.end(), inst3.begin(), inst3.end());
	expected.insert(expected.end(), inst4.begin(), inst4.end());
	std::vector<double> result = HexadecimalParser().parse_in(instructions);
	EXPECT_EQ(result, expected);
}