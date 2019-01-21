/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __TFM_PARTITION_LIST_INC__
#define __TFM_PARTITION_LIST_INC__

#ifdef TFM_PSA_API
/******** PSA_ITS ********/
PARTITION_DECLARE(PSA_ITS, SPM_PART_FLAG_IPC
    , "PSA-ROT", 0x0000000A, NORMAL);
PARTITION_ADD_INIT_FUNC(PSA_ITS, its_entry);
#endif /* TFM_PSA_API */

#ifdef TFM_PSA_API
/******** CRYPTO_SRV ********/
PARTITION_DECLARE(CRYPTO_SRV, SPM_PART_FLAG_IPC
    , "PSA-ROT", 0x00000023, NORMAL);
PARTITION_ADD_INIT_FUNC(CRYPTO_SRV, crypto_main);
#endif /* TFM_PSA_API */

/******** TFM_SP_PLATFORM ********/
PARTITION_DECLARE(TFM_SP_PLATFORM, 0
    , "PSA-ROT", 0x00000102, NORMAL);
PARTITION_ADD_INIT_FUNC(TFM_SP_PLATFORM, platform_sp_init);

#endif /* __TFM_PARTITION_LIST_INC__ */
