#include <string.h>
#include <stdlib.h>
#include "psa_prot_internal_storage.h"
#include "test_pits_impl.h"
#include "kv_config.h"
#include "KVMap.h"
#include "KVStore.h"
#include "mbed_error.h"

#ifdef   __cplusplus
extern "C"
{
#endif

using namespace mbed;

psa_its_status_t test_psa_its_reset_impl(void)
{
    psa_its_status_t status = PSA_ITS_SUCCESS;

    int kv_status = kv_init_storage_config();
    if(kv_status != MBED_SUCCESS) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    KVMap &kv_map = KVMap::get_instance();
    KVStore *kvstore = kv_map.get_main_kv_instance("/kv/");//MBED_CONF_STORAGE_DEFAULT_KV); // Danny-TODO: Use configuration name
    if (!kvstore) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    if (kvstore->reset() != MBED_SUCCESS) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    return status;
}

#ifdef   __cplusplus
}
#endif
