#ifndef BINARY_WITH_SPLIT_PARSER_HPP
#define BINARY_WITH_SPLIT_PARSER_HPP

#include "iparser.hpp"
#include <string>
#include <vector>

class BinaryWithSplitParser : public IParser {
  public:
	BinaryWithSplitParser() = default;
	~BinaryWithSplitParser() override = default;
	std::vector<double> parse_from_string(std::string& input) override;
};

#endif // BINARY_WITH_SPLIT_PARSER_HPP