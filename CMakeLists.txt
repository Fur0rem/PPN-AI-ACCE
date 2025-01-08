cmake_minimum_required(VERSION 3.10)
project(PPN-AI-ACCE)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
set(SANITIZER_LIB -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer -fsanitize=undefined)
set(DEBUG_FLAGS -Og -g -Wall -Wextra ${SANITIZER_LIB})
set(RELEASE_FLAGS -O3)
set(TEST_FLAGS --coverage ${SANITIZER_LIB})
set(NANOBENCH_LINK_FLAGS -I${CMAKE_SOURCE_DIR}/nanobench/src/include)
set(BENCHMARK_FLAGS -lm ${NANOBENCH_LINK_FLAGS})

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/nanobench/src/include)

# Add the Eigen library
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIR})

# Add the nanobench library
add_library(nanobench STATIC ${CMAKE_SOURCE_DIR}/nanobench/src/test/app/nanobench.cpp)
target_include_directories(nanobench PUBLIC ${CMAKE_SOURCE_DIR}/nanobench/src/include)

# Add the main executable
add_executable(main 
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/iparser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/rawbinary_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/binary_with_split_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/rawbinary_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/xor_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/neural_network/activation_functions.cpp
    ${CMAKE_SOURCE_DIR}/src/neural_network/layer.cpp
    ${CMAKE_SOURCE_DIR}/src/neural_network/neural_network.cpp
    ${CMAKE_SOURCE_DIR}/src/dataset/dataset.cpp
)
target_link_libraries(main)
target_compile_options(main PRIVATE ${RELEASE_FLAGS})

# Add the test executables
enable_testing()
find_package(GTest REQUIRED)

add_executable(test_iparser 
    ${CMAKE_SOURCE_DIR}/tests/iparser.cpp 
    ${CMAKE_SOURCE_DIR}/src/parsing/iparser.cpp
)
target_link_libraries(test_iparser PUBLIC GTest::GTest GTest::Main)
target_compile_options(test_iparser PRIVATE ${DEBUG_FLAGS} ${TEST_FLAGS})
target_link_options(test_iparser PRIVATE ${DEBUG_FLAGS} ${TEST_FLAGS})
add_test(NAME test_iparser COMMAND test_iparser)

add_executable(test_rawbinary_parser 
    ${CMAKE_SOURCE_DIR}/tests/rawbinary_parser.cpp 
    ${CMAKE_SOURCE_DIR}/src/parsing/rawbinary_parser.cpp
)
target_link_libraries(test_rawbinary_parser PUBLIC GTest::GTest GTest::Main)
target_compile_options(test_rawbinary_parser PRIVATE ${DEBUG_FLAGS} ${TEST_FLAGS})
target_link_options(test_rawbinary_parser PRIVATE ${DEBUG_FLAGS} ${TEST_FLAGS})
add_test(NAME test_rawbinary_parser COMMAND test_rawbinary_parser)

add_executable(test_binary_with_split_parser 
    ${CMAKE_SOURCE_DIR}/tests/binary_with_split_parser.cpp 
    ${CMAKE_SOURCE_DIR}/src/parsing/binary_with_split_parser.cpp
)
target_link_libraries(test_binary_with_split_parser PUBLIC GTest::GTest GTest::Main)
target_compile_options(test_binary_with_split_parser PRIVATE ${DEBUG_FLAGS} ${TEST_FLAGS})
target_link_options(test_binary_with_split_parser PRIVATE ${DEBUG_FLAGS} ${TEST_FLAGS})
add_test(NAME test_binary_with_split_parser COMMAND test_binary_with_split_parser)

add_executable(test_recognises_xor 
    ${CMAKE_SOURCE_DIR}/tests/recognises_xor.cpp 
    ${CMAKE_SOURCE_DIR}/src/neural_network/activation_functions.cpp
    ${CMAKE_SOURCE_DIR}/src/neural_network/layer.cpp
    ${CMAKE_SOURCE_DIR}/src/neural_network/neural_network.cpp
)
target_link_libraries(test_recognises_xor PUBLIC GTest::GTest GTest::Main)
target_compile_options(test_recognises_xor PRIVATE ${DEBUG_FLAGS} ${TEST_FLAGS})
target_link_options(test_recognises_xor PRIVATE ${DEBUG_FLAGS} ${TEST_FLAGS})
add_test(NAME test_recognises_xor COMMAND test_recognises_xor)

# Add the benchmark executable
add_executable(benchmark_dummy ${CMAKE_SOURCE_DIR}/benchmarks/dummy.cpp)
target_link_libraries(benchmark_dummy nanobench)
target_compile_options(benchmark_dummy PRIVATE ${RELEASE_FLAGS})
target_link_options(benchmark_dummy PRIVATE ${BENCHMARK_FLAGS})

add_executable(benchmark_vec_add ${CMAKE_SOURCE_DIR}/benchmarks/vec_add.cpp)
target_link_libraries(benchmark_vec_add nanobench)
target_compile_options(benchmark_vec_add PRIVATE ${RELEASE_FLAGS} -mavx2)
target_link_options(benchmark_vec_add PRIVATE ${BENCHMARK_FLAGS})

add_executable(benchmark_mat_mul ${CMAKE_SOURCE_DIR}/benchmarks/array_copy.cpp)
target_link_libraries(benchmark_mat_mul nanobench)
target_compile_options(benchmark_mat_mul PRIVATE ${RELEASE_FLAGS} -mavx2)
target_link_options(benchmark_mat_mul PRIVATE ${BENCHMARK_FLAGS})

# Add custom targets
add_custom_target(check 
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_iparser test_rawbinary_parser test_binary_with_split_parser
)
add_custom_target(benchmark COMMAND ${CMAKE_BINARY_DIR}/benchmark_dummy)

# Add clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_clean.cmake
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
)

# Generate a cmake_clean.cmake file to clean the build directory
file(WRITE ${CMAKE_BINARY_DIR}/cmake_clean.cmake
    "file(GLOB_RECURSE files \"${CMAKE_BINARY_DIR}/*\")\n"
    "foreach(file \${files})\n"
    "    if(NOT IS_DIRECTORY \${file})\n"
    "        file(REMOVE \${file})\n"
    "    endif()\n"
    "endforeach()\n"
)

enable_testing()