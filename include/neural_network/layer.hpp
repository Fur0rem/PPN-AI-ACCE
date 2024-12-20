#ifndef LAYER_HPP
#define LAYER_HPP

#include <eigen3/Eigen/Dense>

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
#endif // LAYER_HPP