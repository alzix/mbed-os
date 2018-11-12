/* Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstring>
#include "KVMap.h"
#include "KVStore.h"
#include "TDBStore.h"
#include "psa_prot_internal_storage.h"
#include "pits_impl.h"

#ifdef   __cplusplus
extern "C"
{
#endif

using namespace mbed;

#define PSA_ITS_FILENAME_LEN_BYTES      14          // Maximum length of filename we use for kvstore API.
                                                    // uid: 6; delimiter: 1; pid: 6; str terminator: 1


const uint8_t fn_coding_table[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', '$', '@'
};


static KVStore *get_kvstore_instance(void)
{
    KVMap &kv_map = KVMap::get_instance();

    return kv_map.get_main_kv_instance("/kv/");//MBED_CONF_STORAGE_DEFAULT_KV); // Danny-TODO: Use configuration name
}

static void generate_fn(uint8_t *tdb_filename, uint32_t uid, uint32_t pid)
{
    // Note: tdb_filename assumed to be of size PSA_ITS_FILENAME_LEN_BYTES and reset with zeros

    uint8_t filename_idx = 0;
    uint32_t tmp_uid = uid;
    uint32_t tmp_pid = pid;

    // Iterate on UID; each time convert 6 bits of UID into a character; first iteration must be done
    do {
        tdb_filename[filename_idx++] = fn_coding_table[tmp_uid & 0x3F];
        tmp_uid = tmp_uid >> 6;
    } while (tmp_uid != 0);

    // Write delimiter
    tdb_filename[filename_idx++] = '#';

    // Iterate on PID; each time convert 6 bits of PID into a character; first iteration must be done
    do {
        tdb_filename[filename_idx++] = fn_coding_table[tmp_pid & 0x3F];
        tmp_pid = tmp_pid >> 6;
    } while (tmp_pid != 0);
}


psa_its_status_t psa_its_set_impl(uint32_t pid, uint32_t uid, uint32_t data_length, const void *p_data, psa_its_create_flags_t create_flags)
{
    KVStore *kvstore = get_kvstore_instance();
    if (!kvstore) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    // Generate KVStore key
    uint8_t kv_key[PSA_ITS_FILENAME_LEN_BYTES] = {'\0'};
    generate_fn(kv_key, uid, pid);

    uint32_t kv_create_flags = 0;
    if (create_flags & PSA_ITS_WRITE_ONCE_FLAG) {
        kv_create_flags = KVStore::WRITE_ONCE_FLAG;
    }

    int kvstore_status = kvstore->set((const char *)kv_key, p_data, data_length, kv_create_flags);

    psa_its_status_t status = PSA_ITS_SUCCESS;
    if (kvstore_status != 0) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    return status;
}

psa_its_status_t psa_its_get_impl(uint32_t pid, uint32_t uid, uint32_t data_offset, uint32_t data_length, void *p_data)
{
    KVStore *kvstore = get_kvstore_instance();
    if (!kvstore) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    // Generate KVStore key
    uint8_t kv_key[PSA_ITS_FILENAME_LEN_BYTES] = {'\0'};
    generate_fn(kv_key, uid, pid);

    KVStore::info_t kv_info;
    int kvstore_status = kvstore->get_info((const char *)kv_key, &kv_info);
    if (kvstore_status) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    if (data_offset + data_length > kv_info.size) {
        return PSA_ITS_ERROR_INCORRECT_SIZE;
    }

    size_t actual_size = 0;
    kvstore_status = kvstore->get((const char *)kv_key, p_data, data_length, &actual_size, data_offset);

    psa_its_status_t status = PSA_ITS_SUCCESS;
    if (kvstore_status == 0) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    return status;
}

psa_its_status_t psa_its_get_info_impl(uint32_t pid, uint32_t uid, struct psa_its_info_t *p_info)
{
    psa_its_status_t status = PSA_ITS_SUCCESS;

    KVStore *kvstore = get_kvstore_instance();
    if (!kvstore) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    // Generate KVStore key
    uint8_t kv_key[PSA_ITS_FILENAME_LEN_BYTES] = {'\0'};
    generate_fn(kv_key, uid, pid);

    KVStore::info_t kv_info;
    int kvstore_status = kvstore->get_info((const char *)kv_key, &kv_info);

    if (kvstore_status == 0) {
        p_info->flags = 0;
        if (kv_info.flags & KVStore::WRITE_ONCE_FLAG) {
            p_info->flags |= PSA_ITS_WRITE_ONCE_FLAG;
        }
        p_info->size = (uint32_t)(kv_info.size);   // kv_info.size is of type size_t
    } else {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    return status;
}

psa_its_status_t psa_its_remove_impl(uint32_t pid, uint32_t uid)
{
    KVStore *kvstore = get_kvstore_instance();
    if (!kvstore) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    // Generate KVStore key
    uint8_t kv_key[PSA_ITS_FILENAME_LEN_BYTES] = {'\0'};
    generate_fn(kv_key, uid, pid);

    int kvstore_status = kvstore->remove((const char *)kv_key);

    if (kvstore_status) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    return PSA_ITS_SUCCESS;
}

#ifdef   __cplusplus
}
#endif
