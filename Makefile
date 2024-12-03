# --- Compilers, tools, and flags ---
CC := g++

DEBUG_FLAGS := -Og -g -Wall -Wextra -fsanitize=address -fsanitize=undefined -fsanitize=leak -fno-omit-frame-pointer
RELEASE_FLAGS := -O4

GTEST_LINK_FLAGS := -lgtest -lgtest_main -pthread
ifeq ($(CC), g++)
	CCOV := gcov
else
	CCOV := llvm-cov
endif
TEST_FLAGS := --coverage -s $(GTEST_LINK_FLAGS)

NANOBENCH_LINK_FLAGS := -I./nanobench/src/include
BENCHMARK_FLAGS := -lm $(NANOBENCH_LINK_FLAGS)

# --- Utilities ---
all: clean check bin/main
.PHONY: all

clean:
	rm -f bin/*
# Ignore if the directory doesn't exist
	-rmdir bin 

bin_dir:
	mkdir -p bin

# --- Main executable ---
# FIXME: les .cpp du parsing sont dans des sous-dossiers, et ça cause des soucis
iparser.o: src/parsing/iparser.cpp src/parsing/iparser.hpp | bin_dir
	$(CC) $(DEBUG_FLAGS) -c $< -o bin/$@

rawbinary_parser.o: src/parsing/rawbinary_parser.cpp src/parsing/rawbinary_parser.hpp src/parsing/iparser.hpp | bin_dir
	$(CC) $(DEBUG_FLAGS) -c $< -o bin/$@

%.o: src/%.cpp | bin_dir
	$(CC) $(RELEASE_FLAGS) $< -c -o bin/$@

main: main.o | bin_dir
	$(CC) $(RELEASE_FLAGS) $(addprefix bin/, $^) -o bin/$@

# --- Tests ---
test_iparser: iparser.o
	$(CC) $(DEBUG_FLAGS) $(TEST_FLAGS) tests/iparser.cpp bin/iparser.o -o bin/test_iparser

test_rawbinary_parser: rawbinary_parser.o
	$(CC) $(DEBUG_FLAGS) $(TEST_FLAGS) tests/rawbinary_parser.cpp bin/rawbinary_parser.o -o bin/test_rawbinary_parser

check: test_rawbinary_parser test_iparser | bin_dir
	$(foreach test2run, $^, ./bin/$(test2run);)

# --- Benchmarking ---
benchmark_dummy:
benchmark_%: benchmarks/%.cpp | bin_dir
	$(CC) $(RELEASE_FLAGS) $(BENCHMARK_FLAGS) $< -o bin/$@ -lm

benchmark: benchmark_dummy
	$(foreach bench2run, $^, -./bin/$(bench2run);)

# --- Documentation ---
docs: Doxyfile src/*
	- mkdir -p docs
	doxygen Doxyfile

clean_docs:
	rm -rf docs