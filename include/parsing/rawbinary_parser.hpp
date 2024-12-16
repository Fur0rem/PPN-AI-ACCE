#ifndef RAWBINARY_PARSER_HPP
#define RAWBINARY_PARSER_HPP

#include "iparser.hpp"
#include <string>
#include <vector>

class RawBinaryParser : public IParser {
  public:
	RawBinaryParser() = default;
	~RawBinaryParser() override = default;
	std::vector<double> parse_from_string(std::string& input) override;
};

#endif // RAWBINARY_PARSER_HPP