---
title: 为 Mi10 编译内核
date: 2023-11-21 08:48:59
tags:
---

## INFO
property | value |
---------|-------|
Device name| Mi10
Code name| umi
Kernel version:|4.19

## 编译环境
```bash
$ neofetch                       
                   -`                    caiyi@archlinux 
                  .o+`                   --------------- 
                 `ooo/                   OS: Arch Linux x86_64 
                `+oooo:                  Host: 83AQ XiaoXinPro 16 IRH8 
               `+oooooo:                 Kernel: 6.6.1-zen1-1-zen 
               -+oooooo+:                Uptime: 35 mins 
             `/:-:++oooo+:               Packages: 1215 (pacman) 
            `/++++/+++++++:              Shell: zsh 5.9 
           `/++++++++++++++:             Resolution: 2560x1600 
          `/+++ooooooooooooo/`           DE: Plasma 5.27.9 
         ./ooosssso++osssssso+`          WM: KWin 
        .oossssso-````/ossssss+`         Theme: [Plasma], Breeze [GTK2/3] 
       -osssssso.      :ssssssso.        Icons: [Plasma], breeze-dark [GTK2/3] 
      :osssssss/        osssso+++.       Terminal: konsole 
     /ossssssss/        +ssssooo/-       Terminal Font: Hack 14 
   `/ossssso+/:-        -:/+osssso+-     CPU: 13th Gen Intel i5-13500H (16) @ 4.700GHz 
  `+sso+:-`                 `.-/+oso:    GPU: NVIDIA GeForce RTX 3050 6GB Laptop GPU 
 `++:.                           `-/+/   GPU: Intel Raptor Lake-P [Iris Xe Graphics] 
 .`                                 `/   Memory: 2794MiB / 15723MiB 
```

## Install dependencies
### Packages
Install via `pacman`
```
sudo pacman -S llvm bc bison base-devel ccache curl flex gcc-multilib git git-lfs gnupg gperf imagemagick lib32-readline lib32-zlib elfutils lz4 sdl openssl libxml2 lzop pngcrush rsync schedtool squashfs-tools libxslt zip zlib lib32-ncurses wxgtk3 ncurses
```

### Toolchains
#### Compulsory toolchains(Clang)
1. LineageOS/clang  r416183b
```
git clone https://github.com/LineageOS/android_prebuilts_clang_kernel_linux-x86_clang-r416183b --depth=1 /path/to/your/toolchains/clang-12-lineage
```

2. Zyc-clang-12/17/18
Download in `https://github.com/ZyCromerZ/Clang/releases`

Example:
```
cd /path/to/your/toolchains/
wget https://github.com/ZyCromerZ/Clang/releases/download/18.0.0-20231121-release/Clang-18.0.0-20231121.tar.gz

tar xvf *.tar.gz -C Clang-18
```


2. GCC-aarch64 LineageOS/android_prebuilts_gcc_linux-x86_aarch64_aarch64-linux-android-4.9
```
git clone https://github.com/LineageOS/android_prebuilts_gcc_linux-x86_aarch64_aarch64-linux-android-4.9 --depth=1 /path/to/your/toolchains/gcc
```

3. GCC-arm32 LineageOS/android_prebuilts_gcc_linux-x86_arm_arm-linux-androideabi-4.9
```
git clone https://github.com/LineageOS/android_prebuilts_gcc_linux-x86_arm_arm-linux-androideabi-4.9 --depth=1 /path/to/your/toolchains/gcc32
```

## Suitable `defconfig`
一般内核源码里面的`defconfig`并不靠谱（甚至代码本身都不靠谱，即使是第三方维护的）。提取当前ROM的`defconfig`能更容易编译成功。

在内核编译时，会自动生成完整的`defconfig`并打包到内核中，这为我们编译提供了方便。

一般自动生成的`defconfig`会带有类似如下的header:
```
#
# Automatically generated file; DO NOT EDIT.
# Linux/arm64 4.19.157 Kernel Configuration
#

#
# Compiler: Android (8490178, based on r450784d) clang version 14.0.6 (https://android.googlesource.com/toolchain/llvm-project 4c603efb0cca074e9238af8b4106c30add4418f6)
#
```

### 提取方法
在一台可以在shell中提权到Root的设备上，进入shell，执行一下命令
```
# Elevate yourself to root, you may do this either by
#   $ su
# or:
#   $ adb root

# 进入工作目录
cd /sdcard/defconfig

cp /proc/config.gz ./

gunzip config.gz

# 检查生成的defconfig
cat config
```

接下来，将提取的`defconfig`转移到PC上备用。

## Prepare kernel repo
### Choose
选择内核的思路基于以下优先级


1. 同 ROM 的第三方维护内核
2. 其他类原生维护的内核（如果你也是类原生）
3. 官方的内核（别抱太大希望）

内核仓库命名方式一般为`<android/platform>_kernel_<vendor>_<cpu/code name>`
### Clone repo
本文选择 `tequilaOS` 维护的`platform_kernel_xiaomi_sm8250`，并且使用较为稳定的`taco`（对于本仓库而言）分支

