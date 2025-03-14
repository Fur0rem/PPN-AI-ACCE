/**
 * @file include/parsing/iparser.hpp
 * @brief The header for an interface class for parsing and transforming assembly instructions into an input for a neural network.
 */

#ifndef IPARSER_HPP
#define IPARSER_HPP

#include <array>
#include <string>
#include <tuple>
#include <vector>

constexpr size_t MAX_TOKENS_NN = 11000; ///< Maximum number of tokens for the neural network
constexpr float MAX_CYCLES = 4e5;		///< Maximum number of cycles as an output for the neural network
constexpr double NO_MORE_VALUE = -1.0;	///< Filler value for the neural network input

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
	std::tuple<std::vector<double>, std::vector<double>> parse_from_string(std::string& input);

	/**
	 * @brief Parse a file into a combination of input and output for the neural network
	 * @param filename The name of the file to parse
	 * @return A tuple containing a vector of inputs and a vector of outputs
	 */
	std::tuple<std::vector<double>, std::vector<double>> parse_from_file(std::string& filename);

	/**
	 * @brief Convert a vector of parsed values into an input for the neural network
	 * @param parsed The vector of parsed values
	 * @param topology The topology of the neural network
	 * @return An array of doubles representing the input for the neural network
	 */
	static std::vector<double> into_neural_network_input(std::vector<double>& parsed, std::vector<size_t>& topology);

	/**
	 * @brief destructor for the IParser class
	 */
	virtual ~IParser() = default;

	/**
	 * @brief Parse the output of the neural network into a vector of doubles
	 * @param input The string to parse
	 * @return A vector of doubles representing the output of the neural network
	 */
	virtual std::vector<double> parse_out(std::string& input) = 0;

	/**
	 * @brief Parse the input of the neural network into a vector of doubles
	 * @param input The string to parse
	 * @return A vector of doubles representing the input of the neural network
	 */
	virtual std::vector<double> parse_in(std::string& input) = 0;

  protected:
	IParser() = default;
};

#endif // IPARSER_HPP