/**
 * @file src/topology_finder.cpp
 * @brief Uses a genetic algorithm to find the best topology for a neural network
 */

#include "neural_network/neural_network.hpp"
#include "neural_network/optimiser.hpp"
#include "parsing/cycles_encoders.hpp"
#include "parsing/hexadecimal_parser.hpp"
#include "parsing/size_encoder.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <utility>
#include <vector>

#define LOG 1
#define LOG_PRINT(x)                                                                                                                       \
	if (LOG) {                                                                                                                             \
		x                                                                                                                                  \
	}

/**
 * @brief Struct representing the metrics that we want to study for the neural network over time
 */
struct Metrics {
	float train_acc;			  ///< Training accuracy of the neural network
	float validation_acc;		  ///< Validation accuracy of the neural network
	float fitness;				  ///< Fitness of the neural network according to the genetic algorithm
	std::vector<size_t> topology; ///< Topology of the neural network
};

/**
 * @brief Class representing a candidate in the genetic algorithm
 */
class Candidate {
  private:
	std::vector<size_t> m_topology; ///< The topology of the neural network
	NeuralNetwork m_nn;				///< The neural network

  public:
	/**
	 * @brief Create a candidate with a given topology
	 */
	Candidate(std::vector<size_t> topology)
		: m_topology(std::move(topology)),
		  m_nn(NeuralNetwork(m_topology, std::make_unique<Sigmoid>(), std::make_unique<TrainingNoise>(0.0F, 0.001F, 0.1F, 0.0F))) {}

	/**
	 * @brief Create a random candidate
	 */
	Candidate() : m_nn(NeuralNetwork({0}, nullptr, nullptr)) {
		size_t nb_layers = rand() % 4 + 1;					// Random number of layers between 1 and 5
		std::vector<size_t> topology = {MAX_TOKENS_NN / 8}; // Input layer
		for (size_t i = 0; i < nb_layers; i++) {
			size_t layer_size = rand() % 1000 + 1; // Random size between 1 and 1000
			topology.push_back(layer_size);
		}
		topology.push_back(1); // Output layer
		this->m_topology = topology;
		this->m_nn = NeuralNetwork(m_topology, std::make_unique<Sigmoid>(), std::make_unique<TrainingNoise>(0.0F, 0.001F, 0.1F, 0.0F));
	}

	/**
	 * @brief Copy constructor for the Candidate class
	 * @param other The candidate to copy
	 */
	Candidate(const Candidate& other)
		: m_topology(other.m_topology),
		  m_nn(NeuralNetwork(other.m_topology, std::make_unique<Sigmoid>(), std::make_unique<TrainingNoise>(0.0F, 0.001F, 0.1F, 0.0F))) {}

	Candidate& operator=(const Candidate& other) {
		if (this != &other) {
			this->m_topology = other.m_topology;
			this->m_nn =
				NeuralNetwork(other.m_topology, std::make_unique<Sigmoid>(), std::make_unique<TrainingNoise>(0.0F, 0.001F, 0.1F, 0.0F));
		}
		return *this;
	}

	/**
	 * @brief Destructor for the Candidate class
	 */
	~Candidate() = default;

	/**
	 * @brief Get the topology of the candidate
	 * @return The topology of the candidate
	 */
	std::vector<size_t> get_topology() const {
		return m_topology;
	}

	/**
	 * @brief Set the topology of the candidate
	 * @param topology The topology to set
	 */
	void set_topology(std::vector<size_t> topology) {
		m_topology = std::move(topology);
		m_nn = NeuralNetwork(m_topology, std::make_unique<Sigmoid>(), std::make_unique<TrainingNoise>(0.0F, 0.001F, 0.1F, 0.0F));
	}

