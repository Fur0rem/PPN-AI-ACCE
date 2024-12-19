#include <Eigen/Dense>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "dataset/dataset.hpp"
#include "neuralnet/Net.h"
#include "parsing/iparser.hpp"

auto rng = std::default_random_engine{};

Dataset::Dataset(IParser* parser, std::string dir_path) {
	m_dir_path = dir_path;
	m_parser = parser;

	for (const auto& entry : std::filesystem::directory_iterator(m_dir_path)) {
		std::cout << entry.path() << std::endl;
		std::string file_path = entry.path();
		std::vector<double> input = m_parser->parse_in(file_path);

		int input_size = input.size();
		Eigen::VectorXf vec_in(input_size);
		for (int i = 0; i < input_size; i++) {
			vec_in << (float)(input[i]);
		}
		m_inputs.push_back(vec_in);

		std::vector<double> output = m_parser->parse_out(file_path);

		Eigen::VectorXf vec_out(input_size);
		for (int i = 0; i < input_size; i++) {
			vec_out << (float)(input[i]);
		}
		m_outputs.push_back(vec_out);
	}

	m_dataset_size = m_inputs.size();
}

std::vector<std::pair<Eigen::VectorXf, Eigen::VectorXf>> Dataset::get_data(double proportion) {
	std::vector<int> indeces(m_dataset_size);
	std::iota(indeces.begin(), indeces.end(), 0);

	std::shuffle(indeces.begin(), indeces.end(), rng);
	size_t last_index = (size_t)((double)m_dataset_size * proportion);

	std::vector<std::pair<Eigen::VectorXf, Eigen::VectorXf>> result;

	for (int i = 0; i < last_index; ++i) {
		result.push_back({m_inputs[indeces[i]], m_outputs[indeces[i]]});
	}
	return result;
}
