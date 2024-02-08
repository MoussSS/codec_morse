# Nucleo-F446RE-From-Scratch
## Prerequisites
For Ubuntu:22.04 version :
```bash
$ sudo apt install gcc-arm-none-eabi gdb-multiarch openocd
```
Renode installation :
Download the package from Github repository.
```bash
mkdir renode_portable
tar xf renode-*.linux-portable.tar.gz -C renode --strip-components=1
cd renode_portable
export PATH="`pwd`:$PATH"
```
## Build the firmware
```bash
mkdir build
# Compile object file from source code
arm-none-eabi-gcc -Wall -c startup.c -o build/startup.o -mcpu=cortex-m4 -mthumb -Og -g
arm-none-eabi-gcc -Wall -c main.c -o build/main.o -mcpu=cortex-m4 -mthumb -Og -g
# Build firmware file from object with linker file
arm-none-eabi-gcc -T nucleo-f446re.ld -nostdlib build/startup.o build/main.o -o build/firmware.elf
# Display assembler code
arm-none-eabi-objdump -D build/firmware.elf
```
## Flash the firmware
```bash
$ openocd -f nucleo-f446re.cfg -c "setup" -c "program_release build/firmware.elf"
```
## Open a debug session
```bash
$ openocd -f nucleo-f446re.cfg -c "setup" -c "program_debug build/firmware.elf"
```
```bash
$ gdb-multiarch --tui build/firmware.elf
(gdb) target extended-remote localhost:3333
(gdb) monitor reset halt
(gdb) load
(gdb) break main
(gdb) next # Debug line per line
```
## Open a debug session with Renode
```bash
renode nucleo-f446re.resc
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
(gdb) file build/firmware.elf
(gdb) load
(gdb) monitor start
(gdb) monitor halt
(gdb) continue
```

## Resources
- [Baremetal from zero to blink](https://www.linuxembedded.fr/2021/02/bare-metal-from-zero-to-blink)
- [From Zero to main(): Bare metal C](https://interrupt.memfault.com/blog/zero-to-main-1)
- [From Zero to main(): Demystifying Firmware Linker Scripts](https://interrupt.memfault.com/blog/how-to-write-linker-scripts-for-firmware)
- [From Zero to main(): How to Write a Bootloader from Scratch](https://interrupt.memfault.com/blog/how-to-write-a-bootloader-from-scratch)
- [From Zero to main(): Bootstrapping libc with Newlib](https://interrupt.memfault.com/blog/boostrapping-libc-with-newlib#from-zero-to-main-bootstrapping-libc-with-newlib)
- [From Zero to main(): Bare metal Rust](https://interrupt.memfault.com/blog/zero-to-main-rust-1)
- [STM32 Startup script in C++](https://medium.com/@csrohit/stm32-startup-script-in-c-b01e47c55179)