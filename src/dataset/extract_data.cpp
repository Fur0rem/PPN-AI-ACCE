/**
 * @file src/dataset/extract_data.cpp
 * @brief Runs the benchmarks and extracts their assembly code
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

std::pair<std::string, int> exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	int exit_status = 0;

	using PipeDeleter = int (*)(FILE*);
	std::unique_ptr<FILE, PipeDeleter> pipe(popen(cmd, "r"), pclose);

	if (!pipe) {
		throw std::runtime_error("popen() failed!");
		exit(EXIT_FAILURE);
	}

	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
		result += buffer.data();
	}

	exit_status = pclose(pipe.release());
	return {result, exit_status};
}

std::string& ltrim(std::string& str) {
	auto it2 = std::find_if(str.begin(), str.end(), [](char ch) {
		return !std::isspace<char>(ch, std::locale::classic());
	});
	str.erase(str.begin(), it2);
	return str;
}

std::string& rtrim(std::string& str) {
	auto it1 = std::find_if(str.rbegin(), str.rend(), [](char ch) {
		return !std::isspace<char>(ch, std::locale::classic());
	});
	str.erase(it1.base(), str.end());
	return str;
}

std::string& trim(std::string& str) {
	return ltrim(rtrim(str));
}

int main() {
	constexpr auto build_dir = "."; // The executable is in the build directory

	// List all the files starting with "benchmark" in the build directory, then collect them all
	std::string cmd = "ls " + std::string(build_dir) + " | grep benchmark";
	auto [output, exit_status] = exec(cmd.c_str());
	if (exit_status != 0) {
		std::cerr << "Error while listing the files in the build directory" << '\n';
		return 1;
	}
	std::vector<std::string> benchmarks;
	std::istringstream iss(output);
	std::string benchmark;
	while (iss >> benchmark) {
		benchmarks.push_back(benchmark);
	}

	// Read all the files
	size_t nb_extracted = 0;
	for (const auto& benchmark : benchmarks) {

		std::cout << '\n' << "Benchmark file : " << benchmark << '\n';

		// Run the benchmark and save the output
		std::string cmd = "./" + std::string(build_dir) + "/" + benchmark;
		auto [bench_times, exit_status] = exec(cmd.c_str());
		if (exit_status != 0) {
			std::cerr << "Error while running the benchmark" << '\n';
			return 1;
		}

		cmd = "objdump -d " + std::string(build_dir) + "/" + benchmark;
		auto [assembly, exit_status2] = exec(cmd.c_str());
		if (exit_status2 != 0) {
			std::cerr << "Error while running objdump" << '\n';
			return 1;
		}

		// Read the whole assembly file
		std::istringstream iss(assembly);
		std::string content;
		std::string line;
		while (std::getline(iss, line)) {
			content += line + '\n';
		}

		// Extract every function name from the benchmark output
		std::istringstream iss2(bench_times);
		std::string line2;

		while (std::getline(iss2, line2)) {
			std::string func_name;
			std::string time;
			if (line2.find(" : ") != std::string::npos) {
				func_name = line2.substr(0, line2.find(" : "));
				// remove the _start_PPN_LABEL from the function name
				func_name = func_name.substr(0, func_name.find("_start_PPN_LABEL"));
				std::cout << "Function name : " << func_name << '\n';

				// Get the time
				time = line2.substr(line2.find(" : ") + 3);
				std::cout << "Time : " << time << '\n';
			}

			// Find the first line of the function in the assembly
			if (!func_name.empty()) {
				// Find the start label
				std::string start_label = func_name + "_start_PPN_LABEL";
				size_t start_index = content.find(start_label);
				if (start_index == std::string::npos) {
					throw std::runtime_error(std::string("Start label not found for function ") + start_label);
				}
				// assert that there is a >: in the same line
				while (true) {
					for (size_t pos = start_index; content[pos] != '\n'; pos++) {
						if (content[pos] == '>' && content[pos + 1] == ':') {
							goto found_definition;
						}
					}
					start_index = content.find(start_label, start_index + 1);
				}
			found_definition:

				// Find the end label
				std::string end_label = func_name + "_end_PPN_LABEL>:";
				size_t end_index = content.find(end_label);
				size_t next_empty_line;
				if (end_index == std::string::npos) {
					// If the end label doesn't exist, then just find the next empty line
					next_empty_line = content.find("\n\n", start_index);
				}
				else {
					// If the end label exists, find the next empty line after it
					next_empty_line = content.find("\n\n", end_index);
				}

				if (next_empty_line == std::string::npos) {
					throw std::runtime_error("No empty line found after the end label nor function end");
				}

				// Extract the function code
				std::string func_code = content.substr(start_index, next_empty_line - start_index);
				std::cout << "Function code : " << '\n' << func_code << '\n';

				// Extract the binary part from the source code
				std::istringstream iss3(func_code);
				std::string binary_code;
				while (std::getline(iss3, line)) {
					// Check if the line is a label
					if (line.find(">:") != std::string::npos) {
						continue;
					}

					// Check if it's a format line
					if (line.find("format") != std::string::npos) {
						continue;
					}

					// Read everything between : and the first letter
					std::istringstream iss4(line);
					std::string word;
					while (std::getline(iss4, word)) {
						auto colon_index = word.find(':');
						size_t first_letter_index;
						// While you can read a pair of two valid hexadecimal characters
						word = word.substr(colon_index + 1);
						word = ltrim(word);

						for (size_t i = 0; i < word.size(); i += 3) {
							if (isxdigit(word[i]) && isxdigit(word[i + 1]) && word[i + 2] == ' ') {
								first_letter_index = i;
							}
							else {
								break;
							}
						}
						first_letter_index += 3;

						if (colon_index != std::string::npos && first_letter_index != std::string::npos) {
							std::string extracted = word.substr(0, first_letter_index);
							extracted = trim(extracted);
							binary_code += extracted + '\n';
						}
					}
				}

				std::cout << "Binary code : " << '\n' << binary_code << '\n';

				// Parse the time (in cycles)
				size_t nb_cycles = static_cast<size_t>(std::stof(time));

				// Write it into a file called ../dataset/bench_bins/benchmark_name_time
				std::string filename =
					"../dataset/bench_bins/" + std::string(benchmark) + "_" + func_name + "_" + std::to_string(nb_cycles);
				std::ofstream file(filename);
				file << ";; Cycles : " << nb_cycles << "\n\n";
				file << binary_code;

				nb_extracted++;
			}
		}
	}

	std::cout << "Extracted " << nb_extracted << " kernels" << '\n';

	exit(EXIT_SUCCESS);
}