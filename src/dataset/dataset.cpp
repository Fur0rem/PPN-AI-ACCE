/**
 * @file src/dataset/dataset.cpp
 * @brief Implementation of the Dataset class
 */

#include <algorithm>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/src/Core/Matrix.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "dataset/dataset.hpp"
#include "parsing/iencoder.hpp"
#include "parsing/iparser.hpp"

auto rng = std::default_random_engine{}; ///< Random number generator

Dataset::Dataset(IParser* parser, std::string dir_path, std::vector<size_t>& topology, std::unique_ptr<IEncoder> input_encoder,
				 std::unique_ptr<IEncoder> output_encoder) {
	m_dir_path = std::move(dir_path);
	m_parser = parser;
	m_input_encoder = std::move(input_encoder);
	m_output_encoder = std::move(output_encoder);

	auto dir = std::filesystem::directory_iterator(m_dir_path);
	if (dir == std::filesystem::directory_iterator()) {
		throw std::runtime_error("The directory does not exist");
		exit(1);
	}
	// Collect all the files in the directory
	std::vector<std::filesystem::directory_entry> files;
	for (const auto& entry : dir) {
		files.push_back(entry);
	}

	// Sort the files
	std::sort(files.begin(), files.end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
		return a.path().string() < b.path().string();
	});

	for (const auto& entry : files) {
		std::cout << "Reading : " << entry.path() << std::endl;

		// Get only the last part of the path
		std::string name = entry.path().filename().string();
		m_names.push_back(name);

		// Read the file
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

		// Parse the input
		std::vector<float> input = m_parser->parse_in(content);
		auto input_encoded = m_input_encoder->encode(input);
		Eigen::VectorXf vec_in(input_encoded.size());
		for (size_t i = 0; i < input_encoded.size(); i++) {
			vec_in(i) = static_cast<float>(input_encoded[i]);
		}
		m_inputs.push_back(vec_in);

		// Parse the output
		std::vector<float> output = m_parser->parse_out(content);
		auto output_encoded = m_output_encoder->encode(output);
		Eigen::VectorXf vec_out(output_encoded.size());
		for (size_t i = 0; i < output_encoded.size(); i++) {
			vec_out(i) = static_cast<float>(output_encoded[i]);
		}
		m_outputs.push_back(vec_out);
	}

	m_dataset_size = m_inputs.size();
}

Dataset::Dataset(std::vector<std::vector<float>> inputs, std::vector<std::vector<float>> target_outputs, std::vector<size_t>& topology,
				 std::unique_ptr<IEncoder> input_encoder, std::unique_ptr<IEncoder> output_encoder) {
	this->m_input_encoder = std::move(input_encoder);
	this->m_output_encoder = std::move(output_encoder);
	size_t input_size = topology[0];
	size_t output_size = topology[topology.size() - 1];
	for (size_t i = 0; i < inputs.size(); i++) {
		if (inputs[i].size() != input_size) {
			throw std::runtime_error("Input size does not match the topology");
		}

		auto input = m_input_encoder->encode(inputs[i]);
		Eigen::VectorXf vec_in(input.size());
		for (size_t j = 0; j < input.size(); j++) {
			vec_in(j) = static_cast<float>(input[j]);
		}
		m_inputs.push_back(vec_in);

		if (target_outputs[i].size() != output_size) {
			throw std::runtime_error("Output size does not match the topology");
		}

		auto output = m_output_encoder->encode(target_outputs[i]);
		Eigen::VectorXf vec_out(output.size());
		for (size_t j = 0; j < output.size(); j++) {
			vec_out(j) = static_cast<float>(output[j]);
		}
		m_outputs.push_back(vec_out);

		std::string name = "";
		for (size_t j = 0; j < input_size; j++) {
			name += std::to_string(inputs[i][j]) + " ";
		}
		name.pop_back();
		m_names.push_back(name);
	}

	m_dataset_size = m_inputs.size();
}

std::vector<std::tuple<std::string, Eigen::VectorXf, Eigen::VectorXf>> Dataset::get_data(double proportion) {
	std::vector<int> indeces(m_dataset_size);
	std::iota(indeces.begin(), indeces.end(), 0);

	std::shuffle(indeces.begin(), indeces.end(), rng);
	size_t last_index = (size_t)((double)m_dataset_size * proportion);

	std::vector<std::tuple<std::string, Eigen::VectorXf, Eigen::VectorXf>> result;

	for (size_t i = 0; i < last_index; ++i) {
		result.push_back({m_names[indeces[i]], m_inputs[indeces[i]], m_outputs[indeces[i]]});
	}
	return result;
}

std::vector<std::tuple<std::string, Eigen::VectorXf, Eigen::VectorXf>> Dataset::get_all_data() {
	std::vector<std::tuple<std::string, Eigen::VectorXf, Eigen::VectorXf>> result;

	for (size_t i = 0; i < m_dataset_size; ++i) {
		result.push_back({m_names[i], m_inputs[i], m_outputs[i]});
	}
	return result;
}

IEncoder* Dataset::get_output_encoder() const {
	return m_output_encoder.get();
}

IEncoder* Dataset::get_input_encoder() const {
	return m_input_encoder.get();
}