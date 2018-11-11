#include <stdlib.h>
#include <string.h>

#include "psa_prot_internal_storage.h"
#include "pits_impl.h"


#define PSA_ITS_EMUL_PID        0xFFFFFFFF  // In EMUL world, there is no real partitioning, which makes the source partition irrelevant.
                                            // So here we set a global pid value to be used for when calling IMPL functions

psa_its_status_t psa_its_set(uint32_t uid, uint32_t data_length, const void *p_data, psa_its_create_flags_t create_flags)
{
    if (p_data == NULL) {
        return PSA_ITS_ERROR_BAD_POINTER;
    }

    const uint32_t extended_length = data_length + PITS_HEADER_SIZE;

    // Make sure add operation does not wrap UINT32_MAX
    if(extended_length < data_length) {
       return PSA_ITS_ERROR_INSUFFICIENT_SPACE;
    }

    uint8_t *record = (uint8_t *)malloc(extended_length);
    if (record == NULL) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    add_headers_to_record(record, create_flags);
    memcpy(PITS_DATA_PTR(record), p_data, data_length);

    psa_its_status_t res = psa_its_set_impl(PSA_ITS_EMUL_PID, uid, extended_length, record, create_flags);

    memset(record, 0, extended_length);
    free(record);

    return res;
}

psa_its_status_t psa_its_get(uint32_t uid, uint32_t data_offset, uint32_t data_length, void *p_data)
{
    if (p_data == NULL) {
        return PSA_ITS_ERROR_BAD_POINTER;
    }

    return psa_its_get_impl(PSA_ITS_EMUL_PID, uid, data_offset, data_length, p_data);
}

psa_its_status_t psa_its_get_info(uint32_t uid, struct psa_its_info_t *p_info)
{
    if (p_info == NULL) {
        return PSA_ITS_ERROR_BAD_POINTER;
    }

    return psa_its_get_info_impl(PSA_ITS_EMUL_PID, uid, p_info);
}

psa_its_status_t psa_its_remove(uint32_t uid)
{
    return psa_its_remove_impl(PSA_ITS_EMUL_PID, uid);
}
