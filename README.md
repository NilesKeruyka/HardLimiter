# Directory Structure for Dirac Programming Task

```plaintext
dirac_programming_task_v2/
├── build/
│   └── (Compiled executables and build artifacts will be placed here)
├── inc/
│   ├── dirac.h
│   └── dirac_test_vectors.h
├── src/
│   └── hardlimiter.c
└── run.sh


hardlimiter.c is the main file for function calls. run.sh is a bash script for compiling and running the program. It has two modes: test to run predefined test cases or terminal to get input from command line.

chmod +x run.sh
./run.sh test : for tests
./run.sh terminal: for command line inputs <number_of_samples><sample1>...<sampleN><threshold_db>

The general approach of the program is to replace samples in a buffer that exceed a global threshold with the threshold value using a loop. To be able to run the code with different inputs without recompiling, command line inputs can be used.

It is assumed that enough space is available to allocate to buffer and the number of samples is in a reasonable range.

