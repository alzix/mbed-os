# PSA Compliance test framework

PSA compliance tests are a set of tests that should allow a platform to be PSA-compliant.

The set of tests includes tests for:
* Internal trusted storage
* Crypto
* Attestation
* Protected storage

At the moment only Internal trusted storage tests are imported.

## Update process

The framework and tests are imported into the mbed-os tree using the importer.py in the tools.

To add more test cases edit `tools/importer/psa_compliance_importer.json` in the following way:

1. Add a new entry of the desired test case folder to the `folders` section of the json.
2. Run the importer.
3. Add a `main.c` file (template below).
4. If the tests require specific defines, Add them to each source file in the test case folder.

## Testcase main.c template

```
#include "val_interfaces.h"

extern const val_api_t val_api;
extern const psa_api_t psa_api;

int main(void)
{
    test_entry(&val_api, &psa_api);
    return 0;
}
```

* Please note that `test_entry` should be changed according to the test case.
