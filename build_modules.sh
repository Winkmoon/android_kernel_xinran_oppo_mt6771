#!/bin/bash
# Build external connectivity modules (GPS/BT/FM) for MT6771
# WiFi drivers are now built-in, no longer need external compilation
set -e
SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
cd "$SCRIPT_DIR"
export PATH=/usr/lib/llvm-11/bin:$PATH
touch .scmversion

MAKE_BASE="make LLVM=1 LLVM_IAS=0 ARCH=arm64"
MAKE_BASE="$MAKE_BASE CLANG_TRIPLE=aarch64-linux-android-"
MAKE_BASE="$MAKE_BASE CC=clang LD=ld.lld AR=llvm-ar NM=llvm-nm"
MAKE_BASE="$MAKE_BASE OBJCOPY=llvm-objcopy OBJDUMP=llvm-objdump"
MAKE_BASE="$MAKE_BASE HOSTCC=clang HOSTCXX=clang++ PYTHON=python3 O=out"
MAKE_BASE="$MAKE_BASE KCFLAGS=-Wno-error"

TOP_DIR=$(dirname "$SCRIPT_DIR")
MODULES_BASE="$TOP_DIR/vendor/mediatek/kernel_modules/connectivity"
BASE_OPTS="TARGET_BUILD_VARIANT=userdebug TARGET_BOARD_PLATFORM=mt6771 MTK_PLATFORM=mt6771"

echo "=== Building external connectivity modules ==="
echo "WiFi is built-in, skipping wmt_drv/wmt_chrdev_wifi/wlan_drv_gen4m"
echo ""

[ -d "$MODULES_BASE/gps" ] && echo "[1/3] gps_drv.ko" && eval $MAKE_BASE M=$MODULES_BASE/gps $BASE_OPTS modules
[ -d "$MODULES_BASE/bt/mt66xx/legacy" ] && echo "[2/3] bt_drv.ko" && eval $MAKE_BASE M=$MODULES_BASE/bt/mt66xx/legacy $BASE_OPTS modules
AUTOCONF_H=$(pwd)/out/include/generated/autoconf.h
[ -d "$MODULES_BASE/fmradio" ] && echo "[3/3] fmradio_drv.ko" && eval $MAKE_BASE M=$MODULES_BASE/fmradio AUTOCONF_H=$AUTOCONF_H CFG_FM_CHIP_ID=6771 CFG_FM_CHIP=mt6631 $BASE_OPTS modules

echo ""
echo "=== Done ==="
find $MODULES_BASE -name "*.ko" -exec ls -la {} \; 2>/dev/null
