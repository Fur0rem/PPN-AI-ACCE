/**
 * @file include/parsing/binary_parser.hpp
 * @brief The header for a parser to convert binary in hexadecimal form into a neural network input by splitting each instruction with a 0.5
 */

#ifndef BINARY_PARSER_HPP
#define BINARY_PARSER_HPP

#include "iparser.hpp"
#include <string>
#include <vector>

/**
 * @brief A parser to convert assembly into a neural network input by splitting each instruction with a 0.5
 */
class BinaryParser : public IParser {
  public:
	/**
	 * @brief Construct a new BinaryParser object
	 */
	BinaryParser() = default;

	/**
	 * @brief Destroy the BinaryParser object
	 */
	~BinaryParser() override = default;

	/**
	 * @brief Parses a binary representation of assembly code into a vector of 0's and 1's according to its binary representation,
	 * @param input The binary code to parse
	 * @return A vector of doubles representing the input of the neural network
	 */
	std::vector<double> parse_in(std::string& input) override;

	/**
	 * @brief Parse the time taken for the code to run using a header in the first line Cycles: (number)
	 * @param input The string to parse
	 * @return A vector of doubles representing the output of the neural network
	 */
	std::vector<double> parse_out(std::string& input) override;
};

#endif // BINARY_PARSER_HPP