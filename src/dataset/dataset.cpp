/**
 * @file src/dataset/dataset.cpp
 * @brief Implementation of the Dataset class
 */

#include <algorithm>
#include <eigen3/Eigen/Dense>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "dataset/dataset.hpp"
#include "neural_network/neural_network.hpp"
#include "parsing/iparser.hpp"

auto rng = std::default_random_engine{}; ///< Random number generator

Dataset::Dataset(IParser* parser, std::string dir_path) {
	m_dir_path = dir_path;
	m_parser = parser;

	for (const auto& entry : std::filesystem::directory_iterator(m_dir_path)) {
		std::cout << entry.path() << std::endl;

		// read the file
		std::string file_path = entry.path();
		std::string content;
		std::ifstream file(file_path);
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				content += line + "\n";
			}
			file.close();
		}

		std::vector<double> input_vec = m_parser->parse_in(content);
		auto input = m_parser->into_neural_network_input(input_vec);
		int input_size = input.size();
		Eigen::VectorXf vec_in(input_size);
		for (int i = 0; i < input_size; i++) {
			vec_in(i) = static_cast<float>(input[i]);
		}
		m_inputs.push_back(vec_in);

		std::vector<double> output = m_parser->parse_out(content);
		int output_size = output.size();
		Eigen::VectorXf vec_out(output_size);
		for (int i = 0; i < output_size; i++) {
			vec_out(i) = static_cast<float>(output[i]);
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
