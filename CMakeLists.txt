cmake_minimum_required(VERSION 3.10)
project(PPN-AI-ACCE)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
set(DEBUG_FLAGS -Og -g -Wall -Wextra -fsanitize=address -fsanitize=undefined -fsanitize=leak -fno-omit-frame-pointer)
set(RELEASE_FLAGS -O3)
set(TEST_FLAGS --coverage -s -fsanitize=address -fsanitize=undefined -fsanitize=leak -fno-omit-frame-pointer)
set(NANOBENCH_LINK_FLAGS -I${CMAKE_SOURCE_DIR}/nanobench/src/include)
set(BENCHMARK_FLAGS -lm ${NANOBENCH_LINK_FLAGS})

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/nanobench/src/include)

# Add the nanobench library
add_library(nanobench STATIC ${CMAKE_SOURCE_DIR}/nanobench/src/test/app/nanobench.cpp)
target_include_directories(nanobench PUBLIC ${CMAKE_SOURCE_DIR}/nanobench/src/include)

# Add the main executable
add_executable(main 
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/iparser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/rawbinary_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/parsing/binary_with_split_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/dataset/dataset.cpp
)
target_link_libraries(main)

# Add the test executables
enable_testing()
find_package(GTest REQUIRED)

add_executable(test_iparser 
    ${CMAKE_SOURCE_DIR}/tests/iparser.cpp 
    ${CMAKE_SOURCE_DIR}/src/parsing/iparser.cpp
)
target_link_libraries(test_iparser PUBLIC GTest::GTest GTest::Main)
target_compile_options(test_iparser PRIVATE ${DEBUG_FLAGS})
target_link_options(test_iparser PRIVATE ${TEST_FLAGS})
add_test(NAME test_iparser COMMAND test_iparser)

add_executable(test_rawbinary_parser 
    ${CMAKE_SOURCE_DIR}/tests/rawbinary_parser.cpp 
    ${CMAKE_SOURCE_DIR}/src/parsing/rawbinary_parser.cpp
)
target_link_libraries(test_rawbinary_parser PUBLIC GTest::GTest GTest::Main)
target_compile_options(test_rawbinary_parser PRIVATE ${DEBUG_FLAGS})
target_link_options(test_rawbinary_parser PRIVATE ${TEST_FLAGS})
add_test(NAME test_rawbinary_parser COMMAND test_rawbinary_parser)

add_executable(test_binary_with_split_parser 
    ${CMAKE_SOURCE_DIR}/tests/binary_with_split_parser.cpp 
    ${CMAKE_SOURCE_DIR}/src/parsing/binary_with_split_parser.cpp
)
target_link_libraries(test_binary_with_split_parser PUBLIC GTest::GTest GTest::Main)
target_compile_options(test_binary_with_split_parser PRIVATE ${DEBUG_FLAGS})
target_link_options(test_binary_with_split_parser PRIVATE ${TEST_FLAGS})
add_test(NAME test_binary_with_split_parser COMMAND test_binary_with_split_parser)

add_executable(test_xor_parser
    ${CMAKE_SOURCE_DIR}/tests/xor_parser.cpp 
    ${CMAKE_SOURCE_DIR}/src/parsing/xor_parser.cpp
)
target_link_libraries(test_xor_parser PUBLIC GTest::GTest GTest::Main)
target_compile_options(test_xor_parser PRIVATE ${DEBUG_FLAGS})
target_link_options(test_xor_parser PRIVATE ${TEST_FLAGS})
add_test(NAME test_xor_parser COMMAND test_xor_parser)

# Add the benchmark executable
add_executable(benchmark_dummy ${CMAKE_SOURCE_DIR}/benchmarks/dummy.cpp)
target_link_libraries(benchmark_dummy nanobench)
target_compile_options(benchmark_dummy PRIVATE ${RELEASE_FLAGS})
target_link_options(benchmark_dummy PRIVATE ${BENCHMARK_FLAGS})

add_executable(benchmark_vec_add ${CMAKE_SOURCE_DIR}/benchmarks/vec_add.cpp)
target_link_libraries(benchmark_vec_add nanobench)
target_compile_options(benchmark_vec_add PRIVATE ${RELEASE_FLAGS} -mavx2)
target_link_options(benchmark_vec_add PRIVATE ${BENCHMARK_FLAGS})

# Add custom targets
add_custom_target(check 
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_iparser test_rawbinary_parser test_binary_with_split_parser test_xor_parser
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

# Enable testing
enable_testing()