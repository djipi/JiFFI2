# JiFFI2
Atari Jaguar file format interchanger.

JiFFI2 is a program to convert between the various homebrew formats of the Atari Jaguar.

## Usage
The command line option -? will display the application usage and arguments list.<br>
The application GUI is accessible by launching the application without arguments.

## Features
Conversion between one to all supported formats.<br>
Command line and GUI support.<br>
6MB pad rom support.<br>
Debug information removal for COFF and ELF format.
### Additional features specific to GUI
jcp and jcp2 executable detection.<br>
lo_inp executable detection.<br>
Configuration file in XML format.

## Platforms
Linux<br>
MSYS2/MinGW<br>
Windows 10 64 bits

## Supported formats
BIN/BJL<br>
COFF/ABS<br>
ELF<br>
J64/ROM<br>
Jaguar Server (v2 and v3)

## Documentations
Please refer to the [Wiki](https://github.com/djipi/JiFFI2/wiki) page.

## Projects
CMake<br>
Visual Studio 2022
### Requirements
[CRC32](https://github.com/djipi/JiFFI2/wiki/Librairies#crc32)<br>
[libelf](https://github.com/djipi/JiFFI2/wiki/Librairies#elf)
### Additional requirements specific to GUI
[Qt5](https://github.com/djipi/JiFFI2/wiki/Librairies#qt)<br>
[tinyxml2](https://github.com/djipi/JiFFI2/wiki/Librairies#xml)

## Revision
This version has been converted to C/C++ from JiFFI v1.43 written in GFA Basic.
