/**
 * @file tests/binary_with_split_parser.cpp
 * Used to test the assembly parser that converts it to raw binary but split each instruction with a 0.5.
 * It assumes the rawbinary_parser is working correctly.
 */

#include "parsing/binary_with_split_parser.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <string>
#include <vector>

std::vector<double> binary_from_hexadecimal(std::vector<uint8_t>&& hex) {
	std::vector<double> result;
	for (uint8_t byte : hex) {
		for (int j = 7; j >= 0; j--) {
			result.push_back((byte >> j) & 1);
		}
	}
	return result;
}

TEST(BinaryWithSplit_Parser, Single_Instruction) {
	std::string instruction = "add eax, ebx";
	std::vector<double> expected = binary_from_hexadecimal({0x01, 0xD8});
	expected.push_back(0.5);
	std::vector<double> result = BinaryWithSplitParser().parse_in(instruction);
	EXPECT_EQ(result, expected);

	instruction = "add eax, 0x1234";
	expected = binary_from_hexadecimal({0x05, 0x34, 0x12, 0x00, 0x00});
	expected.push_back(0.5);
	result = BinaryWithSplitParser().parse_in(instruction);
	EXPECT_EQ(result, expected);
}

TEST(BinaryWithSplit_Parser, Multiple_Instructions) {
	std::string instructions = "imul eax, ebx;\n"
							   "imul ecx, edx;\n"
							   "add eax, ecx;\n"
							   "ret;\n";

	std::vector<double> expected;
	std::vector<double> inst1 = binary_from_hexadecimal({0x0F, 0xAF, 0xC3});
	std::vector<double> inst2 = binary_from_hexadecimal({0x0F, 0xAF, 0xCA});
	std::vector<double> inst3 = binary_from_hexadecimal({0x01, 0xC8});
	std::vector<double> inst4 = binary_from_hexadecimal({0xC3});
	expected.insert(expected.end(), inst1.begin(), inst1.end());
	expected.push_back(0.5);
	expected.insert(expected.end(), inst2.begin(), inst2.end());
	expected.push_back(0.5);
	expected.insert(expected.end(), inst3.begin(), inst3.end());
	expected.push_back(0.5);
	expected.insert(expected.end(), inst4.begin(), inst4.end());
	expected.push_back(0.5);
	std::vector<double> result = BinaryWithSplitParser().parse_in(instructions);
	EXPECT_EQ(result, expected);
}

TEST(BinaryWithSplit_Parser, Memory) {
	std::string instruction = "mov eax, [ebx * 8 + ecx]";
	std::vector<double> expected = binary_from_hexadecimal({0x67, 0x8B, 0x04, 0xD9});
	expected.push_back(0.5);
	std::vector<double> result = BinaryWithSplitParser().parse_in(instruction);
	EXPECT_EQ(result, expected);
}

TEST(BinaryWithSplit_Parser, Loops) {
	std::string instructions = "mov rax, 0;\n"
							   "mov r8, rbx;\n"
							   "mov r9, 0;\n"
							   "loop:;\n"
							   "cmp r8, r9;\n"
							   "je end;\n"
							   "mov r10, [rdi + r9*8];\n"
							   "mov r11, [rsi + r9*8];\n"
							   "imul r10, r11;\n"
							   "add rax, r10;\n"
							   "inc r9;\n"
							   "jmp loop;\n"
							   "end:;\n"
							   "ret;\n";
}

TEST(BinaryWithSplit_Parser, SIMD_Instruction) {
	std::string instruction = "movdqa xmm0, xmm1";
	std::vector<double> expected = binary_from_hexadecimal({0x66, 0x0F, 0x6F, 0xC1});
	expected.push_back(0.5);
	std::vector<double> result = BinaryWithSplitParser().parse_in(instruction);
	EXPECT_EQ(result, expected);

	instruction = "movdqa xmm0, [eax];";
	expected = binary_from_hexadecimal({0x67, 0x66, 0x0F, 0x6F, 0x00});
	expected.push_back(0.5);
	result = BinaryWithSplitParser().parse_in(instruction);
	EXPECT_EQ(result, expected);
}