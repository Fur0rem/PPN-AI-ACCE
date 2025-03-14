/**
 * @file tests/hexadecimal_with_split_parser.cpp
 * @brief Used to test the assembly parser that converts hexadecimal into a neural network input by splitting each instruction with a 1.0.
 */

#include "parsing/hexadecimal_with_split_parser.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <string>
#include <vector>

/**
 * @brief Test to check if the parser can convert a single instruction to hexadecimal
 */
TEST(hexadecimalWithSplit_Parser, Single_Instruction) {
	std::string instruction = ";; Cycles: x\n\n"
							  "01 D8\n";

	std::vector<double> expected = {(double)0x01 / 280.0, (double)0xD8 / 280.0};
	expected.push_back(1.0);
	std::vector<double> result = HexadecimalWithSplitParser().parse_in(instruction);
	EXPECT_EQ(result, expected);

	instruction = ";; Cycles: x\n\n"
				  "05 34 12 00 00";
	expected = {(double)0x05 / 280.0, (double)0x34 / 280.0, (double)0x12 / 280.0, 0.0, 0.0};
	expected.push_back(1.0);
	result = HexadecimalWithSplitParser().parse_in(instruction);
	EXPECT_EQ(result, expected);
}

/**
 * @brief Test to check if the parser can convert multiple instructions to hexadecimal
 */
TEST(hexadecimalWithSplit_Parser, Multiple_Instructions) {
	std::string instructions = ";; Cycles: x\n\n"
							   "AA BB\n"
							   "C5 E4 B8\n"
							   "00\n"
							   "71 D9\n";

	std::vector<double> expected;
	std::vector<double> inst1 = {(double)0xAA / 280.0, (double)0xBB / 280.0};
	std::vector<double> inst2 = {(double)0xC5 / 280.0, (double)0xE4 / 280.0, (double)0xB8 / 280.0};
	std::vector<double> inst3 = {0.0};
	std::vector<double> inst4 = {(double)0x71 / 280.0, (double)0xD9 / 280.0};
	expected.insert(expected.end(), inst1.begin(), inst1.end());
	expected.push_back(1.0);
	expected.insert(expected.end(), inst2.begin(), inst2.end());
	expected.push_back(1.0);
	expected.insert(expected.end(), inst3.begin(), inst3.end());
	expected.push_back(1.0);
	expected.insert(expected.end(), inst4.begin(), inst4.end());
	expected.push_back(1.0);
	std::vector<double> result = HexadecimalWithSplitParser().parse_in(instructions);
	EXPECT_EQ(result, expected);
}