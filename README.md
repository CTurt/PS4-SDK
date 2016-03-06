# PS4 SDK
An open source PlayStation 4 SDK for writing userland applications which can be run with the [PS4-playground](https://github.com/CTurt/PS4-playground).

## Building and installing
You will need [LLVM](http://llvm.org/releases/) version 3.7 or later for compiling, and `binutils` for linking (from [MinGW](http://www.mingw.org/) if you are on Windows).

Clone this repository locally, or download and extract a ZIP of the repository [here](https://github.com/CTurt/PS4-SDK/archive/master.zip).

Then create an environment variable called, `PS4SDK` which points to the base directory of the repository, using the Unix path style; for example:

    /C/Users/Chris/Documents/GitHub/PS4-SDK

Build the libraries by running `make` on the `libPS4` and `libusbfatfs` directories.

Finally, you can build examples by running `make` on them.

## Design
At the moment, it is primarily focused on system calls.

However, using function pointers and the `RESOLVE` macro, we can call functions from other modules. For example, [the socket functions](https://github.com/CTurt/PS4-SDK/blob/master/source/network.c) from `libSceNet.sprx`.

## Support
Whilst the SDK isn't capable of most basic functionality yet, such as displaying graphics, or receiving controller input, there are workarounds: rendering to an HTML5 canvas, and using third party USB controllers, or a DS/PSP wirelessly for example.

The SDK is currently in a state where projects like an FTP server, [a mini Pong game](https://github.com/CTurt/PS4-Pong), or [GameBoy emulator](https://github.com/CTurt/Cinoop) could be developed.

## Examples
`hello` - Load `libc` module and perform `getpid` system call

`sockets` - Send a message over a TCP socket

`modules` - Load and dump a module

`threads` - Create and wait for a thread to increment a variable

`canvas` - Render to an HTML5 canvas

`camera` - Initialise the PlayStation Camera and capture 10 frames

`pad` - Read controller input (not finished)

`jit` - Setting up JIT shared memory

### Filesystem
`directories` - List the contents of a directory

`files` - Read a file's size, and its contents

### USB
`list_devices` - List information about all USB devices connected

`storage` - Read raw image from a USB flash drive

`usbfatfs` - Read files from USB formatted with FAT
