/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __TFM_PARTITION_DEFS_INC__
#define __TFM_PARTITION_DEFS_INC__

#ifdef TFM_PSA_API
#define PSA_ITS_ID (TFM_SP_BASE + 0)
#endif /* TFM_PSA_API */

#ifdef TFM_PSA_API
#define CRYPTO_SRV_ID (TFM_SP_BASE + 1)
#endif /* TFM_PSA_API */

#define TFM_SP_PLATFORM_ID (TFM_SP_BASE + 2)

#define TFM_MAX_USER_PARTITIONS (3)

#endif /* __TFM_PARTITION_DEFS_INC__ */
