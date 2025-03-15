/**
 * @file include/dataset/dataset.hpp
 * @brief Class representing a dataset (inputs and outputs)
 */

#ifndef DATASET_HPP
#define DATASET_HPP

#include <eigen3/Eigen/Dense>
#include <memory>
#include <string>
#include <vector>

#include "parsing/iencoder.hpp"
#include "parsing/iparser.hpp"

/**
 * @brief Class representing a dataset (inputs and outputs)
 */
class Dataset {
  private:
	IParser* m_parser;		///< Parser used to parse the data
	std::string m_dir_path; ///< Path to the directory containing the data

	std::vector<std::string>
		m_names; ///< Vector containing the names of the files if the data is parsed from files, a string representing the data otherwise
	std::vector<Eigen::VectorXf> m_inputs;	///< Vector containing the inputs
	std::vector<Eigen::VectorXf> m_outputs; ///< Vector containing the outputs

	std::unique_ptr<IEncoder> m_input_encoder;	///< Encoder used to encode the input data
	std::unique_ptr<IEncoder> m_output_encoder; ///< Encoder used to encode the output data

	size_t m_dataset_size; ///< Size of the dataset

  public:
	/**
	 * @brief Construct a new Dataset object
	 * @param parser The parser used to parse the data
	 * @param dir_path The path to the directory containing the data
	 * @param topology The topology of the neural network
	 * @param input_encoder The encoder used to encode the input data
	 * @param output_encoder The encoder used to encode the output data
	 */
	Dataset(IParser* parser, std::string dir_path, std::vector<size_t>& topology, std::unique_ptr<IEncoder> input_encoder,
			std::unique_ptr<IEncoder> output_encoder);

	/**
	 * @brief Construct a new Dataset object from already parsed data
	 * @param inputs The inputs of the dataset
	 * @param target_outputs The outputs of the dataset
	 * @param topology The topology of the neural network
	 * @param input_encoder The encoder used to encode the input data
	 * @param output_encoder The encoder used to encode the output data
	 */
	Dataset(std::vector<std::vector<float>> inputs, std::vector<std::vector<float>> target_outputs, std::vector<size_t>& topology,
			std::unique_ptr<IEncoder> input_encoder, std::unique_ptr<IEncoder> output_encoder);

	/**
	 * @brief Get the data from the dataset
	 * @param proportion The proportion of the dataset used as training data
	 * @return A vector of pairs containing the input and output data
	 */
	std::vector<std::tuple<std::string, Eigen::VectorXf, Eigen::VectorXf>> get_data(double proportion);

	/**
	 * @brief Get all the data from the dataset
	 * @return A vector of tuples containing the name, input and output data
	 */
	std::vector<std::tuple<std::string, Eigen::VectorXf, Eigen::VectorXf>> get_all_data();

	/**
	 * @brief Get the output encoder
	 * @return The output encoder
	 */
	IEncoder* get_output_encoder() const;

	/**
	 * @brief Get the input encoder
	 * @return The input encoder
	 */
	IEncoder* get_input_encoder() const;
};

#endif // DATASET_HPP