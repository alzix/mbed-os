#include "val_greentea.h"
#include "val_interfaces.h"
#include "val_crypto.h"
#include "val_internal_trusted_storage.h"
#include "val_protected_storage.h"
#include "val_attestation.h"

/*VAL APIs to be used by test */
const val_api_t val_api = {
    .print                     = mbed_val_print,
    .set_status                = mbed_val_set_status,
    .get_status                = mbed_val_get_status,
    .test_init                 = mbed_val_test_init,
    .test_exit                 = mbed_val_test_exit,
    .err_check_set             = NULL,
    .target_get_config         = NULL,
    .execute_non_secure_tests  = mbed_val_execute_non_secure_tests,
    .switch_to_secure_client   = NULL,
    .execute_secure_test_func  = mbed_val_execute_secure_test_func,
    .get_secure_test_result    = mbed_val_get_secure_test_result,
    .ipc_connect               = mbed_val_ipc_connect,
    .ipc_call                  = mbed_val_ipc_call,
    .ipc_close                 = mbed_val_ipc_close,
    .nvmem_read                = NULL,
    .nvmem_write               = NULL,
    .wd_timer_init             = NULL,
    .wd_timer_enable           = NULL,
    .wd_timer_disable          = NULL,
    .wd_reprogram_timer        = mbed_val_wd_reprogram_timer,
    .set_boot_flag             = NULL,
    .get_boot_flag             = NULL,
    .crypto_function           = val_crypto_function,
    .its_function              = val_its_function,
    .ps_function               = val_ps_function,
    .attestation_function      = val_attestation_function,
};

const psa_api_t psa_api = {
    .framework_version     = pal_ipc_framework_version,
    .version               = pal_ipc_version,
    .connect               = pal_ipc_connect,
    .call                  = pal_ipc_call,
    .close                 = pal_ipc_close,
};
