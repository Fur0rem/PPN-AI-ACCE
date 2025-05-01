/**
 * @file benchmarks/approximations.cpp
 * @brief Benchmark for different approximations of mathematical functions.
 */

#include <cstdlib>
#define ANKERL_NANOBENCH_IMPLEMENT ///< Needed to use nanobench
#include "../nanobench/src/include/nanobench.h"

#include <cmath>

float baseline_sigmoid(float x) {
	return 1.0F / (1 + std::exp(-x));
}

float baseline_sigmoid_derivative(float x) {
	return baseline_sigmoid(x) * (1 - baseline_sigmoid(x));
}

// Taken from http://spfrnd.de/posts/2018-03-10-fast-exponential.html
inline float truncated_product_exp(float x) {
	x = 1.0F + x / 256.0F;
	x *= x;
	x *= x;
	x *= x;
	x *= x;
	x *= x;
	x *= x;
	x *= x;
	x *= x;
	return x;
}

float truncated_product_sigmoid(float x) {
	return 1.0F / (1 + truncated_product_exp(-x));
}

float truncated_product_sigmoid_derivative(float x) {
	return truncated_product_sigmoid(x) * (1 - truncated_product_sigmoid(x));
}

inline float polynomial_exp(float x) {
	// 3rd order polynomial approximation
	// return 1.0F + (x) + (x2 * 0.5F) + (x2 * x * (1.0F / 6.0F)); // 1 + x + x^2/2 + x^3/6

	// 5th order polynomial approximation of exp(x) = 1 + x + x^2/2 + x^3/6 + x^4/24 + x^5/120
	float x2 = x * x;
	float x4 = x2 * x2;
	return 1.0F + x + (x2 * 0.5F) + (x * x2 * (1.0F / 6.0F)) + (x4 * (1.0F / 24.0F)) + (x4 * x * (1.0F / 120.0F));
}

float polynomial_sigmoid(float x) {
	return 1.0F / (1 + polynomial_exp(-x));
}

float polynomial_sigmoid_derivative(float x) {
	return polynomial_sigmoid(x) * (1 - polynomial_sigmoid(x));
}

// Taken from https://gist.github.com/jrade/293a73f89dfef51da6522428c857802d
// Paper : https://nic.schraudolph.org/pubs/Schraudolph99.pdf
inline float schraudolph_exp(float x) {
	constexpr float a = (1 << 23) / 0.69314718f;
	constexpr float b = (1 << 23) * (127 - 0.043677448f);
	x = a * x + b;

	// Bounds checking
	constexpr float c = (1 << 23);
	constexpr float d = (1 << 23) * 255;
	if (x < c || x > d) {
		x = (x < c) ? 0.0f : d;
	}

	// Cast it as int
	uint32_t n = static_cast<uint32_t>(x);
	// Transmute the bits to float
	union {
		int i;
		float f;
	} transmute_device;
	transmute_device.i = n;
	x = transmute_device.f;

	return x;
}

float schraudolph_sigmoid(float x) {
	return 1.0F / (1 + schraudolph_exp(-x));
}

float schraudolph_sigmoid_derivative(float x) {
	return schraudolph_sigmoid(x) * (1 - schraudolph_sigmoid(x));
}

