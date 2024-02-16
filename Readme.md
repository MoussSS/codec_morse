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
(gdb) break main
(gdb) next # Debug line per line
(gdb) continue
```
## Build test suite
```bash
$ mkdir -p build/Test && cd build/Test
$ cmake ../.. -DCMAKE_BUILD_TYPE=Test
$ cmake --build .
$ ctest -V
```
