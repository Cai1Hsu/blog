---
title: "Arch Linux 启用独显直连(Nvidia)"
date: 2024-03-06 11:09:28
tags: ["Linux", "Nivida"]
---

网上的大多数博客仅仅是在`hybrid`模式下仅使用独显允许所有进程，并非真正的独显直连。这种配置下独立显卡绘制完成后，会将`framebuffer`交由集成显卡然后输出至显示器，不仅延迟更高，性能也会受到总线带宽的限制。

## 环境
```
                   -`                    caiyi@archlinux 
                  .o+`                   --------------- 
                 `ooo/                   OS: Arch Linux x86_64 
                `+oooo:                  Host: 83AQ XiaoXinPro 16 IRH8 
               `+oooooo:                 Kernel: 6.7.8-zen1-1-zen 
               -+oooooo+:                Uptime: 37 mins 
             `/:-:++oooo+:               Packages: 942 (pacman) 
            `/++++/+++++++:              Shell: fish 3.7.0 
           `/++++++++++++++:             Resolution: 2560x1600 
          `/+++ooooooooooooo/`           DE: GNOME 45.4 
         ./ooosssso++osssssso+`          WM: Mutter 
        .oossssso-````/ossssss+`         WM Theme: Adwaita 
       -osssssso.      :ssssssso.        Theme: Adwaita [GTK2/3] 
      :osssssss/        osssso+++.       Icons: Adwaita [GTK2/3] 
     /ossssssss/        +ssssooo/-       Terminal: kgx 
   `/ossssso+/:-        -:/+osssso+-     CPU: 13th Gen Intel i5-13500H (16) @ 4.700GHz 
  `+sso+:-`                 `.-/+oso:    GPU: NVIDIA GeForce RTX 3050 6GB Laptop GPU 
 `++:.                           `-/+/   GPU: Intel Raptor Lake-P [Iris Xe Graphics] 
 .`                                 `/   Memory: 1457MiB / 15721MiB 
```

## 检查是否支持独显直连

终端中执行`lspci | grep VGA`，如果支持，则应该有包含`nvidia`显卡的项。

## 安装 Nvidia 专有驱动

```bash
sudo pacman -S nvidia-dkms nvidia-utils nvidia-settings nvidia-prime
```

## 启用 Nvidia drm

向内核参数中添加`nvidia_drm.modeset=1`。

### 如果使用`systemd-boot`
修改`/boot/loader/entries/`下启动项对应的`.conf`文件，在`options`开头行末尾添加`nvidia_drm.modeset=1`。

样例：
```
title   Arch Linux (linux)
linux   /vmlinuz-linux
initrd  /intel-ucode.img
initrd  /initramfs-linux.img
options root=PARTUUID=ddaffeea-5631-484e-b452-984a5b6ef1a8 zswap.enabled=0 rootflags=subvol=@ rw rootfstype=btrfs nvidia_drm.modeset=1
```

### 如果使用`grub`
修改`/etc/default/grub`文件，找到`GRUB_CMDLINE_LINUX_DEFAULT`行，末尾添加`nvidia_drm.modeset=1`。

验证是否已经启用，重启后执行`cat /sys/module/nvidia_drm/parameters/modeset`，如果有输出`Y`则表示启用成功。

## 配置 Nvidia 显卡

### 启用相关服务
为了避免`gnome`拒绝在`xorg`上运行，我们需要开启以下三个服务

```bash
sudo systemctl enable nvidia-hibernate
sudo systemctl enable nvidia-resume
sudo systemctl enable nvidia-suspend
```

### 开启显存保留

Nvidia显卡在睡眠时会回收部分显存，在重新唤醒后屏幕会出现花屏的现象。开启显存保留即可解决。在`/etc/modprobe.d/nvidia.conf`中加入以下行：
```
options nvidia "NVreg_PreserveVideoMemoryAllocations=1"
```

## 切换到独显直连

### 安装切换工具

[syetem76](https://system76.com/)的[pop-os](https://pop.system76.com/)是为数不多的自带独显驱动和管理器的发行版，并且`system76`编写了一个优秀的电源管理器`system76-power`用于控制电源配置和显卡模式。根据我的使用经验，我更倾向于使用`system76-power`而不是`optimus-manager`或其他管理器。并且`optimus-manager`只能在混合模式下使用独显，而不是真正的独显直连。

对于`Arch Linux`用户，`system76-power`已经包含在`aur`中，软件包名为`system76-power`。该工具由Rust编写，并依赖多个crates，对于中国大陆的用户来说，需要对cargo进行换源或者使用加速工具。但这些不再本文的讨论范围内。

使用yay安装:
```bash
yay -S system76-power
```

手动安装:
```bash
git clone https://aur.archlinux.org/system76-power.git
cd system76-power
makepkg -si
```

### 启用`system76-power`守护服务

```bash
#可能不存在，忽略就行
sudo rm /etc/systemd/system/multi-user.target.wants/system76-power.service
sudo systemctl enable com.system76.PowerDaemon.service
sudo systemctl start com.system76.PowerDaemon.service
```

### 切换到独显直连

```bash
sudo system76-power profile performance
sudo system76-power graphics nvidia
```

## 效果

运行 osu!lazer 帧率如下：
| 集成显卡 | prime-run | 独显直连 |
|----------|-----------|----------|
|350fps|700fps|>1000fps|

ps: osu!lazer限制最大帧率为1000hz