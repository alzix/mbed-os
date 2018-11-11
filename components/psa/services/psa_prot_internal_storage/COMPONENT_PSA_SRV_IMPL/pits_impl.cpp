#include <string.h>
#include "psa_prot_internal_storage.h"
#include "pits_impl.h"
#include "nvstore.h"

#ifdef   __cplusplus
extern "C"
{
#endif

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

#define PSA_ITS_FILENAME_LEN_BYTES      14          // uid: 6; delimiter: 1; pid: 6; str terminator: 1

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

static psa_its_status_t get_key_header(uint32_t uid, psa_its_create_flags_t *flags, uint32_t *size)
{
    uint16_t actual_size = 0;
    psa_its_status_t res = PSA_ITS_SUCCESS;
    NVStore &nvstore = NVStore::get_instance();
    int status = nvstore.get_item_size(uid, actual_size);
    if (status == NVSTORE_SUCCESS)
    {
        void *data = malloc(actual_size);
        if (data == NULL) {
            return PSA_ITS_ERROR_STORAGE_FAILURE;
        }

        status = nvstore.get(uid, actual_size, data, actual_size);
        if (status == NVSTORE_SUCCESS) {
            if (flags != NULL) {
                *flags = *(psa_its_create_flags_t *)PITS_FLAGS_PTR(data);
            }
            if (size != NULL) {
                *size = actual_size - PITS_HEADER_SIZE;
            }
        }

        memset(data, 0, actual_size);
        free(data);
    } else if (status == NVSTORE_NOT_FOUND) {
        res = PSA_ITS_ERROR_KEY_NOT_FOUND;
    } else {
        res = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    return res;
}

 void add_headers_to_record(void *record, psa_its_create_flags_t create_flags)
{
    memcpy(PITS_FLAGS_PTR(record), &create_flags, sizeof(create_flags));
}

psa_its_status_t psa_its_set_impl(uint32_t pid, uint32_t uid, uint32_t data_length, const void *p_data, psa_its_create_flags_t create_flags)
{
    psa_its_status_t status = PSA_ITS_SUCCESS;

    uint8_t tdb_filename[PSA_ITS_FILENAME_LEN_BYTES] = {'\0'};
    generate_fn(tdb_filename, uid, pid);


    NVStore &nvstore = NVStore::get_instance();
    int nvstore_status = 0;
    if (create_flags & PSA_ITS_WRITE_ONCE_FLAG) {
        nvstore_status = nvstore.set_once(uid, data_length, p_data);
    } else {
        nvstore_status = nvstore.set(uid, data_length, p_data);
    }

    switch(nvstore_status) {
        case NVSTORE_SUCCESS:
            status = PSA_ITS_SUCCESS;
            break;
        case NVSTORE_WRITE_ERROR:
            status = PSA_ITS_ERROR_STORAGE_FAILURE;
            break;
        case NVSTORE_BAD_VALUE:
            status = PSA_ITS_ERROR_BAD_POINTER;
            break;
        case NVSTORE_FLASH_AREA_TOO_SMALL:
            status = PSA_ITS_ERROR_INSUFFICIENT_SPACE;
            break;
        case NVSTORE_ALREADY_EXISTS:
            status = PSA_ITS_ERROR_FLAGS_SET_AFTER_CREATE;
            break;
        default:
            break;
    }

    return status;
}

psa_its_status_t psa_its_get_impl(uint32_t pid, uint32_t uid, uint32_t data_offset, uint32_t data_length, void *p_data)
{
    NVStore &nvstore = NVStore::get_instance();
    uint16_t data_size = 0;
    uint8_t tdb_filename[PSA_ITS_FILENAME_LEN_BYTES] = {'\0'};

    generate_fn(tdb_filename, uid, pid);

    int nvstore_status = nvstore.get_item_size(uid, data_size);
    switch(nvstore_status) {
        case NVSTORE_SUCCESS:
            break;
        case NVSTORE_NOT_FOUND:
            return PSA_ITS_ERROR_KEY_NOT_FOUND;
            break;
        case NVSTORE_READ_ERROR:
        case NVSTORE_DATA_CORRUPT:
            return PSA_ITS_ERROR_STORAGE_FAILURE;
            break;
        case NVSTORE_BAD_VALUE:
            return PSA_ITS_ERROR_INVALID_KEY;
        default:
            break;
    }

    if (PITS_HEADER_SIZE + data_offset > data_size) {
        return PSA_PS_ERROR_OFFSET_INVALID;
    }

    if (PITS_HEADER_SIZE + data_offset + data_length > data_size) {
        return PSA_ITS_ERROR_INCORRECT_SIZE;
    }

    uint8_t *record = (uint8_t *)malloc(data_size);
    if (record == NULL) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    nvstore_status = nvstore.get(uid, data_size, record, data_size);
    switch(nvstore_status) {
        case NVSTORE_SUCCESS:
            break;
        case NVSTORE_NOT_FOUND:
        case NVSTORE_READ_ERROR:
        case NVSTORE_DATA_CORRUPT:
        case NVSTORE_BUFF_TOO_SMALL:
        case NVSTORE_BAD_VALUE:
            free(record);
            return PSA_ITS_ERROR_STORAGE_FAILURE;
            break;
        default:
            break;
    }

    memcpy(p_data, PITS_DATA_PTR_AT_OFFSET(record, data_offset), data_length);
    free(record);

    return PSA_ITS_SUCCESS;
}

psa_its_status_t psa_its_get_info_impl(uint32_t pid, uint32_t uid, struct psa_its_info_t *p_info)
{
    psa_its_create_flags_t flags;
    uint32_t size;
    uint8_t tdb_filename[PSA_ITS_FILENAME_LEN_BYTES] = {'\0'};

    generate_fn(tdb_filename, uid, pid);

    psa_its_status_t status = get_key_header(uid, &flags, &size);
    if (status != PSA_ITS_SUCCESS) {
        return status;
    }

    p_info->size = size;
    p_info->flags = flags;

    return PSA_ITS_SUCCESS;
}

psa_its_status_t psa_its_remove_impl(uint32_t pid, uint32_t uid)
{
    psa_its_create_flags_t flags = 0;
    NVStore &nvstore = NVStore::get_instance();

    uint8_t tdb_filename[PSA_ITS_FILENAME_LEN_BYTES] = {'\0'};

    generate_fn(tdb_filename, uid, pid);

    psa_its_status_t status = get_key_header(uid, &flags, NULL);
    if (status != PSA_ITS_SUCCESS) {
        return status;
    }

    if (nvstore.remove(uid) != NVSTORE_SUCCESS) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    return PSA_ITS_SUCCESS;
}

#ifdef   __cplusplus
}
#endif
