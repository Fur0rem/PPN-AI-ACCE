/**
 * @file include/dataset/dataset.hpp
 * @brief Class representing a dataset (inputs and outputs)
 */

#ifndef DATASET_HPP
#define DATASET_HPP

#include <eigen3/Eigen/Dense>
#include <string>
#include <vector>

#include "parsing/iparser.hpp"

/**
 * @brief Class representing a dataset (inputs and outputs)
 */
class Dataset {
  private:
	IParser* m_parser;		///< Parser used to parse the data
	std::string m_dir_path; ///< Path to the directory containing the data

	std::vector<Eigen::VectorXf> m_inputs;	///< Vector containing the inputs
	std::vector<Eigen::VectorXf> m_outputs; ///< Vector containing the outputs

	size_t m_dataset_size; ///< Size of the dataset

  public:
	/**
	 * @brief Construct a new Dataset object
	 * @param parser The parser used to parse the data
	 * @param dir_path The path to the directory containing the data
	 * @param topology The topology of the neural network
	 */
	Dataset(IParser* parser, std::string dir_path, std::vector<size_t>& topology);

	/**
	 * @brief Construct a new Dataset object from already parsed data
	 * @param target_inputs The inputs of the dataset
	 * @param target_outputs The outputs of the dataset
	 * @param topology The topology of the neural network
	 */
	Dataset(std::vector<std::vector<float>> target_inputs, std::vector<std::vector<float>> target_outputs, std::vector<size_t>& topology);

	/**
	 * @brief Get the data from the dataset
	 * @param proportion The proportion of the dataset used as training data
	 * @return A vector of pairs containing the input and output data
	 */
	std::vector<std::pair<Eigen::VectorXf, Eigen::VectorXf>> get_data(double proportion);
};

#endif // DATASET_HPP