	/**
	 * @brief Mutate the topology of the candidate, either by adding or removing a layer, or modifying the number of neurons in a layer
	 * @param topology The topology of the candidate
	 */
	std::vector<size_t> mutate(std::vector<size_t> topology) {
		float chance_to_add_layer = 0.5F;
		float chance_to_remove_layer = 0.3F;
		float chance_to_modify_layer = 0.2F;

		float random_value = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		if (random_value < chance_to_add_layer) {
			// Add a layer
			size_t new_layer_size = rand() % 2000 + 1; // Random size between 1 and 2000
			// Add it before the output layer
			topology.insert(topology.end() - 1, new_layer_size);
		}
		else if (random_value < chance_to_add_layer + chance_to_remove_layer) {
			// Remove a layer
			if (topology.size() > 4) { // Ensure there are at least 4 layers (input, 2 hidden, output)
				// remove a random layer
				size_t layer_index = rand() % (topology.size() - 2) + 1; // Random layer index to remove (not the input or output layer)
				topology.erase(topology.begin() + layer_index);
			}
		}
		else if (random_value < chance_to_add_layer + chance_to_remove_layer + chance_to_modify_layer) {
			// Modify a layer
			size_t layer_index = rand() % (topology.size() - 2) + 1; // Random layer index to modify (not the input or output layer)
			float mult_factor = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.3F + 0.85F; // Between 0.85 and 1.15
			int add_factor = rand() % 200 - 100;														  // Between -100 and 100
			long int new_layer_size = static_cast<long int>(topology[layer_index] * mult_factor) + add_factor;

			// Ensure the new layer size is within bounds
			if (new_layer_size < 10) {
				new_layer_size = 10;
			}
			if (new_layer_size > 5000) {
				new_layer_size = 5000;
			}

			// Update the layer size
			topology[layer_index] = static_cast<size_t>(new_layer_size);
		}
		return topology;
	}

	/**
	 * @brief Evaluate the fitness of the candidate, it favours the validation accuracy, the training accuracy and penalises larger networks
	 * @param dataset The dataset to use for evaluation
	 * @return The fitness of the candidate
	 */
	Metrics evaluate_fitness(Dataset& dataset, int nb_epochs, float train_acc_threshold_at_half, float validation_acc_threshold_at_half) {
		LOG_PRINT(std::cout << "Evaluating candidate with topology: "; for (auto& layer
																			: m_topology) {
			std::cout << layer << " ";
		} std::cout << '\n';)

		// Train the neural network
		m_nn.reset();
		std::unique_ptr<IOptimiser> optimiser = std::make_unique<AMSGrad>(m_nn, 0.9, 0.999, 1e-8, 0.0005);
		// auto [train_acc, validation_acc] =
		// m_nn.train_batch(dataset, nb_epochs, 0.8, 16, *optimiser, std::string("/tmp/PPN_topology_finder"), 1);
		auto [train_acc, validation_acc] = m_nn.train_batch_for_topology_finder(
			dataset, nb_epochs, 0.8, 16, *optimiser, train_acc_threshold_at_half, validation_acc_threshold_at_half);

		// Compute the size penalty
		float size_penalty = 0.0F;
		for (size_t i = 0; i < m_topology.size(); i++) {
			size_penalty += static_cast<float>(m_topology[i]);
		}

		// printf("Size penalty: %f\n", size_penalty);
		// printf("Train accuracy: %f\n", train_acc);
		// printf("Validation accuracy: %f\n", validation_acc);
		// printf("Fitness = (%f + %f)² - %f\n", 1.0F + (70.0F * validation_acc), 30.0F * train_acc, std::sqrt(size_penalty));
		// printf("Fitness: %f\n", std::pow(1.0F + (70.0F * validation_acc + 30.0F * train_acc), 2) - std::pow(size_penalty, 1 / 2.0F));
		// return std::pow(1.0F + (70.0F * validation_acc + 30.0F * train_acc), 2) - std::sqrt(size_penalty);
		return Metrics{
			.train_acc = train_acc,
			.validation_acc = validation_acc,
			.fitness =
				static_cast<float>(std::pow(1.0F + (70.0F * validation_acc + 30.0F * train_acc), 2) - std::sqrt(std::sqrt(size_penalty))),
			.topology = m_topology,
		};
	}
};

/**
 * @brief Find the best topology for a neural network using a genetic algorithm
 * @param dataset The dataset to use for training
 * @param nb_epochs The number of epochs to train for (May not be trained for the full number of epochs if it's really bad)
 * @param population_size The size of the population
 * @param nb_generations The number of generations to run
 * @param selection_rate The rate of selection for the genetic algorithm
 * @return A vector of metrics for each generation
 */
