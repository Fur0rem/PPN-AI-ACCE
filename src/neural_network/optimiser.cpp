/**
 * @file src/neural_network/optimiser.cpp
 * @brief Implementation of optimisers during training of neural networks.
 */

 #include "neural_network/optimiser.hpp"
 #include "neural_network/neural_network.hpp"
 #include <eigen3/Eigen/src/Core/Matrix.h>
 
 // Implementation of SGD
 SGD::SGD(float learning_rate) : m_learning_rate(learning_rate) {}
 
 void SGD::update_weights(const Gradients& grads, NeuralNetwork& nn) {
	 for (size_t i = 0; i < nn.m_weights.size(); i++) {
		 for (size_t k = 0; k < nn.m_weights[i].cols(); k++) {
		 	for (size_t j = 0; j < nn.m_weights[i].rows(); j++) {
				 nn.m_weights[i](j, k) -= m_learning_rate * grads.d_w()[i](j, k);
			 }
		 }
		 for (size_t j = 0; j < nn.m_biases[i].cols(); j++) {
			 nn.m_biases[i](0, j) -= m_learning_rate * grads.d_b()[i](0, j);
		 }
	 }
 }
 
 SGD::~SGD() {}
 
 float SGD::get_learning_rate() const {
	 return m_learning_rate;
 }
 
 // Implementation of Momentum
 Momentum::Momentum(NeuralNetwork& nn, float beta, float learning_rate) : m_beta(beta), m_learning_rate(learning_rate) {
	 // Initialise the momentum vectors to 0
	 m_vW.resize(nn.m_weights.size());
	 m_vB.resize(nn.m_biases.size());
	 for (size_t i = 0; i < nn.m_weights.size(); i++) {
		 m_vW[i] = Eigen::MatrixXf::Zero(nn.m_weights[i].rows(), nn.m_weights[i].cols());
	 }
	 for (size_t i = 0; i < nn.m_biases.size(); i++) {
		 m_vB[i] = Eigen::MatrixXf::Zero(nn.m_biases[i].rows(), nn.m_biases[i].cols());
	 }
 }
 
 void Momentum::update_weights(const Gradients& grads, NeuralNetwork& nn) {
	 for (size_t i = 0; i < nn.m_weights.size(); i++) {
		 // Calculate the new momentum with accumulation to the previous momentum
		 m_vW[i] = m_vW[i] * m_beta + grads.d_w()[i] * (1 - m_beta);
		 m_vB[i] = m_vB[i] * m_beta + grads.d_b()[i] * (1 - m_beta);
 
		 // Update the weights and biases with the momentum
		 for (size_t k = 0; k < nn.m_weights[i].cols(); k++) {
		 	for (size_t j = 0; j < nn.m_weights[i].rows(); j++) {
				 nn.m_weights[i](j, k) -= m_learning_rate * m_vW[i](j, k);
			 }
		 }
		 for (size_t j = 0; j < nn.m_biases[i].cols(); j++) {
			 nn.m_biases[i](0, j) -= m_learning_rate * m_vB[i](0, j);
		 }
	 }
 }
 Momentum::~Momentum() {}
 
 float Momentum::get_learning_rate() const {
	 return m_learning_rate;
 }
 
 // Implementation of AMSGrad
 AMSGrad::AMSGrad(NeuralNetwork& nn, float beta1, float beta2, float epsilon, float learning_rate)
	 : m_beta1(beta1), m_beta2(beta2), m_epsilon(epsilon), m_learning_rate(learning_rate) {
 
	 // Create the momentum and velocity vectors for each layer
	 m_mW = std::vector<Eigen::MatrixXf>(nn.m_weights.size());
	 m_mB = std::vector<Eigen::MatrixXf>(nn.m_biases.size());
	 m_vW = std::vector<Eigen::MatrixXf>(nn.m_weights.size());
	 m_vB = std::vector<Eigen::MatrixXf>(nn.m_biases.size());
	 m_vW_hat = std::vector<Eigen::MatrixXf>(nn.m_weights.size());
	 m_vB_hat = std::vector<Eigen::MatrixXf>(nn.m_biases.size());
 
	 // Initialise the momentum and velocity vectors to 0
	 for (size_t i = 0; i < nn.m_weights.size(); i++) {
		 size_t layer_rows = nn.m_weights[i].rows();
		 size_t layer_cols = nn.m_weights[i].cols();
		 m_mW[i] = Eigen::MatrixXf::Zero(layer_rows, layer_cols);
		 m_vW[i] = Eigen::MatrixXf::Zero(layer_rows, layer_cols);
		 m_vW_hat[i] = Eigen::MatrixXf::Zero(layer_rows, layer_cols);
	 }
	 for (size_t i = 0; i < nn.m_biases.size(); i++) {
		 size_t layer_rows = nn.m_biases[i].rows();
		 size_t layer_cols = nn.m_biases[i].cols();
		 m_mB[i] = Eigen::MatrixXf::Zero(layer_rows, layer_cols);
		 m_vB[i] = Eigen::MatrixXf::Zero(layer_rows, layer_cols);
		 m_vB_hat[i] = Eigen::MatrixXf::Zero(layer_rows, layer_cols);
	 }
 }
 
 void AMSGrad::update_weights(const Gradients& grads, NeuralNetwork& nn) {
	 for (size_t i = 0; i < nn.m_weights.size(); i++) {
		 auto gdwi = grads.d_w()[i];
		 auto gdbi = grads.d_b()[i];
 
		 // Update the momentum and velocity vectors
		 m_mW[i] = m_mW[i] * m_beta1 + gdwi * (1 - m_beta1);
		 m_mB[i] = m_mB[i] * m_beta1 + gdbi * (1 - m_beta1);
		 m_vW[i] = m_vW[i] * m_beta2 + gdwi.cwiseProduct(gdwi) * (1 - m_beta2);
		 m_vB[i] = m_vB[i] * m_beta2 + gdbi.cwiseProduct(gdbi) * (1 - m_beta2);
 
		 // Compute the maximum of the past gradients
		 m_vW_hat[i] = m_vW_hat[i].cwiseMax(m_vW[i]);
		 m_vB_hat[i] = m_vB_hat[i].cwiseMax(m_vB[i]);
 
		 // Update weights and biases
		 for (int k = 0; k < nn.m_weights[i].cols(); k++) {
		 	for (int j = 0; j < nn.m_weights[i].rows(); j++) {
				 nn.m_weights[i](j, k) -= m_learning_rate * m_mW[i](j, k) / (std::sqrt(m_vW_hat[i](j, k)) + m_epsilon);
			 }
		 }
		 for (int j = 0; j < nn.m_biases[i].cols(); j++) {
			 nn.m_biases[i](0, j) -= m_learning_rate * m_mB[i](0, j) / (std::sqrt(m_vB_hat[i](0, j)) + m_epsilon);
		 }
	 }
 }
 
 AMSGrad::~AMSGrad() {}
 
 float AMSGrad::get_learning_rate() const {
	 return m_learning_rate;
 }
 
 // Implementation of Adam
 Adam::Adam(NeuralNetwork& nn, float beta1, float beta2, float epsilon, float learning_rate)
	 : m_beta1(beta1), m_beta2(beta2), m_epsilon(epsilon), m_learning_rate(learning_rate) {
 
	 // Create the momentum and velocity vectors for each layer
	 m_mW = std::vector<Eigen::MatrixXf>(nn.m_weights.size());
	 m_mB = std::vector<Eigen::MatrixXf>(nn.m_biases.size());
	 m_vW = std::vector<Eigen::MatrixXf>(nn.m_weights.size());
	 m_vB = std::vector<Eigen::MatrixXf>(nn.m_biases.size());
 
	 // Initialise the momentum and velocity vectors to 0
	 for (size_t i = 0; i < nn.m_weights.size(); i++) {
		 size_t layer_x = nn.m_weights[i].rows();
		 size_t layer_y = nn.m_weights[i].cols();
		 m_mW[i] = Eigen::MatrixXf::Zero(layer_x, layer_y);
		 m_vW[i] = Eigen::MatrixXf::Zero(layer_x, layer_y);
	 }
 
	 for (size_t i = 0; i < nn.m_biases.size(); i++) {
		 size_t layer_x = nn.m_biases[i].rows();
		 size_t layer_y = nn.m_biases[i].cols();
		 m_mB[i] = Eigen::MatrixXf::Zero(layer_x, layer_y);
		 m_vB[i] = Eigen::MatrixXf::Zero(layer_x, layer_y);
	 }
 }
 
 void Adam::update_weights(const Gradients& grads, NeuralNetwork& nn) {
	 for (size_t i = 0; i < nn.m_weights.size(); i++) {
		 // Calculate the new momentum and velocity with accumulation to the previous momentum
		 m_mW[i] = m_mW[i] * m_beta1 + grads.d_w()[i] * (1 - m_beta1);
		 m_mB[i] = m_mB[i] * m_beta1 + grads.d_b()[i] * (1 - m_beta1);
		 m_vW[i] = m_vW[i] * m_beta2 + grads.d_w()[i].cwiseProduct(grads.d_w()[i]) * (1 - m_beta2);
		 m_vB[i] = m_vB[i] * m_beta2 + grads.d_b()[i].cwiseProduct(grads.d_b()[i]) * (1 - m_beta2);
 
		 // Compute the bias-corrected estimates with exponential decay
		 Eigen::MatrixXf m_w_hat = m_mW[i] / (1.0f - std::pow(m_beta1, 1));
		 Eigen::MatrixXf m_b_hat = m_mB[i] / (1.0f - std::pow(m_beta1, 1));
		 Eigen::MatrixXf v_w_hat = m_vW[i] / (1.0f - std::pow(m_beta2, 1));
		 Eigen::MatrixXf v_b_hat = m_vB[i] / (1.0f - std::pow(m_beta2, 1));
 
		 // Update weights and biases
		 for (int k = 0; k < nn.m_weights[i].cols(); k++) {
		 	for (int j = 0; j < nn.m_weights[i].rows(); j++) {
				 nn.m_weights[i](j, k) -= m_learning_rate * m_w_hat(j, k) / (std::sqrt(v_w_hat(j, k)) + m_epsilon);
			 }
		 }
		 for (int j = 0; j < nn.m_biases[i].cols(); j++) {
			 nn.m_biases[i](0, j) -= m_learning_rate * m_b_hat(0, j) / (std::sqrt(v_b_hat(0, j)) + m_epsilon);
		 }
	 }
 }
 
 Adam::~Adam() {}
 
 float Adam::get_learning_rate() const {
	 return m_learning_rate;
 }
 
 // Implementation of RMSProp
 RMSProp::RMSProp(NeuralNetwork& nn, float beta, float epsilon, float learning_rate)
	 : m_beta(beta), m_epsilon(epsilon), m_learning_rate(learning_rate) {
 
	 // Create the momentum and velocity vectors for each layer
	 m_vW = std::vector<Eigen::MatrixXf>(nn.m_weights.size());
	 m_vB = std::vector<Eigen::MatrixXf>(nn.m_biases.size());
 
	 // Initialise the momentum and velocity vectors to 0
	 for (size_t i = 0; i < nn.m_weights.size(); i++) {
		 size_t layer_x = nn.m_weights[i].rows();
		 size_t layer_y = nn.m_weights[i].cols();
		 m_vW[i] = Eigen::MatrixXf::Zero(layer_x, layer_y);
	 }
	 for (size_t i = 0; i < nn.m_biases.size(); i++) {
		 size_t layer_x = nn.m_biases[i].rows();
		 size_t layer_y = nn.m_biases[i].cols();
		 m_vB[i] = Eigen::MatrixXf::Zero(layer_x, layer_y);
	 }
 }
 
 void RMSProp::update_weights(const Gradients& grads, NeuralNetwork& nn) {
	 for (size_t i = 0; i < nn.m_weights.size(); i++) {
		 // Calculate the new momentum with accumulation to the previous momentum but using the squared gradients
		 m_vW[i] = m_vW[i] * m_beta + grads.d_w()[i].cwiseProduct(grads.d_w()[i]) * (1 - m_beta);
		 m_vB[i] = m_vB[i] * m_beta + grads.d_b()[i].cwiseProduct(grads.d_b()[i]) * (1 - m_beta);
 
		 // Update weights and biases
		 for (int k = 0; k < nn.m_weights[i].cols(); k++) {
		 	for (int j = 0; j < nn.m_weights[i].rows(); j++) {
				 nn.m_weights[i](j, k) -= m_learning_rate * grads.d_w()[i](j, k) / (std::sqrt(m_vW[i](j, k)) + m_epsilon);
			 }
		 }
		 for (int j = 0; j < nn.m_biases[i].cols(); j++) {
			 nn.m_biases[i](0, j) -= m_learning_rate * grads.d_b()[i](0, j) / (std::sqrt(m_vB[i](0, j)) + m_epsilon);
		 }
	 }
 }
 
 RMSProp::~RMSProp() {}
 
 float RMSProp::get_learning_rate() const {
	 return m_learning_rate;
 }
 