/**
 * @file include/parsing/asm_parser.hpp
 * @brief The header for a parser to convert assembly into a neural network input by just taking the binary values
 */

#ifndef ASM_PARSER_HPP
#define ASM_PARSER_HPP

#include "iparser.hpp"
#include <string>
#include <vector>

/**
 * @brief A parser to convert assembly into a neural network input by just taking the binary values
 */
class AsmParser : public IParser {
  public:
	/**
	 * @brief Construct a new AsmParser object
	 */
	AsmParser() = default;

	/**
	 * @brief Destroy the AsmParser object
	 */
	~AsmParser() override = default;

	/**
	 * @brief Parse the input of the neural network into a vector of 0's and 1's according its binary representation
	 * @param input The string to parse
	 * @return A vector of floats representing the input of the neural network
	 */
	std::vector<float> parse_in(std::string& input) override;

	/**
	 * @brief Parse the time taken for the code to run using a header in the first line Cycles: (number)
	 * @param input The string to parse
	 * @return A vector of floats representing the output of the neural network
	 */
	std::vector<float> parse_out(std::string& input) override;
};

#endif // ASM_PARSER_HPP