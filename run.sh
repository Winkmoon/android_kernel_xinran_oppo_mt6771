#工具链 clang-11
export PATH=/usr/lib/llvm-11/bin:$PATH

#验证版本
clang --version | head -1
ld.lld --version | head -1
git -v

# 禁用 Git 信息
touch .scmversion

# 确保 EXTRAVERSION 为空
sed -i 's/^EXTRAVERSION =.*/EXTRAVERSION =/' Makefile

#用户主机名
export KBUILD_BUILD_USER=ZakoBai♡
export KBUILD_BUILD_HOST=XinRan

MAKE_CMD="make LLVM=1 LLVM_IAS=0 ARCH=arm64 \
  CLANG_TRIPLE=aarch64-linux-android- \
  CC=clang \
  LD=ld.lld \
  AR=llvm-ar \
  NM=llvm-nm \
  OBJCOPY=llvm-objcopy \
  OBJDUMP=llvm-objdump \
  HOSTCC=clang \
  HOSTCXX=clang++ \
  PYTHON=python3 O=out"

#生成配置文件
$MAKE_CMD oppo6771_defconfig

#构建内核
$MAKE_CMD KCFLAGS=-Wno-error -j$(nproc --all)

#构建连接模块 (WiFi/BT/GPS/FM)
bash build_modules.sh
