#include "mbed.h"
#include "rtos.h"
#include "val_interfaces.h"

#define TEST_STACK_SIZE 8192
extern val_api_t val_api;
extern psa_api_t psa_api;

extern "C" {
void test_entry_c006(val_api_t *val_api, psa_api_t *psa_api); 
}

void main_wrapper(void)
{
    test_entry_c006(&val_api, &psa_api);
}

int main(void)
{
    Thread thread(osPriorityNormal, TEST_STACK_SIZE, NULL);    
    thread.start(main_wrapper);    
    thread.join();
    return 0;
}
