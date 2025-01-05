/**
 * @file include/parsing/rawbinary_parser.hpp
 * @brief The header for a parser to convert assembly into a neural network input by just taking the binary values
 */

#ifndef RAWBINARY_PARSER_HPP
#define RAWBINARY_PARSER_HPP

#include "iparser.hpp"
#include <string>
#include <vector>

/**
 * @brief A parser to convert assembly into a neural network input by just taking the binary values
 */
class RawBinaryParser : public IParser {
  public:
	/**
	 * @brief Construct a new RawBinaryParser object
	 */
	RawBinaryParser() = default;

	/**
	 * @brief Destroy the RawBinaryParser object
	 */
	~RawBinaryParser() override = default;

	/**
	 * @brief Parse the input of the neural network into a vector of 0's and 1's according its binary representation
	 * @param input The string to parse
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

#endif // RAWBINARY_PARSER_HPP