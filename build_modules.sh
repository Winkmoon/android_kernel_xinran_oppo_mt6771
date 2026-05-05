#!/bin/bash
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
SYMVER="out/Module.symvers"

echo "=== Building connectivity modules (WiFi/BT/GPS/FM) ==="

echo "[1/6] wmt_drv.ko (common transport)"
eval $MAKE_BASE M=$MODULES_BASE/common $BASE_OPTS modules
cp $MODULES_BASE/common/Module.symvers $SYMVER 2>/dev/null || true

echo "[2/6] wmt_chrdev_wifi.ko (WiFi char device)"
eval $MAKE_BASE M=$MODULES_BASE/wlan/adaptor $BASE_OPTS modules
cat $MODULES_BASE/wlan/adaptor/Module.symvers >> $SYMVER 2>/dev/null || true

echo "[3/6] wlan_6771_axi.ko (WiFi core)"
WLAN_OPTS="$BASE_OPTS MTK_COMBO_CHIP=CONNAC WLAN_CHIP_ID=6771 CONFIG_MTK_COMBO_WIFI_HIF=axi MTK_ANDROID_WMT=y MTK_ANDROID_EMI=y WIFI_IP_SET=1"
eval $MAKE_BASE M=$MODULES_BASE/wlan/core/gen4m $WLAN_OPTS modules

echo "[4/6] gps_drv.ko (GPS)"
eval $MAKE_BASE M=$MODULES_BASE/gps $BASE_OPTS modules

echo "[5/6] bt_drv.ko (Bluetooth)"
eval $MAKE_BASE M=$MODULES_BASE/bt/mt66xx/legacy $BASE_OPTS modules

echo "[6/6] fmradio_drv.ko (FM Radio)"
AUTOCONF_H=$(pwd)/out/include/generated/autoconf.h
FM_OPTS="$BASE_OPTS AUTOCONF_H=$AUTOCONF_H CFG_FM_CHIP_ID=6771 CFG_FM_CHIP=mt6631 CFG_BUILD_CONNAC2=false"
eval $MAKE_BASE M=$MODULES_BASE/fmradio $FM_OPTS modules

echo ""
echo "=== Build complete ==="
echo "Kernel: out/arch/arm64/boot/Image.gz-dtb"
echo "Modules:"
find $MODULES_BASE -name "*.ko" -exec echo "  {}" \; 2>/dev/null