由于我们不需要历史记录，因此选择shallow clone:
```
cd /path/to/your/repos
git clone https://github.com/tequilaOS/platform_kernel_xiaomi_sm8250 --depth=1 taco
```

## Prepare to compile
### Apply `defconfig`
将前面提取的`defconfig`移动到`<repo-root>/arch/arm64/configs/`下。

###  Build script
以下是我自己编写的 build script, 你需要修改里面的变量才能使用。
```bash
#! /usr/bin/bash

# ===----------------------------------------------=== #
# This script is used to build android kernel for mi10 #
# ---------------------------------------------------- #
# Kernel repo: tequilaOS/platform_kernel_xiaomi_sm8250 #
# branch: taco                                         #
# ===----------------------------------------------=== #

# CHANGE THE PATH TO YOUR TOOLCHAINS
TOOLCHAIN_PATH="/home/caiyi/toolchains/lineage"

CLANG_VERSION="17"
GCC_VERSION="4.9"

CLANG_PATH="${TOOLCHAIN_PATH}/zyc-${CLANG_VERSION}/bin"
GCC32_PATH="${TOOLCHAIN_PATH}/gcc32-${GCC_VERSION}/bin"
GCC_PATH="${TOOLCHAIN_PATH}/gcc-${GCC_VERSION}/bin"

echo "[!] setting up environment"

echo "[+] clang path: ${CLANG_PATH}"
echo "[+] gcc32 path: ${GCC32_PATH}"
echo "[+] gcc path: ${GCC_PATH}"

export PATH="${CLANG_PATH}:${GCC32_PATH}:${GCC_PATH}:${PATH}"

# 线程改成适合你自己的，你也可以直接-j${nproc}
args="-j16 \
O=out \
ARCH=arm64 \
CROSS_COMPILE=aarch64-linux-gnu- \
CROSS_COMPILE_ARM32=arm-linux-androideabi- \
CLANG_TRIPLE=aarch64-linux-gnu- \
CC=clang \
LD=ld.lld \
AR=llvm-ar \
NM=llvm-nm \
OBJCOPY=llvm-objcopy \
OBJDUMP=llvm-objdump \
READELF=llvm-readelf \
OBJSIZE=llvm-size \
STRIP=llvm-strip \
LDGOLD=aarch64-linux-gnu-ld.gold \
LLVM_AR=llvm-ar \
LLVM_DIS=llvm-dis"

echo "\e[32m[!] cleanning old configuration\e[0m"
make mrproper

echo "\e[32m[!] executing defconfig\e[0m"

# 将这个改成defconfig的文件名
# makefile会在 <repo-root>/arch/<$ARCH>/configs/里搜索
make ${args} umi_user_defconfig


echo "\e[32m[!] compiling\e[0m"
make ${args}
```

写入完记得加权：
```
chmod +x build.sh
```

## Compile
```
./build.sh
```

完整的一次编译大概耗时 3min

如果报错，你可以尝试其他版本的工具链和其他地方获取到的`defconfig`。

或者你需要手动修改内核代码，但是如果修改地方过多，你应该考虑使用其他工具链和其他`defconfig`。

对于本次使用的仓库，以下文件需要修改：
```patch
diff --git a/drivers/mmc/core/core.c b/drivers/mmc/core/core.c
index 437fb613a..d553f45bf 100644
--- a/drivers/mmc/core/core.c
+++ b/drivers/mmc/core/core.c
@@ -333,8 +333,7 @@ static bool mmc_is_valid_state_for_clk_scaling(struct mmc_host *host)
         * this mode.
         */
        if (!card || (mmc_card_mmc(card) &&
-                       (card->part_curr == EXT_CSD_PART_CONFIG_ACC_RPMB ||
-                       mmc_card_doing_bkops(card))))
+                       (card->part_curr == EXT_CSD_PART_CONFIG_ACC_RPMB)))
                return false;
 
        if (mmc_send_status(card, &status)) {
```

### 为内核添加你的签名
默认情况下，你使用的工具链，提交哈希，编译参数，whoami，hostname都会被记录在内核信息中。如果你还希望记录其他信息，你可以编辑`defconfig`。

`CONFIG_DEFAULT_HOSTNAME`用于记录默认hostname。
`CONFIG_LOCALVERSION`用于记录额外的版本信息，比如`CONFIG_LOCALVERSION="-caiyi@cai1hsu.me"`或者`CONFIG_LOCALVERSION=-RELEASE`。但是一定不能包含空格，否则可能会导致解析错误。

## 检查 build artifacts
进入 `<repo-root>/out/arch/arm64/boot` 下，检查所有的文件。

如果编译成功，你应该至少会看见`Image`，除此之外，你可能还会有`Image.gz`, `Image.gz-dtb`, `dbto.img`。

`Image`是未压缩的内核文件，如果你只有这个文件，说明你的设备不压缩内核，使用这个文件即可。

如果存在`Image.gz-dtb`则使用`Image.gz-dtb`。没有的话，如果有`Image.gz`则使用`Image.gz`。

## Flash
使用 Anykernel3 或者 magiskboot 手动修补，具体步骤请查看其他文章。
