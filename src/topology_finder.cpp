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
#include <iostream>
#include <utility>
#include <vector>

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
		: m_topology(std::move(topology)), m_nn(NeuralNetwork(m_topology, std::make_unique<Sigmoid>())) {}

	/**
	 * @brief Create a random candidate
	 */
	Candidate() : m_nn(NeuralNetwork({0}, nullptr)) {
		size_t nb_layers = rand() % 4 + 1;					// Random number of layers between 1 and 5
		std::vector<size_t> topology = {MAX_TOKENS_NN / 8}; // Input layer
		for (size_t i = 0; i < nb_layers; i++) {
			size_t layer_size = rand() % 1000 + 1; // Random size between 1 and 1000
			topology.push_back(layer_size);
		}
		topology.push_back(1); // Output layer
		this->m_topology = topology;
		this->m_nn = NeuralNetwork(m_topology, std::make_unique<Sigmoid>());
	}

	/**
	 * @brief Copy constructor for the Candidate class
	 * @param other The candidate to copy
	 */
	Candidate(const Candidate& other) : m_topology(other.m_topology), m_nn(NeuralNetwork(other.m_topology, std::make_unique<Sigmoid>())) {}

	Candidate& operator=(const Candidate& other) {
		if (this != &other) {
			this->m_topology = other.m_topology;
			this->m_nn = NeuralNetwork(other.m_topology, std::make_unique<Sigmoid>());
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
			if (topology.size() > 3) { // Ensure there are at least 3 layers (input, hidden, output)
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
			if (new_layer_size > 4000) {
				new_layer_size = 4000;
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
	float evaluate_fitness(Dataset& dataset, int nb_epochs) {
		std::cout << "Evaluating candidate with topology: ";
		for (auto& layer : m_topology) {
			std::cout << layer << " ";
		}
		std::cout << '\n';

		// Train the neural network
		m_nn.reset();
		IOptimiser* optimiser = new Adam(m_nn, 0.9, 0.999, 1e-8, 0.001);
		auto [train_acc, validation_acc] =
			m_nn.train_batch(dataset, nb_epochs, 0.8, 16, *optimiser, std::string("/tmp/PPN_topology_finder"), 1);
		delete optimiser;

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
		return std::pow(1.0F + (70.0F * validation_acc + 30.0F * train_acc), 2) - std::sqrt(size_penalty);
	}
};

/**
 * @brief Find the best topology for a neural network using a genetic algorithm
 * @param dataset The dataset to use for evaluation
 * @param nb_epochs The number of epochs to train each candidate for
 * @param population_size The size of the population of each generation
 * @param nb_generations The number of generations to run the genetic algorithm for
 * @param selection_rate How many candidates to select for the next generation
 * @return A vector of pairs containing the fitness and topology of the best candidate of each generation
 */
std::vector<std::pair<float, std::vector<size_t>>> find_best_topology(Dataset& dataset, int nb_epochs, size_t population_size,
																	  size_t nb_generations, float selection_rate) {
	std::vector<Candidate> population(population_size);
	std::vector<std::pair<float, std::vector<size_t>>> evolution_through_time(nb_generations);

#pragma omp parallel
	{

// Populate the initial population with random candidates in parallel
#pragma omp for
		for (size_t i = 0; i < population_size; i++) {
			population[i] = Candidate();
		}
	}

	for (size_t i = 0; i < nb_generations; i++) {
		std::vector<float> fitness(population_size);

		// Evaluate the fitness of each candidate in parallel
#pragma omp for
		for (size_t j = 0; j < population_size; j++) {
			fitness[j] = population[j].evaluate_fitness(dataset, nb_epochs);
		}

		// Sort the population by fitness
		std::vector<size_t> indices(population_size);
		for (size_t j = 0; j < population_size; j++) {
			indices[j] = j;
		}
		std::sort(indices.begin(), indices.end(), [&fitness](size_t a, size_t b) {
			return fitness[a] > fitness[b];
		});

		// Create a new population
		std::vector<Candidate> new_population(population_size);

		// Select the best candidates
		size_t nb_selected = static_cast<size_t>(population_size * selection_rate);
#pragma omp for
		for (size_t j = 0; j < nb_selected; j++) {
			new_population[j] = population[indices[j]];
		}

		// Mutate the selected candidates to create new candidates
#pragma omp for
		for (size_t j = nb_selected; j < population_size; j++) {
			size_t parent_index = rand() % nb_selected;
			new_population[j] = Candidate(new_population[parent_index].mutate(new_population[parent_index].get_topology()));
		}

		// Replace the old population with the new population
		population = new_population;
		std::cout << "Generation " << i + 1 << ": Best fitness = " << fitness[indices[0]] << ", ";
		std::cout << "Best topology: ";
		for (auto& layer : population[indices[0]].get_topology()) {
			std::cout << layer << " ";
		}
		std::cout << '\n';

		// For each generation, store the best candidate
		evolution_through_time[i].first = fitness[indices[0]];
		evolution_through_time[i].second = population[indices[0]].get_topology();
	}

	// Get the best candidate
	for (size_t i = 1; i < nb_generations; i++) {
		std::cout << "Best fitness of generation " << i + 1 << ": " << evolution_through_time[i].first << ", ";
		std::cout << "Best topology: ";
		for (auto& layer : evolution_through_time[i].second) {
			std::cout << layer << " ";
		}
		std::cout << '\n';
	}

	return evolution_through_time;
}

/**
 * @brief Main function to run the topology finder
 */

int main() {

	// Seed the random number generator
	srand(time(NULL));

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
	auto evolution = find_best_topology(dataset, 30, 15, 5, 0.4F);
	std::cout << "Best topology: ";
	for (auto& layer : evolution.back().second) {
		std::cout << layer << " ";
	}
	std::cout << '\n';
	std::cout << "Best fitness: " << evolution.back().first << '\n';

	// Train the neural network with the best topology
	NeuralNetwork nn(evolution.back().second, std::make_unique<Sigmoid>());
	auto [train_acc, validation_acc] =
		nn.train_batch(dataset, 100, 0.8, 16, *new Adam(nn, 0.9, 0.999, 1e-8, 0.001), std::string("/tmp/PPN_topology_finder"), 1);
	std::cout << "Training accuracy: " << train_acc << ", Validation accuracy: " << validation_acc << '\n';
}