/**
 * @file tests/rawbinary_parser.cpp
 * Used to test the assembly parser that converts it to raw binary directly.
 * All tests expected results are checked by ndisasm using -b 64 flag.
 */

#include "parsing/rawbinary_parser.hpp"
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
 * @brief Convert a binary vector to a hexadecimal vector and prints it
 * @param binary The binary vector
 */
void binary_to_hexadecimal(std::vector<double>& binary) {
	for (size_t i = 0; i < binary.size(); i += 8) {
		uint8_t byte = 0;
		for (int j = 0; j < 8; j++) {
			byte |= static_cast<uint8_t>(binary[i + j]) << (7 - j);
		}
		printf("%02X ", byte);
	}
	printf("\n");
}

/**
 * @brief Test to check if the parser can convert a single instruction to binary
 */
TEST(RawBinary_Parser, Single_Instruction) {
	std::string instruction = "add eax, ebx";
	std::vector<double> expected = binary_from_hexadecimal({0x01, 0xD8});
	std::vector<double> result = RawBinaryParser().parse_in(instruction);
	EXPECT_EQ(result, expected);

	instruction = "add eax, 0x1234";
	expected = binary_from_hexadecimal({0x05, 0x34, 0x12, 0x00, 0x00});
	result = RawBinaryParser().parse_in(instruction);
	EXPECT_EQ(result, expected);
}

/**
 * @brief Test to check if the parser can convert multiple instructions to binary
 */
TEST(RawBinary_Parser, Multiple_Instructions) {
	std::string instructions = ";; Cycles : 1002\n"
							   "imul eax, ebx;\n"
							   "imul ecx, edx;\n"
							   "add eax, ecx;\n"
							   "ret;\n";

	std::vector<double> expected = binary_from_hexadecimal({0x0F, 0xAF, 0xC3, 0x0F, 0xAF, 0xCA, 0x01, 0xC8, 0xC3});
	std::vector<double> result = RawBinaryParser().parse_in(instructions);
	EXPECT_EQ(result, expected);

	expected = {1002};
	result = RawBinaryParser().parse_out(instructions);
	EXPECT_EQ(result, expected);
}

/**
 * @brief Test to check if the parser can parse the header with the number of cycles
 */
TEST(RawBinary_Parser_Out, Multiple_Instructions) {
	std::string instructions = ";; Cycles : 1002\n"
							   "imul eax, ebx;\n"
							   "imul ecx, edx;\n"
							   "add eax, ecx;\n"
							   "ret;\n";

	std::vector<double> expected;
	expected.push_back(1002);
	std::vector<double> result = RawBinaryParser().parse_out(instructions);
	EXPECT_EQ(result, expected);

	expected = {1002};
	result = RawBinaryParser().parse_out(instructions);
	EXPECT_EQ(result, expected);
}

/**
 * @brief Test to check if the parser can parse instructions with memory access
 */
TEST(RawBinary_Parser, Memory) {
	std::string instruction = "mov eax, [ebx * 8 + ecx]";
	std::vector<double> expected = binary_from_hexadecimal({0x67, 0x8B, 0x04, 0xD9});
	std::vector<double> result = RawBinaryParser().parse_in(instruction);
	EXPECT_EQ(result, expected);
}

/**
 * @brief Test to check if the parser can parse loop instructions
 */
TEST(RawBinary_Parser, Loops) {
	std::string instructions = ";; Cycles : 2488957\n"
							   "mov rax, 0;\n"
							   "mov rbx, rax;\n"
							   "mov rcx, 0;\n"
							   "loop:;\n"
							   "cmp rbx, rcx;\n"
							   "je end;\n"
							   "mov rdx, [rdi + rcx*8];\n"
							   "mov rsi, [rsi + rcx*8];\n"
							   "imul rdx, rsi;\n"
							   "add rax, rdx;\n"
							   "inc rcx;\n"
							   "jmp loop;\n"
							   "end:;\n"
							   "ret;\n";

	std::vector<double> expected = binary_from_hexadecimal({
		0xB8, 0x00, 0x00, 0x00, 0x00, 0x48, 0x89, 0xC3, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x48, 0x39, 0xCB, 0x74, 0x14, 0x48, 0x8B,
		0x14, 0xCF, 0x48, 0x8B, 0x34, 0xCE, 0x48, 0x0F, 0xAF, 0xD6, 0x04, 0x80, 0x1D, 0x04, 0x8F, 0xFC, 0x1E, 0xBE, 0x7C, 0x03,
	});
	std::vector<double> result = RawBinaryParser().parse_in(instructions);
	EXPECT_EQ(result, expected);
}

/**
 * @brief Test to check if the parser can parse SIMD instructions
 */
TEST(RawBinary_Parser, SIMD_Instruction) {
	std::string instruction = "movdqa xmm0, xmm1";
	std::vector<double> expected = binary_from_hexadecimal({0x66, 0x0F, 0x6F, 0xC1});
	std::vector<double> result = RawBinaryParser().parse_in(instruction);
	EXPECT_EQ(result, expected);

	instruction = "movdqa xmm0, [eax];";
	expected = binary_from_hexadecimal({0x67, 0x66, 0x0F, 0x6F, 0x00});
	result = RawBinaryParser().parse_in(instruction);
	EXPECT_EQ(result, expected);
}