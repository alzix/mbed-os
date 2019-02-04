#include "val_interfaces.h"

extern const val_api_t val_api;
extern const psa_api_t psa_api;

int main(void)
{
    test_entry_c020(&val_api, &psa_api);
    return 0;
}
