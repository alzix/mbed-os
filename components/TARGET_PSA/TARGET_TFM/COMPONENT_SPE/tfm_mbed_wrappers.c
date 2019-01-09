/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(TOOLCHAIN_GCC)

void *__real__malloc_r(struct _reent *r, size_t size);
void *__real__memalign_r(struct _reent *r, size_t alignment, size_t bytes);
void *__real__realloc_r(struct _reent *r, void *ptr, size_t size);
void __real__free_r(struct _reent *r, void *ptr);
void *__real__calloc_r(struct _reent *r, size_t nmemb, size_t size);
void _exit(int return_code);
int __real_main(void);

void *__wrap__malloc_r(struct _reent *r, size_t size)
{
    return __real__malloc_r(r, size);
}

void *__wrap__realloc_r(struct _reent *r, void *ptr, size_t size)
{
    return __real__realloc_r(r, ptr, size);
}

void __wrap__free_r(struct _reent *r, void *ptr)
{
    __real__free_r(r, ptr);
}

void *__wrap__calloc_r(struct _reent *r, size_t nmemb, size_t size)
{
    return __real__calloc_r(r, nmemb, size);
}

void *__wrap__memalign_r(struct _reent *r, size_t alignment, size_t bytes)
{
    return __real__memalign_r(r, alignment, bytes);
}

void __wrap_exit(int return_code)
{
    _exit(return_code);
}

int __wrap_atexit(void (*func)())
{
    return 1;
}

int __wrap_main(void)
{
    return __real_main();
}

#elif defined(TOOLCHAIN_ARM) || defined(__ICCARM__)

#if defined(TOOLCHAIN_ARM)
#define SUPER_MALLOC    $Super$$malloc
#define SUB_MALLOC      $Sub$$malloc
#define SUPER_REALLOC   $Super$$realloc
#define SUB_REALLOC     $Sub$$realloc
#define SUPER_CALLOC    $Super$$calloc
#define SUB_CALLOC      $Sub$$calloc
#define SUPER_FREE      $Super$$free
#define SUB_FREE        $Sub$$free
#elif defined(__ICCARM__)
#define SUPER_MALLOC    $Super$$__iar_dlmalloc
#define SUB_MALLOC      $Sub$$__iar_dlmalloc
#define SUPER_REALLOC   $Super$$__iar_dlrealloc
#define SUB_REALLOC     $Sub$$__iar_dlrealloc
#define SUPER_CALLOC    $Super$$__iar_dlcalloc
#define SUB_CALLOC      $Sub$$__iar_dlcalloc
#define SUPER_FREE      $Super$$__iar_dlfree
#define SUB_FREE        $Sub$$__iar_dlfree
#endif

#endif // #if defined(TOOLCHAIN_GCC)
