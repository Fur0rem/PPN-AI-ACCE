#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <gtest/gtest.h>

#include "parsing/xor_parser.hpp"

TEST(XORParser, XORParseIn) {
	XORParser parser{};
	std::string input = "0 0 0\n";
	std::vector<double> expected = {0.0, 0.0};
	std::vector<double> result = parser.parse_in(input);
	EXPECT_EQ(result, expected);

	input = "0 1 1\n";
	expected = {0.0, 1.0};
	result = parser.parse_in(input);
	EXPECT_EQ(result, expected);

	input = "1 0 1\n";
	expected = {1.0, 0.0};
	result = parser.parse_in(input);
	EXPECT_EQ(result, expected);

	input = "1 1 0\n";
	expected = {1.0, 1.0};
	result = parser.parse_in(input);
	EXPECT_EQ(result, expected);
}

TEST(XORParser, XORParseOut) {
	XORParser parser{};
	std::string input = "0 0 0\n";
	std::vector<double> expected = {0.0};
	std::vector<double> result = parser.parse_out(input);
	EXPECT_EQ(result, expected);

	input = "0 1 1\n";
	expected = {1.0};
	result = parser.parse_out(input);
	EXPECT_EQ(result, expected);

	input = "1 0 1\n";
	expected = {1.0};
	result = parser.parse_out(input);
	EXPECT_EQ(result, expected);

	input = "1 1 0\n";
	expected = {0.0};
	result = parser.parse_out(input);
	EXPECT_EQ(result, expected);
}