int main() {
	std::ostringstream oss;
	auto result = ankerl::nanobench::Bench()
					  .epochs(5)
					  .performanceCounters(true)
					  .output(&oss)
					  .run("Baseline Sigmoid",
						   [&]() {
							   for (float i = -100.0F; i < 100.0F; i += 0.001F) {
								   baseline_sigmoid(i);
							   }
						   })
					  .run("Truncated Product Sigmoid",
						   [&]() {
							   for (float i = -100.0F; i < 100.0F; i += 0.001F) {
								   truncated_product_sigmoid(i);
							   }
						   })
					  .run("Polynomial Sigmoid",
						   [&]() {
							   for (float i = -100.0F; i < 100.0F; i += 0.001F) {
								   polynomial_sigmoid(i);
							   }
						   })
					  .run("Schraudolph Sigmoid",
						   [&]() {
							   for (float i = -100.0F; i < 100.0F; i += 0.001F) {
								   schraudolph_sigmoid(i);
							   }
						   })
					  .run("Baseline Sigmoid Derivative",
						   [&]() {
							   for (float i = -100.0F; i < 100.0F; i += 0.001F) {
								   baseline_sigmoid_derivative(i);
							   }
						   })
					  .run("Truncated Product Sigmoid Derivative",
						   [&]() {
							   for (float i = -100.0F; i < 100.0F; i += 0.001F) {
								   truncated_product_sigmoid_derivative(i);
							   }
						   })
					  .run("Polynomial Sigmoid Derivative",
						   [&]() {
							   for (float i = -100.0F; i < 100.0F; i += 0.001F) {
								   polynomial_sigmoid_derivative(i);
							   }
						   })
					  .run("Schraudolph Sigmoid Derivative",
						   [&]() {
							   for (float i = -100.0F; i < 100.0F; i += 0.001F) {
								   schraudolph_sigmoid_derivative(i);
							   }
						   })
					  .results();

	// Print oss
	// std::cout << oss.str() << '\n';
	for (auto const& res : result) {
		auto measure = res.fromString("elapsed");
		auto name = res.config().mBenchmarkName;
		std::cout << name << ", Min: " << res.minimum(measure) << "s, Max: " << res.maximum(measure) << "s, Med: " << res.median(measure)
				  << "s\n";
	}
	std::cout << "----------------------------\n";

	// Get the accuracy of the approximations compared to the baseline
	std::vector<float> errors_truncated_product;
	std::vector<float> errors_polynomial;
	std::vector<float> errors_truncated_product_derivative;
	std::vector<float> errors_polynomial_derivative;
	std::vector<float> errors_schraudolph;
	std::vector<float> errors_schraudolph_derivative;

	for (float i = -100.0F; i < 100.0F; i += 0.001F) {
		float baseline = baseline_sigmoid(i);
		float baseline_derivative = baseline_sigmoid_derivative(i);
		float truncated_product = truncated_product_sigmoid(i);
		float truncated_product_derivative = truncated_product_sigmoid_derivative(i);
		float polynomial = polynomial_sigmoid(i);
		float polynomial_derivative = polynomial_sigmoid_derivative(i);
		float schraudolph = schraudolph_sigmoid(i);
		float schraudolph_derivative = schraudolph_sigmoid_derivative(i);
		errors_truncated_product.push_back(std::abs(baseline - truncated_product));
		errors_truncated_product_derivative.push_back(std::abs(baseline_derivative - truncated_product_derivative));
		errors_polynomial.push_back(std::abs(baseline - polynomial));
		errors_polynomial_derivative.push_back(std::abs(baseline_derivative - polynomial_derivative));
		errors_schraudolph.push_back(std::abs(baseline - schraudolph));
		errors_schraudolph_derivative.push_back(std::abs(baseline_derivative - schraudolph_derivative));
	}

	// Compute the mean, max, and stddev of the errors
	auto mean_truncated_product =
		std::accumulate(errors_truncated_product.begin(), errors_truncated_product.end(), 0.0F) / errors_truncated_product.size();
	auto mean_truncated_product_derivative =
		std::accumulate(errors_truncated_product_derivative.begin(), errors_truncated_product_derivative.end(), 0.0F) /
		errors_truncated_product_derivative.size();
	auto mean_polynomial = std::accumulate(errors_polynomial.begin(), errors_polynomial.end(), 0.0F) / errors_polynomial.size();
	auto mean_polynomial_derivative = std::accumulate(errors_polynomial_derivative.begin(), errors_polynomial_derivative.end(), 0.0F) /
									  errors_polynomial_derivative.size();
	auto mean_schraudolph = std::accumulate(errors_schraudolph.begin(), errors_schraudolph.end(), 0.0F) / errors_schraudolph.size();
	auto mean_schraudolph_derivative = std::accumulate(errors_schraudolph_derivative.begin(), errors_schraudolph_derivative.end(), 0.0F) /
									   errors_schraudolph_derivative.size();

	auto max_truncated_product = *std::max_element(errors_truncated_product.begin(), errors_truncated_product.end());
	auto max_truncated_product_derivative =
		*std::max_element(errors_truncated_product_derivative.begin(), errors_truncated_product_derivative.end());
	auto max_polynomial = *std::max_element(errors_polynomial.begin(), errors_polynomial.end());
	auto max_polynomial_derivative = *std::max_element(errors_polynomial_derivative.begin(), errors_polynomial_derivative.end());
	auto max_schraudolph = *std::max_element(errors_schraudolph.begin(), errors_schraudolph.end());
	auto max_schraudolph_derivative = *std::max_element(errors_schraudolph_derivative.begin(), errors_schraudolph_derivative.end());

	auto stddev_truncated_product =
		std::sqrt(std::accumulate(errors_truncated_product.begin(),
								  errors_truncated_product.end(),
								  0.0F,
								  [&](float sum, float val) {
									  return sum + (val - mean_truncated_product) * (val - mean_truncated_product);
								  }) /
				  errors_truncated_product.size());
	auto stddev_truncated_product_derivative =
		std::sqrt(std::accumulate(errors_truncated_product_derivative.begin(),
								  errors_truncated_product_derivative.end(),
								  0.0F,
								  [&](float sum, float val) {
									  return sum + (val - mean_truncated_product_derivative) * (val - mean_truncated_product_derivative);
								  }) /
				  errors_truncated_product_derivative.size());
	auto stddev_polynomial = std::sqrt(std::accumulate(errors_polynomial.begin(),
													   errors_polynomial.end(),
													   0.0F,
													   [&](float sum, float val) {
														   return sum + (val - mean_polynomial) * (val - mean_polynomial);
													   }) /
									   errors_polynomial.size());
	auto stddev_polynomial_derivative =
		std::sqrt(std::accumulate(errors_polynomial_derivative.begin(),
								  errors_polynomial_derivative.end(),
								  0.0F,
								  [&](float sum, float val) {
									  return sum + (val - mean_polynomial_derivative) * (val - mean_polynomial_derivative);
								  }) /
				  errors_polynomial_derivative.size());
	auto stddev_schraudolph = std::sqrt(std::accumulate(errors_schraudolph.begin(),
														errors_schraudolph.end(),
														0.0F,
														[&](float sum, float val) {
															return sum + (val - mean_schraudolph) * (val - mean_schraudolph);
														}) /
										errors_schraudolph.size());
	auto stddev_schraudolph_derivative =
		std::sqrt(std::accumulate(errors_schraudolph_derivative.begin(),
								  errors_schraudolph_derivative.end(),
								  0.0F,
								  [&](float sum, float val) {
									  return sum + (val - mean_schraudolph_derivative) * (val - mean_schraudolph_derivative);
								  }) /
				  errors_schraudolph_derivative.size());

	// Print each point for all the methods
	std::cout << "----------------------------\n";
	for (float i = -10.0F; i < 10.0F; i += 0.01F) {
		std::cout << "x: " << i << ", Baseline: " << std::exp(i) << ", Truncated Product: " << truncated_product_exp(i)
				  << ", Polynomial: " << polynomial_exp(i) << ", Schraudolph: " << schraudolph_exp(i) << '\n';
	}

	// Print the results
	std::cout << "----------------------------\n";
	std::cout << "Mean Truncated Product: " << mean_truncated_product << ", Max: " << max_truncated_product
			  << ", Stddev: " << stddev_truncated_product << '\n';
	std::cout << "Mean Truncated Product Derivative: " << mean_truncated_product_derivative << ", Max: " << max_truncated_product_derivative
			  << ", Stddev: " << stddev_truncated_product_derivative << '\n';
	std::cout << "Mean Schraudolph: " << mean_schraudolph << ", Max: " << max_schraudolph << ", Stddev: " << stddev_schraudolph << '\n';
	std::cout << "Mean Schraudolph Derivative: " << mean_schraudolph_derivative << ", Max: " << max_schraudolph_derivative
			  << ", Stddev: " << stddev_schraudolph_derivative << '\n';
	std::cout << "Mean Polynomial: " << mean_polynomial << ", Max: " << max_polynomial << ", Stddev: " << stddev_polynomial << '\n';
	std::cout << "Mean Polynomial Derivative: " << mean_polynomial_derivative << ", Max: " << max_polynomial_derivative
			  << ", Stddev: " << stddev_polynomial_derivative << '\n';
	std::cout << "----------------------------\n";

	exit(EXIT_SUCCESS);
}