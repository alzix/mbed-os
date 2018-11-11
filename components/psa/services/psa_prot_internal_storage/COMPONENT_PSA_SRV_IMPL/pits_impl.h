#ifndef __PITS_IMPL_H__
#define __PITS_IMPL_H__

#include "psa_prot_internal_storage.h"

#ifdef   __cplusplus
extern "C"
{
#endif


#define PITS_HEADER_SIZE (sizeof(psa_its_create_flags_t))
#define PITS_FLAGS_PTR(ptr) ((void *)((uintptr_t)ptr))
#define PITS_DATA_PTR(ptr) ((void *)(((uintptr_t)ptr) + ((uintptr_t)PITS_HEADER_SIZE)))
#define PITS_DATA_PTR_AT_OFFSET(ptr, offset) ((void *)(((uintptr_t)ptr) + ((uintptr_t)PITS_HEADER_SIZE) + ((uintptr_t)offset)))

void add_headers_to_record(void *record, psa_its_create_flags_t create_flags);
psa_its_status_t psa_its_set_impl(uint32_t uid, uint32_t data_length, const void *p_data, psa_its_create_flags_t create_flags);
psa_its_status_t psa_its_get_impl(uint32_t uid, uint32_t data_offset, uint32_t data_length, void *p_data);
psa_its_status_t psa_its_get_info_impl(uint32_t uid, struct psa_its_info_t *p_info);
psa_its_status_t psa_its_remove_impl(uint32_t uid);

#ifdef   __cplusplus
}
#endif

#endif // __PITS_IMPL_H__
