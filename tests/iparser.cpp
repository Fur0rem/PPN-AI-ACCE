/**
 * @file tests/iparser.cpp
 * Used to test that it correctly fills in the neural network input.
 */

#include "parsing/iparser.hpp"
#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include <vector>

TEST(IParser, Into_Neural_Network_Input) {
	std::vector<double> parsed = {0, 1, 0, 1, 1, 0, 1, 0};
	std::array<double, MAX_TOKENS_NN> expected = {(7.0 + 1.0) / static_cast<double>(MAX_TOKENS_NN), 0, 1, 0, 1, 1, 0, 1, 0};
	for (size_t i = 9; i < MAX_TOKENS_NN; i++) {
		expected[i] = NO_MORE_VALUE;
	}
	std::array<double, MAX_TOKENS_NN> result = IParser::into_neural_network_input(parsed);
	EXPECT_EQ(result, expected);
}

TEST(IParser, Into_Neural_Network_Input_Zero) {
	std::vector<double> parsed = {};
	std::array<double, MAX_TOKENS_NN> expected;
	std::fill(expected.begin(), expected.end(), NO_MORE_VALUE);
	expected[0] = 0.0;
	std::array<double, MAX_TOKENS_NN> result = IParser::into_neural_network_input(parsed);
	EXPECT_EQ(result, expected);
}

TEST(IParser, Into_Neural_Network_Input_Max_Tokens) {
	std::vector<double> parsed(MAX_TOKENS_NN - 1, 0);
	std::array<double, MAX_TOKENS_NN> expected{};
	expected[0] = static_cast<double>(MAX_TOKENS_NN - 1) / static_cast<double>(MAX_TOKENS_NN);
	std::fill(expected.begin() + 1, expected.end(), 0);
	std::array<double, MAX_TOKENS_NN> result = IParser::into_neural_network_input(parsed);
	EXPECT_EQ(result, expected);
}