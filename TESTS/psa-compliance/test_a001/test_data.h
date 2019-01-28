/** @file
 * Copyright (c) 2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "val_attestation.h"

typedef struct {
    char                    test_desc[100];
    size_t                  challenge_size;
    size_t                  token_size;
    psa_status_t            expected_status;
} test_data;


static test_data check1[] = {
{"Test psa_initial_attestation_get_token\n",
 CHALLENGE_SIZE, PSA_INITIAL_ATTEST_TOKEN_SIZE, PSA_SUCCESS
},

{"Test psa_initial_attestation_get_token with zero challenge size\n",
 0, PSA_INITIAL_ATTEST_TOKEN_SIZE, PSA_SUCCESS
},

{"Test psa_initial_attestation_get_token with large challenge size\n",
 MAX_CHALLENGE_SIZE+1, PSA_INITIAL_ATTEST_TOKEN_SIZE, PSA_ATTEST_ERR_INVALID_INPUT
},

{"Test psa_initial_attestation_get_token with small token buffer size\n",
 CHALLENGE_SIZE, TOO_SMALL_TOKEN_BUFFER, PSA_ATTEST_ERR_TOKEN_BUFFER_OVERFLOW
},
};
