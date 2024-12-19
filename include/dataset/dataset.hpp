#ifndef DATA_HPP
#define DATA_HPP

#include <Eigen/Dense>
#include <string>
#include <vector>

#include "neuralnet/Net.h"
#include "parsing/iparser.hpp"

class Dataset {
	IParser* m_parser;
	std::string m_dir_path;

	std::vector<Eigen::VectorXf> m_inputs;
	std::vector<Eigen::VectorXf> m_outputs;

	size_t m_dataset_size;

  public:
	Dataset(IParser* parser, std::string dir_path);

	// vector of pairs (in, out)
	// proportion : percentage of the dataset used as training data
	std::vector<std::pair<Eigen::VectorXf, Eigen::VectorXf>> get_data(double proportion);
};

#endif /* DATA_HPP */