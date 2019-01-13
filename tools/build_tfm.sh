#!/bin/bash -ex

SCRIPT_DIR=$(dirname "${BASH_SOURCE[0]}")
MBED_OS_ROOT=$(dirname "$SCRIPT_DIR")
TOOLCHAIN=$1
shift

PROFILE="$(echo $* | grep -o 'profile.*' | cut -d' ' -f2)"
BUILD_DIR=$TOOLCHAIN
if [ $PROFILE = "debug" ]; then BUILD_DIR=$BUILD_DIR-${PROFILE^^}; fi

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

cp BUILD/ARM_MUSCA_A1_S/$BUILD_DIR/TARGET_PSA.bin $MBED_OS_ROOT/targets/TARGET_ARM_SSG/TARGET_MUSCA_A1/TARGET_MUSCA_A1_NS/device/tfm.bin
cp BUILD/ARM_MUSCA_A1_S/$BUILD_DIR/cmse_lib.o $MBED_OS_ROOT/targets/TARGET_ARM_SSG/TARGET_MUSCA_A1/TARGET_MUSCA_A1_NS/device/cmse_lib.o