std::vector<std::vector<Metrics>> find_best_topology(Dataset& dataset, int nb_epochs, size_t population_size, size_t nb_generations,
													 float selection_rate) {
	std::vector<Candidate> population(population_size);
	std::vector<Metrics> best_evolution_through_time(nb_generations);
	std::vector<std::vector<Metrics>> all_evolution_through_time(population_size);

	double best_train_acc = 0.0F;
	double best_validation_acc = 0.0F;

// Populate the initial population with random candidates in parallel
#pragma omp parallel for schedule(static)
	for (size_t i = 0; i < population_size; i++) {
		population[i] = Candidate();
	}

	population[0].set_topology({
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 16,
		MAX_TOKENS_NN / 32,
		1,
	}); // Set the first candidate to a known good topology

	population[1].set_topology({
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 32,
		1,
	}); // Set the second candidate to the topology we've been using

	for (size_t i = 0; i < nb_generations; i++) {
		std::vector<float> fitness(population_size);
		all_evolution_through_time[i].resize(population_size);

		// Evaluate the fitness of each candidate in parallel
#pragma omp parallel for schedule(dynamic)
		for (size_t j = 0; j < population_size; j++) {
			Metrics metrics = population[j].evaluate_fitness(dataset, nb_epochs, best_train_acc / 2.0F, best_validation_acc / 2.0F);
			fitness[j] = metrics.fitness;
			all_evolution_through_time[i][j] = metrics;
		}

		// Create a new population and find the best candidates
		std::vector<Candidate> new_population;
		std::vector<size_t> indices;
		new_population = std::vector<Candidate>(population_size);
		indices = std::vector<size_t>(population_size);

		// Sort the population by fitness

#pragma omp parallel for schedule(static)
		for (size_t j = 0; j < population_size; j++) {
			indices[j] = j;
		}

		std::sort(indices.begin(), indices.end(), [&fitness](size_t a, size_t b) {
			return fitness[a] > fitness[b];
		});

		// Select the best candidates
		size_t nb_selected = static_cast<size_t>(population_size * selection_rate);
#pragma omp parallel for schedule(static)
		for (size_t j = 0; j < nb_selected; j++) {
			new_population[j] = population[indices[j]];
		}

		// Mutate the selected candidates to create new candidates
#pragma omp parallel for schedule(static)
		for (size_t j = nb_selected; j < population_size; j++) {
			size_t parent_index = rand() % nb_selected;
			new_population[j] = Candidate(new_population[parent_index].mutate(new_population[parent_index].get_topology()));
		}

		// Replace the old population with the new population
		population = new_population;

		LOG_PRINT(constexpr char* bold = "\033[1m"; constexpr char* reset = "\033[0m";
				  std::cout << bold << "Generation " << reset << i + 1 << ": Best fitness = " << fitness[indices[0]] << ", ";
				  std::cout << "Best topology: ";
				  for (auto& layer
					   : population[indices[0]].get_topology()) { std::cout << layer << " "; } std::cout
				  << '\n';)

		// For each generation, store the best candidate
		best_evolution_through_time[i] = all_evolution_through_time[i][indices[0]];

		// Update the best training and validation accuracy for the next threshold
		best_train_acc = best_evolution_through_time[i].train_acc;
		best_validation_acc = best_evolution_through_time[i].validation_acc;
	}

	// Get the best candidate
	LOG_PRINT(for (size_t i = 0; i < nb_generations; i++) {
		std::cout << "Best fitness of generation " << i + 1 << ": " << best_evolution_through_time[i].fitness << ", ";
		std::cout << "Best topology: ";
		for (auto& layer : best_evolution_through_time[i].topology) {
			std::cout << layer << " ";
		}
		std::cout << '\n';
	})

	return all_evolution_through_time;
}

/**
 * @brief Find the best topology for a neural network using a genetic algorithm without early stopping
 * @param dataset The dataset to use for training
 * @param nb_epochs The number of epochs to train for
 * @param population_size The size of the population
 * @param nb_generations The number of generations to run
 * @param selection_rate The rate of selection for the genetic algorithm
 * @return A vector of metrics for each generation
 * @note This function does not use early stopping, meaning that it will train the neural network for the full number of epochs
 */
