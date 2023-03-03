# opensource toolchain for Kendryte K210

The Kendryte K210 is a system-on-chip (SoC) that integrates machine vision and machine hearing. Using TSMC's ultra-low-power 28nm advanced process with dual-core 64-bit processors for better power, performance, stability and reliability. The program strives for zero threshold development and can be deployed in the user's products in the shortest time, giving the product artificial intelligence.

The Kendryte K210 is located in the SoC of the AI and IoT markets and is a very convenient MCU.

In Chinese, Kendryte means surveying intelligence, while surveying intelligence is taken from surveying and searching. The main application field of this chip is in the field of Internet of Things, and it is developed in the field of Internet of Things. Therefore, this chip mainly provides artificial intelligence solutions, which are explored in the field.

- Machine vision
- Machine hearing
- Better low power vision processing speed and accuracy
- With convolutional artificial neural network hardware accelerator KPU, high performance convolution artificial neural network operation
- TSMC 28nm advanced process, temperature range -40 ° C to 125 ° C, stable and reliable
- Supports firmware encryption, it is difficult to crack using common methods
- Unique programmable IO array for more flexible product design
- Low voltage, lower power consumption compared to systems with the same processing power
- 3.3V/1.8V dual voltage support, no level shifting, cost saving

In short, Kendryte K210 is **RISC-V Dual Core 64bit MCU with FPU**.


# Hardware prerequiest
- A Kendryte K210 devboard
  + I use [Yahboom K210-Developer-Kit](https://www.yahboom.com/study/K210-Developer-Kit) in this tutorial.
  + Sipeed also produce a series K210 devboards.
- Debugger

<img src="misc/yahboom-k210.png />

# Toolchain overview
- Compiler : RISC-V 64bit GNU toolchain with '--with-abi=lp64f --with-arch=rv64imafc'
- SDK : [kendryte-standalone-sdk](https://github.com/kendryte/kendryte-standalone-sdk)
- Programming : [kflash.py](https://github.com/kendryte/kflash.py)
- Debugging : Kendryte riscv OpenOCD / gdb

# Compilers

Kendryte K210 is 64bit RISC-V MCU with single-precision floating point instructions, It's hard to find a general prebuilt RISC-V toolchain (such as XPack) which can support it directly. We have to build it from source or use Kendryte pre-built toolchain.

# Option 1 : Kendryte pre-built toolchain

The Kendryte pre-built toolchain can be downloaded from : https://github.com/kendryte/kendryte-gnu-toolchain. It provide:
- Binutils 2.31.51
- GDB 8.2
- GCC 8.2.0

A little bit outdated but still works.

Download and extract it as:
```
wget https://github.com/kendryte/kendryte-gnu-toolchain/releases/download/v8.2.0-20190213/kendryte-toolchain-ubuntu-amd64-8.2.0-20190213.tar.gz
sudo tar xf kendryte-toolchain-ubuntu-amd64-8.2.0-20190213.tar.gz -C /opt
```

The toolchain will located at '/opt/kendryte-toolchain'.

**NOTE :**, do NOT add '/opt/kendryte-toolchain/bin' to PATH env, since we will invoke it later by set TOOLCHAIN options in SDK. the triplet of Kendryte RISC-V toolchain is **riscv64-unknown-elf**, it may be conflict with other RISC-V toolchains, so do NOT add it to PATH env.

The prebuilt toolchain was released serveral years ago, it may requires some libraries with specific version your OS may not provided, such as 'libisl.so.19'. These libraries already put to '/opt/kendryte-toolchain/bin' dir officially. We have 2 way to tell commands to find these libraries. the triditional way is set `LD_LIBRARY_PATH` point to '/opt/kendryte-toolchain/bin', but it is not convenient for a toolchain. Here we use `patchelf` to set rpath of binraries. After toolchain extracted:

```
cd /opt/kendryte-toolchain/libexec/gcc/riscv64-unknown-elf/8.2.0
sudo patchelf --set-rpath /opt/kendryte-toolchain/bin cc1
sudo patchelf --set-rpath /opt/kendryte-toolchain/bin cc1plus
sudo patchelf --set-rpath /opt/kendryte-toolchain/bin lto1
```

# Option 2 : build toolchain from sources

Please refer to https://github.com/kendryte/kendryte-gnu-toolchain to build the toolchain yourself.


# SDK
The upstream K210 SDK is https://github.com/kendryte/kendryte-standalone-sdk, just clone it and put it somewhere, for example HOME dir.

```
git clone https://github.com/kendryte/kendryte-standalone-sdk.git
```

The sdk is organized by cmake and very easy to use, to start a project, it just need create a dir under 'src' and put your codes there.

Use 'main.c' in this repo as example, it will blink two LEDs on yahboom K210 devboard (you may need to change it to match your devboard):

```
cd kendryte-standalone-sdk
mkdir src/blink
cp <path>/main.c src/blink
mkdir build
cd build
cmake .. -DPROJ=blink -DTOOLCHAIN=/opt/kendryte-toolchain/bin
make
```
Here are some options need explained:
`-DPROJ=blink` : which project you want to build, 'blink' is the dir name we just created under 'src' dir.
`-DTOOLCHAIN=/opt/kendryte-toolchain/bin` : where to find the toolchain.

Using `make -jN` (N should be the number of processors of your computer) to accelerate the building process
Using `make VERBOSE=1` to get verbose output of make.

After built successfully, 'blink' and 'blink.bin' will be generated at 'build' dir. 'blink' is a ELF file useful for debugging, 'blink.bin' is the firmware we will program to target device later.

# Programming

Programming K210 is also very easy, most of K210 devboards has a UART chip (such as CH34x) integrated on board. After plug the devboard to PC USB port, a serial device such as '/dev/ttyUSB0' or '/dev/ttyACM0' will be created. This is the serial device we will use when programming.

The official programming tool is [kflash.py](https://github.com/kendryte/kflash.py), just clone the repo and put 'kflash.py' to PATH:

```
git clone https://github.com/kendryte/kflash.py
sudo cp kflash.py /usr/bin/kflash
sudo chmod +x /usr/bin/kflash
```

Use above 'blink' demo as example, program it to yahboom k210 devboard as:
```
kflash -p /dev/ttyUSB0 -b 2000000 blink.bin
```
The `-p /dev/ttyUSB0` can be ommited. `-b 2000000` to set a high baudrate will improve the speed of programming.

After programming finished, you will find the 2 LEDs on board blinked.

# Debugging


