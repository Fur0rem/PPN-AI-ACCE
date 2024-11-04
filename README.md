# Repository for the course PPN (Projet de Programmation Numérique): AI-ACCE (AI based Assembly Code Cycles Estimator)
Link : https://academic.liparad.uvsq.fr/m1chps/ppn/projets.html#ai-acce-ai-based-assembly-code-cycles-estimator

# Current status of the project
Currently nothing has been done yet, just the base git structure of the project has been set up.
Done/To do:
- [x] Set up the project
- [] Implement the AI model

# Dependencies
- C++20
- Makefile
- GTest
- clang-format and clang-tidy
- Doxygen (if you want to generate the documentation)
- nanobench (already included in the project as a submodule)

# How to run the project
- Clone the repository
- Now there are 3 targets in the Makefile:
    - `make main` to compile the main file
    - `make check` to compile and run the tests
    - `make benchmarks` to compile and run the benchmarks
    - `make clean` to clean the project
- Run the target you want to run

# Project structure
```
├── benchmarks      # Contains the benchmarks source code
├── bin             # Contains the compiled files (including the tests and benchmarks)
├── docs            # Contains the documentation
├── nanobench
├── experiments     # Contains several experiments using pre-made frameworks we did not related to the main project
├── src             # Contains the source code
│   └── x.cpp
├── tests           # Contains the tests source code
│   └── x.cpp
├── Makefile        # To compile the project
├── default.nix     # Nix file to create a development environment
├── Doxyfile        # Doxygen configuration file
└── .clang files    # Contains the clang files for formatting and linting like .clang-format, .clang-tidy and .clangd
```
The project will automatically format, test and generate the documentation when committing with the .git/hooks/pre-commit file.