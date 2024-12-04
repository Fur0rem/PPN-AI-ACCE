/**
 * @file include/parsing/iparser.hpp
 * The header for an interface class for parsing assembly instructions into an input for a neural network.
 */

#ifndef IPARSER_HPP
#define IPARSER_HPP

#include <array>
#include <string>
#include <vector>

constexpr size_t MAX_TOKENS_NN = 2000; // TODO: Replace it when we fixed a size of the input for the neural network
constexpr double NO_MORE_VALUE = 0.0;

class IParser {
  public:
	virtual std::vector<double> parse_from_string(std::string& input) = 0;
	std::vector<double> parse_from_file(std::string& filename);
	static std::array<double, MAX_TOKENS_NN> into_neural_network_input(std::vector<double>& parsed);

	virtual ~IParser() = default;

  protected:
	IParser() = default;
};

#endif // IPARSER_HPP