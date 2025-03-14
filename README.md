# Repository for the course PPN (Projet de Programmation Numérique): AI-ACCE (AI based Assembly Code Cycles Estimator)

Link : <https://academic.liparad.uvsq.fr/m1chps/ppn/projets.html#ai-acce-ai-based-assembly-code-cycles-estimator>

## Current status of the project

Done/To do:

- [x] Set up the project
- [x] Add parsing of the assembly code
- [x] Get data to train the AI model
- [x] Implement the AI model
- [ ] Improve accuracy of the AI model
- [ ] Improve speed of the AI model

## Dependencies

- C++17
- CMake
- GTest
- clang-format and clang-tidy
- Doxygen (if you want to generate the documentation)
- nanobench (already included in the project as a submodule)
- eigen

## How to run the project

- Clone the repository with submodules: `git clone --recurse-submodules
- Build it with CMake:
  - `mkdir build`
  - `cd build`
  - `cmake ..`
  - `make`
- Now you can run the tests with `make check`
- You can extract the data with `$/build/> ./extract_data`
- You can run the AI model with `$/> ./main`

## Project structure

```r
├── benchmarks              # Contains the benchmarks to generate the data
│   └── op.cpp
├── bin                     # Contains the compiled files (including the tests and benchmarks)
├── dataset                 # Contains the training data
│   ├── bench_bins          # Contains the binaries of the benchmarks along with the cycles count
│   ├── bench_srcs_small    # Contains a small subset of the benchmarks binaries for testing
├── docs                    # Contains the documentation
├── nanobench               # Contains the nanobench submodule
├── experiments             # Contains several experiments using pre-made frameworks we did not related to the main project
├── include                 # Contains the header files
│   └── x.hpp
├── src                     # Contains the source code
│   └── x.cpp
├── tests                   # Contains the tests source code
│   └── test_x.cpp
├── CMakeLists.txt          # To compile the project
├── default.nix             # Nix file to create a development environment
├── Doxyfile                # Doxygen configuration file
└── .clang files            # Contains the clang files for formatting and linting like .clang-format, .clang-tidy and .clangd
```

Move the pre-commit.sh file to .git/hooks/pre-commit to make the project automatically format, test and generate the documentation when committing.
