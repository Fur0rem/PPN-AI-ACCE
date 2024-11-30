/**
 * @file src/parsing/iparser.hpp
 * An interface class for parsing assembly instructions into an input for a neural network.
 */

#ifndef IPARSER_HPP
#define IPARSER_HPP

#include <fstream>
#include <string>
#include <vector>

class IParser {
  public:
	virtual std::vector<double> parse_from_string(std::string& input) = 0;
	std::vector<double> parse_from_file(std::string& filename) {
		std::ifstream file(filename);
		std::string str;
		std::string input;
		while (std::getline(file, str)) {
			input += str;
		}
		return parse_from_string(input);
	}

	virtual ~IParser() = default;

  protected:
	IParser() = default;
};

#endif // IPARSER_HPP