std::vector<std::vector<Metrics>> find_best_topology_no_early_stop(Dataset& dataset, int nb_epochs, size_t population_size,
																   size_t nb_generations, float selection_rate) {
	std::vector<Candidate> population(population_size);
	std::vector<Metrics> best_evolution_through_time(nb_generations);
	std::vector<std::vector<Metrics>> all_evolution_through_time(population_size);

// Populate the initial population with random candidates in parallel
#pragma omp parallel for schedule(static)
	for (size_t i = 0; i < population_size; i++) {
		population[i] = Candidate();
	}

	population[0].set_topology({
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 16,
		MAX_TOKENS_NN / 32,
		1,
	}); // Set the first candidate to a known good topology

	population[1].set_topology({
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 32,
		1,
	}); // Set the second candidate to the topology we've been using

	for (size_t i = 0; i < nb_generations; i++) {
		std::vector<float> fitness(population_size);
		all_evolution_through_time[i].resize(population_size);

		// Evaluate the fitness of each candidate in parallel
#pragma omp parallel for schedule(dynamic)
		for (size_t j = 0; j < population_size; j++) {
			Metrics metrics = population[j].evaluate_fitness(dataset, nb_epochs, 0.0F, 0.0F);
			fitness[j] = metrics.fitness;
			all_evolution_through_time[i][j] = metrics;
		}

		// Create a new population and find the best candidates
		std::vector<Candidate> new_population;
		std::vector<size_t> indices;
		new_population = std::vector<Candidate>(population_size);
		indices = std::vector<size_t>(population_size);

		// Sort the population by fitness

#pragma omp parallel for schedule(static)
		for (size_t j = 0; j < population_size; j++) {
			indices[j] = j;
		}

		std::sort(indices.begin(), indices.end(), [&fitness](size_t a, size_t b) {
			return fitness[a] > fitness[b];
		});

		// Select the best candidates
		size_t nb_selected = static_cast<size_t>(population_size * selection_rate);
#pragma omp parallel for schedule(static)
		for (size_t j = 0; j < nb_selected; j++) {
			new_population[j] = population[indices[j]];
		}

		// Mutate the selected candidates to create new candidates
#pragma omp parallel for schedule(static)
		for (size_t j = nb_selected; j < population_size; j++) {
			size_t parent_index = rand() % nb_selected;
			new_population[j] = Candidate(new_population[parent_index].mutate(new_population[parent_index].get_topology()));
		}

		// Replace the old population with the new population
		population = new_population;

		LOG_PRINT(constexpr char* bold = "\033[1m"; constexpr char* reset = "\033[0m";
				  std::cout << bold << "Generation " << reset << i + 1 << ": Best fitness = " << fitness[indices[0]] << ", ";
				  std::cout << "Best topology: ";
				  for (auto& layer
					   : population[indices[0]].get_topology()) { std::cout << layer << " "; } std::cout
				  << '\n';)

		// For each generation, store the best candidate
		best_evolution_through_time[i] = all_evolution_through_time[i][indices[0]];
	}

	// Get the best candidate
	LOG_PRINT(for (size_t i = 1; i < nb_generations; i++) {
		std::cout << "Best fitness of generation " << i + 1 << ": " << best_evolution_through_time[i].fitness << ", ";
		std::cout << "Best topology: ";
		for (auto& layer : best_evolution_through_time[i].topology) {
			std::cout << layer << " ";
		}
		std::cout << '\n';
	})

	return all_evolution_through_time;
}

/**
 * @brief Log metrics over time in a CSV file of the form: Generation,Train Accuracy,Validation Accuracy,Fitness,Topology
 * @param metrics The metrics to log
 * @param filename The name of the file to log to
 */
