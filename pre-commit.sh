#!/bin/sh

# TODO: Add the following to the .git/hooks/pre-commit file

branch=$(git rev-parse --abbrev-ref HEAD)
BOLD=$(tput bold)
RESET=$(tput sgr0)

# Build everything
mkdir -p build
cd build
cmake ..
make
cd ..

# Run the tests
echo "${BOLD}---Running tests before pushing .... ${RESET}"
if [ "$branch" = 'master' ]; then
    cd build && make check
    cd ..
    exit_code=$?
    echo "Tests exited with code $exit_code"
    if [ "$exit_code" -gt 0 ]; then
        echo "Did not push because of failing tests"
        exit "$exit_code"
    fi
fi

src_files=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(c|cpp|h|hpp)$')

# Verify some violation of code guidelines
echo "${BOLD}---Checking any code violation of the following files before pushing: $src_files ${RESET}"
if [ -n "$src_files" ]; then
    global_exit_code=0
    files_that_failed_clang_tidy=""
    for file in $src_files; do
        clang-tidy "$file" --config-file=.clang-tidy -- -I./include -I"${CMAKE_INCLUDE_PATH}" -I"${CMAKE_SOURCE_DIR}/include"
        exit_code=$?
        if [ "$exit_code" -gt 0 ]; then
            echo "clang-tidy failed on file $file : $exit_code"
            global_exit_code=$exit_code
            files_that_failed_clang_tidy="$files_that_failed_clang_tidy $file"
        fi
    done

    if [ "$global_exit_code" -gt 0 ]; then
        echo "The following files failed clang-tidy style check: $files_that_failed_clang_tidy"
        exit "$global_exit_code"
    fi
fi

# Format all the files using clang-format
if [ -n "$src_files" ]; then
    src_files=$(echo "$src_files" | tr '\n' ' ') # Replace newlines with spaces otherwise clang-format will fail
    echo "${BOLD}---Formatting the following files before pushing:${RESET} $src_files"
    for file in $src_files; do
        clang-format -i -style=file "$file"
        exit_code=$?
        if [ "$exit_code" -gt 0 ]; then
            echo "Did not push because of failing clang-format on $file"
            exit "$exit_code"
        fi
        git add "$file"
    done
    echo "clang-format exited with code $exit_code"
    if [ "$exit_code" -gt 0 ]; then
        echo "Did not push because of failing clang-format"
        exit "$exit_code"
    fi
fi

# Make the documentation
if [ -f Doxyfile ]; then
    echo "${BOLD}---Generating documentation before pushing .... ${RESET}"
    doxygen -q Doxyfile
    exit_code=$?
    echo "Doxygen exited with code $exit_code"
    if [ "$exit_code" -gt 0 ]; then
        echo "Did not push because of failing documentation generation"
        exit "$exit_code"
    fi
    git add docs/*
fi

exit 0
