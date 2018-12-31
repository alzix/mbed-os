/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __TFM_PARTITION_LIST_INC__
#define __TFM_PARTITION_LIST_INC__

/******** TFM_SP_PLATFORM ********/
PARTITION_DECLARE(TFM_SP_PLATFORM, SPM_PART_FLAG_SECURE | SPM_PART_FLAG_TRUSTED);
PARTITION_ADD_INIT_FUNC(TFM_SP_PLATFORM, platform_sp_init);

#endif /* __TFM_PARTITION_LIST_INC__ */
