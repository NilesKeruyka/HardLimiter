#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "dirac.h"
#include "dirac_test_vectors.h"



// Implementations of the virtual functions
int32_t my_process_block(dirac_processor_t *processor, float *buffer, int num_samples) {
    if (!processor || !buffer || num_samples <= 0) {
        return -1; // Return an error code on failure
    }
    for (int i = 0; i < num_samples; i++) {
        float abs_value = fabs(buffer[i]); // Get the magnitude of the current sample
        if (abs_value > processor->threshold) {
            // If magnitude is greater than the threshold, set magnitude to threshold
            // while maintaining the original sign
            buffer[i] = (buffer[i] > 0) ? processor->threshold : -processor->threshold;
        }
        // Otherwise, the sample remains the same
    }
    return 0; // Success
}

int32_t my_set_parameter(dirac_processor_t *processor, int32_t parameter, float value) {
    if (!processor) {
        return -1; // Return an error code on failure
    }

    if (parameter == 1) { // It is assumed that parameter ID for threshold is 1.
        processor->threshold = value;
    } else {
        return -1;
    }

    return 0; // Success
}

float db_to_linear(float db) {
    return powf(10.0f, db / 20.0f);
}

bool compare_arrays(const float *a, const float *b, int size) {
    for (int i = 0; i < size; ++i) {
        if (fabs(a[i] - b[i]) > 1e-4f) { // Compare with a small tolerance
            return false;
        }
    }
    return true;
}

#ifdef RUN_TEST
/* TEST FUNCTIONS */
void run_test(const dirac_test_vector *test) {
    float local_output[TEST_CASE_NUM_SAMPLES];
    dirac_vtbl_t my_vtable = { 
        .process_block = my_process_block, 
        .set_parameter = my_set_parameter 
    };

    dirac_processor_t my_processor = {
        .vtbl_ptr = &my_vtable,
    };

    float threshold_linear = db_to_linear(test->threshold_db);

    if (my_processor.vtbl_ptr->set_parameter(&my_processor, 1, threshold_linear) != 0) {
        printf("Error updating the parameter.\n");
        return;
    }

    // Create a mutable copy of the input data
    float mutable_input[TEST_CASE_NUM_SAMPLES];
    for (int i = 0; i < TEST_CASE_NUM_SAMPLES; ++i) {
        mutable_input[i] = test->input[i];
    }

    if (my_processor.vtbl_ptr->process_block(&my_processor, mutable_input, TEST_CASE_NUM_SAMPLES) != 0) {
        printf("Error processing the buffer.\n");
        return;
    }

    // Copy the processed data to output array
    for (int i = 0; i < TEST_CASE_NUM_SAMPLES; ++i) {
        local_output[i] = mutable_input[i];
    }

    // Compare with expected output
    if (compare_arrays(test->output, local_output, TEST_CASE_NUM_SAMPLES)) {
        printf("Test passed.\n");
    } else {
        printf("Test failed.\n");
        printf("Threshold: %.8f\n", my_processor.threshold);
        printf("Input: ");
        for (int i = 0; i < TEST_CASE_NUM_SAMPLES; ++i) {
            printf("%.2f ", test->input[i]);
        }
        printf("\nExpected Output: ");
        for (int i = 0; i < TEST_CASE_NUM_SAMPLES; ++i) {
            printf("%.8f ", test->output[i]);
        }
        printf("\nActual Output: ");
        for (int i = 0; i < TEST_CASE_NUM_SAMPLES; ++i) {
            printf("%.8f ", local_output[i]);
        }
        printf("\n");
    }
}
#endif

int main(int argc, char *argv[]) {
#ifdef RUN_TEST
    // Run tests
    run_test(&test_case_all_pass);
    run_test(&test_case_all_clip);
    run_test(&test_case_mixed);

#else
    
    int num_samples;
    float *buffer;
    float threshold;

    // Initialize a virtuable table with function implementations
    dirac_vtbl_t my_vtable = { 
        .process_block = my_process_block, 
        .set_parameter = my_set_parameter 
    };

    // Initialize a dirac_processor_t instance
    dirac_processor_t my_processor = {
        .vtbl_ptr = &my_vtable,
        .threshold = threshold,
    };

    // Read number of samples
    printf("Enter the number of samples: ");
    if (scanf("%d", &num_samples) != 1 || num_samples <= 0) {
        printf("Invalid number of samples.\n");
        return -1;
    }

    // Dynamic memory for the buffer
    buffer = (float *)malloc(num_samples * sizeof(float));
    if (!buffer) {
        printf("Could allocate memory\n");
        return -1;
    }

    // Get audio samples
    printf("Enter %d auto data:\n", num_samples);
    for (int i = 0; i < num_samples; ++i) {
        if (scanf("%f", &buffer[i]) != 1) {
            printf("Invalid input for audio data.\n");
            free(buffer);
            return -1;
        }
    }

    // Read the threshold
    printf("Enter the threshold value in dB: ");
    if (scanf("%f", &threshold) != 1) {
        printf("Invalid threshold value.\n");
        free(buffer);
        return -1;
    }

    threshold = db_to_linear(threshold);

    // Set the threshold parameter
    if (my_processor.vtbl_ptr->set_parameter(&my_processor, 1, threshold) != 0) {
        printf("Error updating the parameter.\n");
        free(buffer);
        return -1;
    }

    // Process the buffer
    if (my_processor.vtbl_ptr->process_block(&my_processor, buffer, num_samples) != 0) {
        printf("Error processing the buffer.\n");
        free(buffer);
        return -1;
    }

    // Print the buffer after processing
    printf("Buffer after processing:\n");
    for (int i = 0; i < num_samples; ++i) {
        printf("%.4f ", buffer[i]);
    }
    printf("\n");

    // Free allocated memory
    free(buffer);

    return 0;

#endif   

}
