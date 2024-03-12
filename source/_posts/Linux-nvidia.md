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

## 添加Nvidia内核模块

打开`/etc/mkinitcpio.conf`
- 在`MODULES`内添加`nvidia nvidia_modeset nvidia_uvm nvidia_drm`
- 在`HOOKS`内添加`kms`，以实现early loading.

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

### 切换到独显渲染

```bash
sudo system76-power profile performance
sudo system76-power graphics nvidia
```

此时仍然不是独显直连状态，仅仅是只使用nvidia显卡渲染。

## 将 Nvidia 显卡作为默认输出
目前独显直连通过`XRandR`的`--setprovideroutputsource`实现，这意味着`Wayland`目前无法使用独显直连。[^0]

[^0]: 未考证

### 允许Nvidia-drm作为默认渲染源
检查`/etc/X11/xorg.conf.d/10-nvidia-drm-outputclass.conf`和`/usr/share/X11/xorg.conf.d/10-nvidia-drm-outputclass.conf`，其中有一个文件已经存在了类似一下的内容，这意味着它是你要修改的配置文件.

```
Section "OutputClass"
    Identifier "nvidia"
    MatchDriver "nvidia-drm"
    Driver "nvidia"
    Option "AllowEmptyInitialConfiguration"
    ModulePath "/usr/lib/nvidia/xorg"
    ModulePath "/usr/lib/xorg/modules"
EndSection
```

在`Option "AllowEmptyInitialConfiguration"`后添加一行`Option "PrimaryGPU" "yes"`。

### 配置显示源自动切换
如果此时重启，在进入DE时应该会导致黑屏。这是由于Xorg将集成显卡作为了输出源而不是独立显卡。我们需要在DE实际启动前将显示源切换成Nvidia显卡。

根据不同的DM，需要进行不同的配置。

#### SDDM

在`/usr/share/sddm/scripts/Xsetup`中添加以下内容:
```
xrandr --setprovideroutputsource modesetting NVIDIA-0
xrandr --auto
```

#### Gnome
在`/usr/share/gdm/greeter/autostart/optimus.desktop`和`/etc/xdg/autostart/optimus.desktop`中添加以下相同的内容:
```
[Desktop Entry]
Type=Application
Name=Optimus
Exec=sh -c "xrandr --setprovideroutputsource modesetting NVIDIA-0; xrandr --auto"
NoDisplay=true
X-GNOME-Autostart-Phase=DisplayServer
```

#### 不使用DM
在`~/.xinitrc`中添加以下内容：
```
xrandr --setprovideroutputsource modesetting NVIDIA-0
xrandr --auto
```

#### LightDM
参考[Arch Wiki](https://wiki.archlinux.org/title/NVIDIA_Optimus#LightDM)

## 重启
重新启动后，应该能够进入DM和桌面。在终端中执行`nvidia-smi`，应该能够发现所有的GUI进程都由Nvidia显卡渲染，并且Xorg占用数百兆内存。

### 用Hwinfo检查实际输出源
使用`sudo pacman -S hwinfo`安装`hwinfo`，然后在终端中执行`sudo hwinfo --gfxcard`。你会得到以下类似的输出：

```
15: PCI 100.0: 0300 VGA compatible controller (VGA)             
  [Created at pci.386]
  Unique ID: VCu0.4sa3PTUi1D7
  Parent ID: vSkL.auQjpvHj1k9
  SysFS ID: /devices/pci0000:00/0000:00:01.0/0000:01:00.0
  SysFS BusID: 0000:01:00.0
  Hardware Class: graphics card
  Model: "nVidia VGA compatible controller"
  Vendor: pci 0x10de "nVidia Corporation"
  Device: pci 0x25ec 
  SubVendor: pci 0x17aa "Lenovo"
  SubDevice: pci 0x3ca0 
  Revision: 0xa1
  Driver: "nvidia"
  Driver Modules: "nvidia"
  Memory Range: 0x5f000000-0x5fffffff (rw,non-prefetchable)
  Memory Range: 0x6000000000-0x61ffffffff (ro,non-prefetchable)
  Memory Range: 0x6200000000-0x6201ffffff (ro,non-prefetchable)
  I/O Ports: 0x3000-0x3fff (rw)
  IRQ: 127 (234965 events)
  Module Alias: "pci:v000010DEd000025ECsv000017AAsd00003CA0bc03sc00i00"
  Driver Info #0:
    Driver Status: nouveau is not active
    Driver Activation Cmd: "modprobe nouveau"
  Driver Info #1:
    Driver Status: nvidia_drm is active
    Driver Activation Cmd: "modprobe nvidia_drm"
  Driver Info #2:
    Driver Status: nvidia is active
    Driver Activation Cmd: "modprobe nvidia"
  Config Status: cfg=new, avail=yes, need=no, active=unknown
  Attached to: #11 (PCI bridge)

30: PCI 02.0: 0300 VGA compatible controller (VGA)
  [Created at pci.386]
  Unique ID: _Znp.bhaQ69qvGZE
  SysFS ID: /devices/pci0000:00/0000:00:02.0
  SysFS BusID: 0000:00:02.0
  Hardware Class: graphics card
  Model: "Intel VGA compatible controller"
  Vendor: pci 0x8086 "Intel Corporation"
  Device: pci 0xa7a0 
  SubVendor: pci 0x17aa "Lenovo"
  SubDevice: pci 0x3ca0 
  Revision: 0x04
  Driver: "i915"
  Driver Modules: "i915"
  Memory Range: 0x6202000000-0x6202ffffff (rw,non-prefetchable)
  Memory Range: 0x4000000000-0x400fffffff (ro,non-prefetchable)
  I/O Ports: 0x4000-0x403f (rw)
  Memory Range: 0x000c0000-0x000dffff (rw,non-prefetchable,disabled)
  IRQ: 164 (54456 events)
  Module Alias: "pci:v00008086d0000A7A0sv000017AAsd00003CA0bc03sc00i00"
  Driver Info #0:
    Driver Status: i915 is active
    Driver Activation Cmd: "modprobe i915"
  Config Status: cfg=new, avail=yes, need=no, active=unknown

Primary display adapter: #15
```

最后的`Primary display adapter: #15`说明了我的显示器目前由`15: PCI 100.0: 0300 VGA compatible controller (VGA) `驱动，也就是第一个部分的Nvidia显卡。

## 效果

运行 osu!lazer 帧率如下：
| 集成显卡 | prime-run | 独显直连 |
|----------|-----------|----------|
|350fps|700fps|>1000fps|

ps: osu!lazer限制最大帧率为1000hz