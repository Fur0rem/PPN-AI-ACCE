/**
 * @file tests/cycles_encoders.cpp
 * @brief Used to test the cycles encoders.
 */

#include "parsing/cycles_encoders.hpp"
#include <gtest/gtest.h>

/**
 * @brief Test to check if the normaliser encoder can encode and decode a value
 */
TEST(cycles_encoders, CyclesNormaliserEncoder) {
	CyclesNormaliserEncoder encoder;
	std::vector<float> inputs = {1000.0F, 18929.5F, 1.0F, 89170849.7F, 5460.0F, 4e9F};
	for (float input : inputs) {
		auto input_vector = std::vector<float>{input};
		std::vector<float> encoded = encoder.encode(input_vector);
		EXPECT_GE(encoded[0], 0.0);
		EXPECT_LE(encoded[0], 1.0);
		std::vector<float> decoded = encoder.decode(encoded);
		EXPECT_LE(std::abs(input - decoded[0]) / ((100.0F) * ((decoded[0] + input) / 2)), 1.0F); // Within 1%
	}
}

/**
 * @brief Test to check if the splitter encoder can encode and decode a value
 */
TEST(cycles_encoders, CyclesSplitterEncoder) {
	CyclesSplitterEncoder encoder;
	std::vector<float> inputs = {1000.0F, 18929.5F, 1.0F, 89170849.7F, 5460.0F, 4e9F};
	for (float input : inputs) {
		auto input_vector = std::vector<float>{input};
		std::vector<float> encoded = encoder.encode(input_vector);
		EXPECT_GE(encoded[0], 0.0);
		EXPECT_LE(encoded[0], 1.0);
		std::vector<float> decoded = encoder.decode(encoded);
		EXPECT_LE(std::abs(input - decoded[0]) / ((100.0F) * ((decoded[0] + input) / 2)), 1.0F); // Within 1%
	}
}

/**
 * @brief Test to check if the log encoder can encode and decode a value
 */
TEST(cycles_encoders, CyclesLogEncoder) {
	CyclesLogEncoder encoder(10, 1000);
	std::vector<float> inputs = {1000.0F, 18929.5F, 1.0F, 89170849.7F, 5460.0F, 4e9F};
	for (float input : inputs) {
		auto input_vector = std::vector<float>{input};
		std::vector<float> encoded = encoder.encode(input_vector);
		EXPECT_GE(encoded[0], 0.0);
		EXPECT_LE(encoded[0], 1.0);
		std::vector<float> decoded = encoder.decode(encoded);
		EXPECT_LE(std::abs(input - decoded[0]) / ((100.0F) * ((decoded[0] + input) / 2)), 1.0F); // Within 1%
	}
}

/**
 * @brief Test to check if the sqrt encoder can encode and decode a value
 */
TEST(cycles_encoders, CyclesSqrtEncoder) {
	CyclesSqrtEncoder encoder(1000);
	std::vector<float> inputs = {1000.0F, 18929.5F, 1.0F, 89170849.7F, 5460.0F, 4e9F};
	for (float input : inputs) {
		auto input_vector = std::vector<float>{input};
		std::vector<float> encoded = encoder.encode(input_vector);
		EXPECT_GE(encoded[0], 0.0);
		EXPECT_LE(encoded[0], 1.0);
		std::vector<float> decoded = encoder.decode(encoded);
		EXPECT_LE(std::abs(input - decoded[0]) / ((100.0F) * ((decoded[0] + input) / 2)), 1.0F); // Within 1%
	}
}

/**
 * @brief Test to check if the bounded normaliser encoder can encode and decode a value
 */
TEST(cycles_encoders, CyclesBoundedNormaliserEncoder) {
	CyclesBoundedNormaliserEncoder encoder;
	std::vector<float> inputs = {1000.0F, 18929.5F, 1.0F, 89170849.7F, 5460.0F, 4e9F};
	for (float input : inputs) {
		auto input_vector = std::vector<float>{input};
		std::vector<float> encoded = encoder.encode(input_vector);
		EXPECT_GE(encoded[0], 0.0);
		EXPECT_LE(encoded[0], 1.0);
		std::vector<float> decoded = encoder.decode(encoded);
		EXPECT_LE(std::abs(input - decoded[0]) / ((100.0F) * ((decoded[0] + input) / 2)), 1.0F); // Within 1%
	}
}
