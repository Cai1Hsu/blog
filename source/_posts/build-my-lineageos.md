---
title: '为 Xiaomi Civi 3 移植 LineageOS'
date: 2023-10-27 11:36:05
tags: ['Linux', 'Android', 'LineageOS', 'ROM']
---

# THIS BLOG HAS NOT FINISHED AND MAY CONTAIN SOME MISLEADING INFOMATION

## 编译环境
```
                   -`                    caiyi@archlinux 
                  .o+`                   --------------- 
                 `ooo/                   OS: Arch Linux x86_64 
                `+oooo:                  Host: 83AQ XiaoXinPro 16 IRH8 
               `+oooooo:                 Kernel: 6.5.8-zen1-1-zen 
               -+oooooo+:                Uptime: 2 hours, 53 mins 
             `/:-:++oooo+:               Packages: 1168 (pacman) 
            `/++++/+++++++:              Shell: zsh 5.9 
           `/++++++++++++++:             Resolution: 2560x1600 
          `/+++ooooooooooooo/`           DE: Plasma 5.27.8 
         ./ooosssso++osssssso+`          WM: KWin 
        .oossssso-````/ossssss+`         Theme: [Plasma], Breeze [GTK2/3] 
       -osssssso.      :ssssssso.        Icons: [Plasma], breeze-dark [GTK2/3] 
      :osssssss/        osssso+++.       Terminal: konsole 
     /ossssssss/        +ssssooo/-       Terminal Font: Hack 14 
   `/ossssso+/:-        -:/+osssso+-     CPU: 13th Gen Intel i5-13500H (16) @ 4.700GHz 
  `+sso+:-`                 `.-/+oso:    GPU: NVIDIA GeForce RTX 3050 6GB Laptop GPU 
 `++:.                           `-/+/   GPU: Intel Raptor Lake-P [Iris Xe Graphics] 
 .`                                 `/   Memory: 15723MiB 
```

## Preparation

### Setup `repo`
```bash
curl https://mirrors.tuna.tsinghua.edu.cn/git/git-repo -o ~/bin/repo
chmod +x ~/bin/repo
```

#### 更新
repo的运行过程中会尝试访问官方的git源更新自己，如果想使用tuna的镜像源进行更新，可以将如下内容复制到你的~/.bashrc里
```bash
export REPO_URL='https://mirrors.tuna.tsinghua.edu.cn/git/git-repo'
```

### Install dependencies
```bash
sudo pacman -S llvm bc bison base-devel ccache curl flex gcc-multilib git git-lfs gnupg gperf imagemagick lib32-readline lib32-zlib elfutils lz4 sdl openssl libxml2 lzop pngcrush rsync schedtool squashfs-tools libxslt zip zlib lib32-ncurses wxgtk3 ncurses android-tools
```

### Configure git
Given that repo requires you to identify yourself to sync Android, run the following commands to configure your git identity:
```bash
git config --global user.email "you@example.com"
git config --global user.name "Your Name"
```
Due to their size, some repos are configured for lfs or Large File Storage. To make sure your distribution is prepared for this, run:
```bash
git lfs install
```

### Turn on caching to speed up build
如果您想加速后续构建，请使用ccache，方法如下：
```bash
export USE_CCACHE=1
export CCACHE_EXEC=/usr/bin/ccache
```

并将该行添加到您的 ~/.bashrc 文件中。然后，通过输入以下命令来指定您希望ccache使用的最大磁盘空间：

```bash
ccache -M 50G
```
其中50G对应于50GB的缓存。这需要运行一次。25GB-100GB的缓存空间将显着提高构建速度（例如，典型的1小时构建时间可以缩短到20分钟）。如果您只为一个设备构建，则25GB-50GB就足够了。如果您计划为多个不共享相同内核源的设备构建，则应瞄准75GB-100GB。这些空间将永久占用您的驱动器，因此请考虑这一点。

您还可以启用可选的ccache压缩。虽然这可能会导致轻微的性能下降，但它可以增加适合缓存的文件数量。要启用它，请运行：
```bash
ccache -o compression=true
```
NOTE: If compression is enabled, the ccache size can be lower (aim for approximately 20GB for one device).

### 打开 ZRAM
考虑到我的机器内存较小，需要打开 ZRAM 来满足编译的要求。

1. 首先，通过 sysfs 禁用 zswap。
```bash
sudo echo 0 > /sys/module/zswap/parameters/enabled
```

2. 接着，配置一个采用 zstd 压缩、32GiB 容量且优先度较高的 zram 设备 (仅对当前会话生效)：

```bash
sudo modprobe zram
sudo zramctl /dev/zram0 --algorithm zstd --size 32G
sudo mkswap -U clear /dev/zram0
sudo swapon --priority 100 /dev/zram0
```

- 如需禁用，可以重启或执行以下命令：
```bash
sudo swapoff /dev/zram0
sudo modprobe -r zram
sudo echo 1 > /sys/module/zswap/parameters/enabled
```

### 初始化代码仓库
进入工作目录，执行
```bash
mkdir lineageOS && cd lineageOS
repo init -u https://github.com/LineageOS/android.git -b lineage-21.0
```
### 拉取代码
```bash
repo sync
```
LineageOS清单包括一个合理的默认配置，我们强烈建议您使用它（即不要添加任何选项进行同步）。作为参考，我们的默认值为-j 4和-c。-j 4部分意味着有四个同时的线程/连接。如果您在同步时遇到问题，可以将其降低到-j 3或-j 2。另一方面，-c使repo仅拉取当前分支，而不是GitHub上可用的所有分支。

然后就静静等待吧。

### 为该型号移植

#### 为设备添加配置

##### 配置环境

在仓库根目录执行
```bash
source build/envsetup.sh
```

##### 生成设备树
使用`breakfast <code_name>`，对于 Xiaomi Civi 3 执行
```
breakfast yuechu
```

##### 修改

##### 生成设备(device)配置文件
```bash
lunch lineage_yuechu-userdebug
```

##### 克隆内核(kernel)源代码
```bash
cd kernel
# 如果有请忽略
mkdir xiaomi
git clone https:/   /github.com/MiCode/Xiaomi_Kernel_OpenSource -b yuechu-t-oss yuechu
```

##### 生成内核(kernel)配置
```bash
lunch lineage_yuechu-userdebug
mka kernel_config
```

##### 编译内核(kernel)
```bash
cd <root_of_repo>
mka bootimage
```

##### 配置驱动(vendor)


## Referrence
[为Xiaomi 4C编译LineageOS 18.1（Android 11）](https://zhuanlan.zhihu.com/p/353719573)

[Android 13 lineageOS-20.0 下载编译并刷入小米8](https://zhuanlan.zhihu.com/p/640695411)

[自己动手编译Android(LineageOS)源码](https://www.cnblogs.com/luoyesiqiu/p/10701419.html)

[ROM 编译指南及踩坑](https://zhuanlan.zhihu.com/p/555521275)