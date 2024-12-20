#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP
#include <eigen3/Eigen/Dense>
#include <string>
#include <vector>

class Layer {
  public:
	Layer(int input_size, int output_size);
	Eigen::MatrixXf feed_forward(Eigen::MatrixXf& input);
	Eigen::MatrixXf back_propagate(const Eigen::MatrixXf& error, float learning_rate, Eigen::MatrixXf& next_values);

  private:
	Eigen::MatrixXf m_weights;
	Eigen::MatrixXf m_biases;
	Eigen::MatrixXf m_values;

	friend class NeuralNetwork;
};

class NeuralNetwork {
  private:
	std::vector<int> m_topology;
	std::vector<Layer> m_layers;
	Eigen::MatrixXf m_last_values;
	float m_learning_rate;

	void feed_forward(const Eigen::VectorXf& input);
	void back_propagate(const Eigen::VectorXf& target_output);

  public:
	NeuralNetwork(std::vector<int>& topology, float learning_rate);

	void train(std::vector<std::vector<float>>& inputs, std::vector<std::vector<float>>& targets, int nb_epochs,
			   std::string&& logging_filename);

	std::vector<float> get_prediction(std::vector<float>& input);

	float get_total_loss(std::vector<std::vector<float>> inputs, std::vector<std::vector<float>> targets);
};

#endif // NEURAL_NETWORK_HPP