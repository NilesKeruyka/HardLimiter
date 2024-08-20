#!/bin/bash

# Define paths
SRC_DIR="./src"
INC_DIR="./inc"
BUILD_DIR="./build"
EXECUTABLE="$BUILD_DIR/hardlimiter"

# Check if mode is specified
if [ "$1" == "test" ]; then
    # Compile with TEST_MODE defined
    echo "Compiling in test mode..."
    gcc -DRUN_TEST -I$INC_DIR $SRC_DIR/*.c -o $EXECUTABLE
elif [ "$1" == "terminal" ]; then
    echo "Compiling in terminal mode..."
    gcc -I$INC_DIR $SRC_DIR/*.c -o $EXECUTABLE
else
    echo "Usage: $0 <mode>"
    echo "Modes:"
    echo "  terminal - Run terminal mode to process user input"
    echo "  test        - Run predefined test cases"
    exit 1
fi

# Check if the compilation succeeded
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi
echo "Compilation succeeded."

# Run the executable
echo "Running the executable..."
$EXECUTABLE
