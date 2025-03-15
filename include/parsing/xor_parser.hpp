/**
 * @file include/parsing/xor_parser.hpp
 * @brief The header for a parser of xor data for a neural network
 */

#ifndef XOR_PARSER_HPP
#define XOR_PARSER_HPP

#include <string>
#include <vector>

#include "iparser.hpp"

/**
 * @brief A parser for the XOR data for a neural network
 */
class XORParser : public IParser {
  public:
	/**
	 * @brief Construct a new XORParser object
	 */
	XORParser() = default;

	/**
	 * @brief Destroy the XORParser object
	 */
	~XORParser() override = default;

	/**
	 * @brief Parse the input of the neural network into a vector of floats.
	 * The input is expected to be in the format "<input1> <input2> <output>"
	 * @param input The string to parse
	 * @return A vector of floats representing the input of the neural network
	 */
	std::vector<float> parse_in(std::string& input) override;

	/**
	 * @brief Parse the output of the neural network into a vector of floats.
	 * The output is expected to be in the format "<input1> <input2> <output>"
	 * @param input The string to parse
	 * @return A vector of floats representing the output of the neural network
	 */
	std::vector<float> parse_out(std::string& input) override;
};

#endif // XOR_PARSER_HPP