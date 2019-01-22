#!/bin/bash

set -e

SCRIPT_DIR=$(dirname "${BASH_SOURCE[0]}")
MBED_OS_ROOT=$(dirname "$SCRIPT_DIR")
TOOLCHAIN=$1
shift

mbed compile -t $TOOLCHAIN -m ARM_MUSCA_A1_S \
    --source $MBED_OS_ROOT/components/TARGET_PSA \
    --source $MBED_OS_ROOT/drivers \
    --source $MBED_OS_ROOT/platform \
    --source $MBED_OS_ROOT/cmsis \
    --source $MBED_OS_ROOT/hal \
    --source $MBED_OS_ROOT/targets \
    --source $MBED_OS_ROOT/features/storage/blockdevice \
    --source $MBED_OS_ROOT/components/storage/blockdevice \
    --source $MBED_OS_ROOT/features/storage/kvstore/tdbstore \
    --source $MBED_OS_ROOT/features/storage/kvstore/include \
    --source $MBED_OS_ROOT/features/storage/kvstore/conf/tdb_internal \
    --source $MBED_OS_ROOT/features/mbedtls \
    $*

cp BUILD/ARM_MUSCA_A1_S/$TOOLCHAIN/TARGET_PSA.bin targets/TARGET_ARM_SSG/TARGET_MUSCA_A1/device/COMPONENT_NSPE/tfm.bin
cp BUILD/ARM_MUSCA_A1_S/$TOOLCHAIN/cmse_lib.o targets/TARGET_ARM_SSG/TARGET_MUSCA_A1/device/COMPONENT_NSPE/cmse_lib.o
