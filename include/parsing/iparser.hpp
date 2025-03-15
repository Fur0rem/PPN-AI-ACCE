/**
 * @file include/parsing/iparser.hpp
 * @brief The header for an interface class for parsing and transforming assembly instructions into an input for a neural network.
 */

#ifndef IPARSER_HPP
#define IPARSER_HPP

#include <string>
#include <tuple>
#include <vector>

/**
 * @brief Interface for parsing and transforming assembly instructions into an input for a neural network
 */
class IParser {
  public:
	/**
	 * @brief Parse a string into a combination of input and output for the neural network.
	 * @param input The string to parse
	 * @return A tuple containing a vector of inputs and a vector of outputs
	 */
	std::tuple<std::vector<float>, std::vector<float>> parse_from_string(std::string& input);

	/**
	 * @brief Parse a file into a combination of input and output for the neural network
	 * @param filename The name of the file to parse
	 * @return A tuple containing a vector of inputs and a vector of outputs
	 */
	std::tuple<std::vector<float>, std::vector<float>> parse_from_file(std::string& filename);

	/**
	 * @brief destructor for the IParser class
	 */
	virtual ~IParser() = default;

	/**
	 * @brief Parse the output of the neural network into a vector of floats
	 * @param input The string to parse
	 * @return A vector of floats representing the output of the neural network
	 */
	virtual std::vector<float> parse_out(std::string& input) = 0;

	/**
	 * @brief Parse the input of the neural network into a vector of floats
	 * @param input The string to parse
	 * @return A vector of floats representing the input of the neural network
	 */
	virtual std::vector<float> parse_in(std::string& input) = 0;

  protected:
	IParser() = default;
};

#endif // IPARSER_HPP