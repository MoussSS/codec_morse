# Nucleo-F446RE-From-Scratch
## Prerequisites
Docker installed on your computer.
```bash
$ docker build -t nucleo-f446re-from-scratch .
$ docker run -it --rm -v $(pwd):/home/user/workspace nucleo-f446re-from-scratch
```
## Build the firmware
On the docker
```bash
$ cmake -B build/<debug or release> -DCMAKE_BUILD_TYPE=<debug or release> # Create workspace
$ cmake --build build/<debug or release> # Build
$ rm -rf build/<debug or release> # Clean with removing folder
```
## Run the firmware
On the target connected by ST-Link:
```bash
$ openocd -f config/stm32f446retx.cfg -c "setup" -c "program_release <firmware file>"
```
On the emulator:
```bash
$
```
## Open a debug session
```bash
$ openocd -f config/stm32f446retx.cfg -c "setup" -c "program_debug <firmware file>"
```
```bash
$ gdb-multiarch --tui <firmware file>
(gdb) target extended-remote localhost:3333
(gdb) monitor reset halt
(gdb) load
(gdb) break main
(gdb) continue
```
If you want use the emulator, need to launch this command:
```bash
$ renode --disable-gui config/nucleo-f446re.resc & # Launch Renode on background

$ kill %1 # Stop backgroung process
```
## Build test suite
```bash
$ cmake -B build/test -DCMAKE_BUILD_TYPE=test
$ cmake --build build/test
$ ctest -V --test-dir build/test # Run all tests with verbose output
```
## Display assembler code
```bash
$ arm-none-eabi-objdump -D <firmware file>
```