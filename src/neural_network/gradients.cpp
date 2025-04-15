/**
 * @file src/neural_network/gradients.cpp
 * @brief Implementation of the Gradients class
 */

#include "neural_network/gradients.hpp"
#include <eigen3/Eigen/Dense>

std::vector<Eigen::MatrixXf>& Gradients::d_w() {
	return m_dW;
}

std::vector<Eigen::MatrixXf>& Gradients::d_b() {
	return m_dB;
}

const std::vector<Eigen::MatrixXf>& Gradients::d_w() const {
	return m_dW;
}

const std::vector<Eigen::MatrixXf>& Gradients::d_b() const {
	return m_dB;
}

Gradients::Gradients(size_t nb_layers) {
	m_dW.resize(nb_layers);
	m_dB.resize(nb_layers);
	for (size_t i = 0; i < nb_layers; i++) {
		m_dW[i] = Eigen::MatrixXf::Zero(1, 1);
		m_dB[i] = Eigen::MatrixXf::Zero(1, 1);
	}
}

float mse_loss(const Eigen::MatrixXf& prediction, const Eigen::MatrixXf& target) {
	float loss = 0.0f;
	for (size_t i = 0; i < prediction.rows(); i++) {
		for (size_t j = 0; j < prediction.cols(); j++) {
			loss += std::pow(target(i, j) - prediction(i, j), 2);
		}
	}
	loss /= target.rows();
	return loss;
}

Eigen::MatrixXf mse_loss_derivative(const Eigen::MatrixXf& prediction, const Eigen::MatrixXf& target) {
	Eigen::MatrixXf result(prediction.rows(), prediction.cols());
	for (size_t i = 0; i < prediction.rows(); i++) {
		for (size_t j = 0; j < prediction.cols(); j++) {
			if (target(i, j) > prediction(i, j)) {
				result(i, j) = 2.0f * (prediction(i, j) - target(i, j));
			}
			else if (target(i, j) < prediction(i, j)) {
				result(i, j) = -2.0f * (target(i, j) - prediction(i, j));
			}
			else {
				result(i, j) = 0.0f;
			}
		}
	}
	return result;
}

float cross_entropy_loss(const Eigen::MatrixXf& prediction, const Eigen::MatrixXf& target) {
	float loss = 0.0f;
	for (size_t i = 0; i < prediction.rows(); i++) {
		for (size_t j = 0; j < prediction.cols(); j++) {
			loss += target(i, j) * std::log(prediction(i, j));
		}
	}
	loss /= target.rows();
	return loss;
}

Eigen::MatrixXf cross_entropy_loss_derivative(const Eigen::MatrixXf& prediction, const Eigen::MatrixXf& target) {
	Eigen::MatrixXf result(prediction.rows(), prediction.cols());
	for (size_t i = 0; i < prediction.rows(); i++) {
		for (size_t j = 0; j < prediction.cols(); j++) {
			if (prediction(i, j) > 0.0f) {
				result(i, j) = -target(i, j) / prediction(i, j);
			}
			else {
				result(i, j) = 0.0f;
			}
		}
	}
	return result;
}

float mae_loss(const Eigen::MatrixXf& prediction, const Eigen::MatrixXf& target) {
	float loss = 0.0f;
	for (size_t i = 0; i < prediction.rows(); i++) {
		for (size_t j = 0; j < prediction.cols(); j++) {
			loss += std::abs(target(i, j) - prediction(i, j));
		}
	}
	loss /= target.rows();
	return loss;
}

Eigen::MatrixXf mae_loss_derivative(const Eigen::MatrixXf& prediction, const Eigen::MatrixXf& target) {
	Eigen::MatrixXf result(prediction.rows(), prediction.cols());
	for (size_t i = 0; i < prediction.rows(); i++) {
		for (size_t j = 0; j < prediction.cols(); j++) {
			if (target(i, j) > prediction(i, j)) {
				result(i, j) = -1.0f;
			}
			else if (target(i, j) < prediction(i, j)) {
				result(i, j) = 1.0f;
			}
			else {
				result(i, j) = 0.0f;
			}
		}
	}
	return result;
}