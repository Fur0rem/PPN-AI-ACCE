/**
 * @file tests/iparser.cpp
 * @brief Used to test that it correctly fills in the neural network input.
 */

#include "parsing/iparser.hpp"
#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include <vector>

/**
 * @brief Test to check if IParser can correctly fill in the neural network input
 */
TEST(IParser, Into_Neural_Network_Input) {
	constexpr int input_size = 50;
	std::vector<size_t> topology = {input_size, 1};
	std::vector<double> parsed = {0, 1, 0, 1, 1, 0, 1, 0};
	std::vector<double> expected(input_size, NO_MORE_VALUE);
	expected[0] = static_cast<double>(parsed.size()) / static_cast<double>(input_size);
	std::copy(parsed.begin(), parsed.end(), expected.begin() + 1);
	std::vector<double> result = IParser::into_neural_network_input(parsed, topology);
	EXPECT_EQ(result, expected);
}

/**
 * @brief Test to check if IParser can correctly fill in the neural network input with an empty vector
 */
TEST(IParser, Into_Neural_Network_Input_Zero) {
	constexpr int input_size = 50;
	std::vector<size_t> topology = {input_size, 1};
	std::vector<double> parsed = {};
	std::vector<double> expected(input_size, NO_MORE_VALUE);
	expected[0] = 0.0;
	std::vector<double> result = IParser::into_neural_network_input(parsed, topology);
	EXPECT_EQ(result, expected);
}

/**
 * @brief Test to check if IParser can correctly fill in the neural network input with the maximum number of tokens
 */
TEST(IParser, Into_Neural_Network_Input_Max_Tokens) {
	constexpr int input_size = 50;
	std::vector<size_t> topology = {input_size, 1};
	std::vector<double> parsed(input_size - 1, 0);
	std::vector<double> expected(input_size, NO_MORE_VALUE);
	expected[0] = static_cast<double>(input_size - 1) / static_cast<double>(input_size);
	std::fill(expected.begin() + 1, expected.end(), 0);
	std::vector<double> result = IParser::into_neural_network_input(parsed, topology);
	EXPECT_EQ(result, expected);
}