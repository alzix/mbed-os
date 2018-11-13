#ifndef __TEST_INTERNAL_TRUSTED_STORAGE_H__
#define __TEST_INTERNAL_TRUSTED_STORAGE_H__

/** @file
@brief This file describes the PSA Internal Trusted Storage API
*/

#include <stddef.h>
#include <stdint.h>
#include "psa_prot_internal_storage.h"

#ifdef   __cplusplus
extern "C"
{
#endif

/**
 * \brief Remove the provided key and its associated data from the storage
 *
 * \param[in] uid   The uid value
 *
 * \return  A status indicating the success/failure of the operation
 *
 * \retval      PSA_ITS_SUCCESS                  The operation completed successfully
 * \retval      PSA_ITS_ERROR_STORAGE_FAILURE    The operation failed because the physical storage has failed (Fatal error)
 */
psa_its_status_t test_psa_its_reset(void);

#ifdef   __cplusplus
}
#endif

#endif // __TEST_INTERNAL_TRUSTED_STORAGE_H__
