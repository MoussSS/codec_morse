# Nucleo-F446RE-From-Scratch
## Prerequisites
Test on Ubuntu 22.04 distribution.
### Toolchain
```bash
$ sudo apt install cmake gcc-arm-none-eabi gdb-multiarch make
```
Download the package from Openocd github repository (need 0.12.0 version).
```bash
$ git clone git://git.code.sf.net/p/openocd/code openocd
$ cd openocd
$ git checkout v0.12.0
$ git submodule update --init
$ sudo apt install autoconf automake libtool libusb-1.0-0 libusb-1.0-0-dev make pkg-config texinfo
$ ./bootstrap
$ ./configure
$ sudo make install
$ openocd --version
```
### Emulator
Download the package from Renode github repository.
```bash
$ mkdir renode
$ tar xf renode-*.linux-portable.tar.gz -C renode --strip-components=1
$ cd renode
$ export PATH="`pwd`:$PATH"
$ renode --version
$ pip3 install -r tests/requirements.txt
```
### SWV Tool
Download code source from Orbuculum github repository.
```bash
$ mkdir orbuculum
$ sudo apt install libusb-1.0-0 libusb-1.0-0-dev libczmq-dev libelf-dev libcapstone-dev libsdl2-2.0-0 libsdl2-dev libncurses-dev meson ncurses-base ninja-build pkg-config
$ tar xf orbuculum-2.1.0.tar.gz -C orbuculum
$ cd orbuculum
$ meson setup build
$ ninja -C build
$ export PATH="`pwd`:$PATH"
```
## Build the firmware
### As Debug
```bash
$ mkdir -p build/Debug && cd build/Debug
$ cmake ../.. -DCMAKE_BUILD_TYPE=Debug
$ cmake --build .
```
### As Release
```bash
$ mkdir -p build/Release && cd build/Release
$ cmake ../.. -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```
## Display assemble code
```bash
$ arm-none-eabi-objdump -D bin/firmware_<Debug or Release>.elf
```
## Flash the firmware
```bash
$ openocd -f config/nucleo-f446re.cfg -c "setup" -c "program_release bin/firmware_<Debug or release>.elf"
```
## Open a debug session
```bash
$ openocd -f config/nucleo-f446re.cfg -c "setup" -c "program_debug bin/firmware_Debug.elf"
```
```bash
$ gdb-multiarch --tui build/firmware_Debug.elf
(gdb) target extended-remote localhost:3333
(gdb) monitor reset halt
(gdb) load
(gdb) monitor mww 0xE0001000 0x17ff # PC Sampling enable
(gdb) monitor itm port 0 on
(gdb) break main
(gdb) next # Debug line per line
(gdb) continue
```
To have SWV Trace with Orbuculum tool :
```bash
$ orbcat -C 16000 -s localhost:3443 -c 0,"%c"
```
To have PC Sampling information with Orbuculum tool :
```bash
$ orbtop -s localhost:3443 -e build/firmware_Debug.elf
```
## Open a debug session with Renode
```bash
$ renode config/nucleo-f446re.resc
(machine-0) logFile @/tmp/function-trace.log
(machine-0) logLevel -1
(machine-0) machine StartGdbServer 3333
(machine-0) start
(machine-0) pause
(machine-0) sysbus.cpu PC
(machine-0) sysbus.cpu LogFunctionNames true
(machine-0) machine GetTimeSourceInfo
(machine-0) machine EnableProfiler
(machine-0) quit
```
```bash
$ gdb-multiarch
(gdb) target extended-remote localhost:3333
(gdb) file build/firmware_Debug.elf
(gdb) load
(gdb) monitor start
(gdb) monitor halt
(gdb) continue
```

## Resources
### Start from scratch
- [Baremetal from zero to blink](https://www.linuxembedded.fr/2021/02/bare-metal-from-zero-to-blink)
- [From Zero to main(): Bare metal C](https://interrupt.memfault.com/blog/zero-to-main-1)
- [From Zero to main(): Demystifying Firmware Linker Scripts](https://interrupt.memfault.com/blog/how-to-write-linker-scripts-for-firmware)
- [From Zero to main(): How to Write a Bootloader from Scratch](https://interrupt.memfault.com/blog/how-to-write-a-bootloader-from-scratch)
- [From Zero to main(): Bootstrapping libc with Newlib](https://interrupt.memfault.com/blog/boostrapping-libc-with-newlib#from-zero-to-main-bootstrapping-libc-with-newlib)
- [From Zero to main(): Bare metal Rust](https://interrupt.memfault.com/blog/zero-to-main-rust-1)
- [STM32 Startup script in C++](https://medium.com/@csrohit/stm32-startup-script-in-c-b01e47c55179)
### Logic analyzer
- [Sigrok/Pulseview l'analyseur logique open source](https://linuxembedded.fr/2014/10/sigrokpulseview-lanalyseur-logique-open-source)
### SWV
- [Serial Wire Vewer (SWD + SWO) - fast & native Debugging](https://www.codeinsideout.com/blog/stm32/swv)
- [Using Single Wire Output SWO with ARM Cortex-M and Eclipse](https://mcuoneclipse.com/2016/10/17/tutorial-using-single-wire-output-swo-with-arm-cortex-m-and-eclipse)
- [Serial Wire Output](https://black-magic.org/usage/swo.html)
- [Profiling Firmware on Cortex-M](https://interrupt.memfault.com/blog/profiling-firmware-on-cortex-m)
- [Exploring printf on Cortex-M](https://interrupt.memfault.com/blog/printf-on-embedded)
- [Visualizing Real-time Data With STMViewer](https://interrupt.memfault.com/blog/stm-viewer-debug)