void log_metrics(const std::vector<std::vector<Metrics>>& metrics, const std::string& filename) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << '\n';
		return;
	}

	// CSV header
	file << "Generation,Train Accuracy,Validation Accuracy,Fitness,Topology\n";

	for (size_t generation = 0; generation < metrics.size(); generation++) {
		// Get the metrics for the current generation
		const auto& gen_metrics = metrics[generation];

		// Log the metrics for each candidate in the generation
		for (const auto& metric : gen_metrics) {
			file << generation + 1 << ',';
			file << metric.train_acc << ',' << metric.validation_acc << ',' << metric.fitness << ',';
			// Log the topology
			for (const auto& layer : metric.topology) {
				file << layer << ' ';
			}

			file << '\n';
		}
	}

	file.close();
}

#include <nanobench.h>

void strong_scaling_benchmark() {

	// Minimal topology of the neural network
	std::vector<size_t> topology = {
		MAX_TOKENS_NN / 8,
		1,
	};

	Dataset dataset = Dataset(new HexadecimalParser(),
							  "dataset/bench_bins_small",
							  topology,
							  std::make_unique<SizeEncoder>(MAX_TOKENS_NN / 8),
							  std::make_unique<CyclesLogEncoder>(2, 0));

	// // Benchmark the topology finder with and without early stopping
	// std::ostringstream oss;
	// auto result = ankerl::nanobench::Bench()
	// 				  .epochs(2)
	// 				  .performanceCounters(true)
	// 				  .output(&oss)
	// 				  .run("Topology finder with early stopping",
	// 					   [&]() {
	// 						   find_best_topology(dataset, 5, 16, 4, 0.4F);
	// 					   })
	// 				  .run("Topology finder without early stopping",
	// 					   [&]() {
	// 						   find_best_topology_no_early_stop(dataset, 5, 16, 4, 0.4F);
	// 					   })
	// 				  .results();

	// // Print oss
	// // std::cout << oss.str() << '\n';

	// for (auto const& res : result) {
	// 	auto measure = res.fromString("elapsed");
	// 	auto name = res.config().mBenchmarkName;
	// 	std::cout << name << ", Min: " << res.minimum(measure) << "s, Max: " << res.maximum(measure) << "s, Med: " << res.median(measure)
	// 			  << "s\n";
	// }

	constexpr int max_threads = 6;

	for (int nb_threads = 1; nb_threads <= max_threads; nb_threads++) {
		omp_set_num_threads(nb_threads);
		std::string name_with = "Topology finder with early stopping (nb_threads = " + std::to_string(nb_threads) + ")";
		std::string name_without = "Topology finder without early stopping (nb_threads = " + std::to_string(nb_threads) + ")";
		std::ostringstream oss;
		auto result = ankerl::nanobench::Bench()
						  .epochs(2)
						  .performanceCounters(true)
						  .output(&oss)
						  .run(name_with,
							   [&]() {
								   find_best_topology(dataset, 5, 4, 4, 0.4F);
							   })
						  .run(name_without,
							   [&]() {
								   find_best_topology_no_early_stop(dataset, 5, 4, 4, 0.4F);
							   })
						  .results();
		// Print oss
		// std::cout << oss.str() << '\n';

		for (auto const& res : result) {
			auto measure = res.fromString("elapsed");
			auto name = res.config().mBenchmarkName;
			std::cout << name << ", Min: " << res.minimum(measure) << "s, Max: " << res.maximum(measure)
					  << "s, Med: " << res.median(measure) << "s\n";
		}
		std::cout << "----------------------------\n";
	}
}

/**
 * @brief Main function to run the topology finder
 */
int main() {

	/*{
		// Seed the random number generator
		srand(0);

		// Minimal topology of the neural network
		std::vector<size_t> topology = {
			MAX_TOKENS_NN / 8,
			1,
		};

		Dataset dataset = Dataset(new HexadecimalParser(),
								  "dataset/bench_bins_small",
								  topology,
								  std::make_unique<SizeEncoder>(topology[0]),
								  std::make_unique<CyclesLogEncoder>(2, 0));

		// Find the best topology
		auto evolution = find_best_topology(dataset, 10, 5, 4, 0.4F);
		log_metrics(evolution, "training_results/topology_finder.csv");
	}*/

	// Benchmark the topology finder with and without early stopping
	strong_scaling_benchmark();
}