#ifndef XOR_PARSER_HPP
#define XOR_PARSER_HPP

#include <string>
#include <tuple>
#include <vector>

#include "iparser.hpp"

class XORParser : public IParser {
	std::vector<double> m_parser_output;

  public:
	XORParser() = default;
	~XORParser() override = default;

	std::vector<double> parse_in(std::string& input) override;
	std::vector<double> parse_out(std::string& input) override;
};

#endif /* XOR_PARSER_HPP */