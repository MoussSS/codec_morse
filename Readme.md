# Nucleo-F446RE-From-Scratch
## Prerequisites
For Ubuntu:22.04 version :
```bash
$ sudo apt install gcc-arm-none-eabi gdb-multiarch openocd
```
## Build the firmware
```bash
mkdir build
# Compile object file from source code
arm-none-eabi-gcc -Wall -c main.c -o build/main.o -mcpu=cortex-m4 -mthumb -Og -g
# Build firmware file from object with linker file
arm-none-eabi-gcc -T nucleo-f446re.ld -nostdlib build/main.o -o build/firmware.elf
# Display assembler code
arm-none-eabi-objdump -d build/firmware.elf
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

## Resources
- [Baremetal from zero to blink](https://www.linuxembedded.fr/2021/02/bare-metal-from-zero-to-blink)
- [From Zero to main(): Bare metal C](https://interrupt.memfault.com/blog/zero-to-main-1)
- [From Zero to main(): Demystifying Firmware Linker Scripts](https://interrupt.memfault.com/blog/how-to-write-linker-scripts-for-firmware)
- [From Zero to main(): How to Write a Bootloader from Scratch](https://interrupt.memfault.com/blog/how-to-write-a-bootloader-from-scratch)
- [From Zero to main(): Bootstrapping libc with Newlib](https://interrupt.memfault.com/blog/boostrapping-libc-with-newlib#from-zero-to-main-bootstrapping-libc-with-newlib)
- [From Zero to main(): Bare metal Rust](https://interrupt.memfault.com/blog/zero-to-main-rust-1)