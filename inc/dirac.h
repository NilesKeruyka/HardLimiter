#ifndef DIRAC_H
#define DIRAC_H

#include <stdint.h>

/**
 * @brief Plain C interface wrapper for the function vTable described in @ref dirac_vtbl_t.
 */
typedef struct dirac_processor_t dirac_processor_t;

typedef struct {
    /**
     * @brief Process a block of audio data using a Dirac Processor
     *
     * @return 0 on success, non 0 on failure.
     */
    int32_t (*process_block)(dirac_processor_t *processor, /**< [in] Handle to the Dirac Processor instance */
                             float *buffer,  /**< [in,out] An inplace mono buffer of audio data to process */
                             int num_samples /**< [in]     Number of samples in @ref buffer */
    );

    /**
     * @brief Sets a parameter value in a Dirac Processor
     *
     * @return Returns 0 on success and non 0 on failure.
     */
    int32_t (*set_parameter)(dirac_processor_t *processor, /**< [in] Handle to the Dirac Processor instance */
                             int32_t parameter,            /**< [in] A parameter ID */
                             float value                   /**< [in] The value of said parameter*/
    );

} dirac_vtbl_t;

/**
 * @brief Plain C interface wrapper for the virtual function table, @ref dirac_vtbl_t.
 *
 * This @ref dirac_vtbl_t structure appears to the caller as a virtual function table.
 * The virtual function table in the instance structure is followed by other
 * structure elements, but those are invisible to the users of the object.
 *
 * This @ref dirac_vtbl_t structure is all that is publicly visible.
 * 
 * threshold element is unique to each to processor object
 */
struct dirac_processor_t {
    dirac_vtbl_t *vtbl_ptr; /**< Pointer to the virtual function table. */
    float threshold; /**< Threshold limit */
};

#endif // DIRAC_H

