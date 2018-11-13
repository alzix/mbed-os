#include <string.h>
#include <stdlib.h>
#include "psa_prot_internal_storage.h"
#include "test_pits_impl.h"
#include "nvstore.h"

#ifdef   __cplusplus
extern "C"
{
#endif

psa_its_status_t test_psa_its_reset_impl(void)
{
    psa_its_status_t status = PSA_ITS_SUCCESS;
    NVStore &nvstore = NVStore::get_instance();

    if (nvstore.reset() != NVSTORE_SUCCESS) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    return status;
}

#ifdef   __cplusplus
}
#endif
