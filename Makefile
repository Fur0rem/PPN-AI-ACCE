# --- Compilers, tools, and flags ---
CC := g++

DEBUG_FLAGS := -O0 -g -Wall -Wextra -fsanitize=address -fsanitize=undefined -fsanitize=leak -fno-omit-frame-pointer
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
%.o : src/%.cpp | bin_dir 
	$(CC) $(RELEASE_FLAGS) $< -c -o bin/$@

main: main.o | bin_dir
	$(CC) $(RELEASE_FLAGS) $(addprefix bin/, $^) -o bin/$@



# --- Tests ---
test_dummy:
test_%: tests/%.cpp | bin_dir
	$(CC) $(DEBUG_FLAGS) $(TEST_FLAGS) $< -o bin/$@

check: test_dummy
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