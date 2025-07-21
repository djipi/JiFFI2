# JiFFI2
Atari Jaguar file format interchanger.

JiFFI2 is a program to convert between the various homebrew formats of the Atari Jaguar.

## Features
Conversion between one to all supported formats.<br>
Command line and GUI support.<br>
6MB pad rom support.<br>
jcp and jcp2 executable detection.<br>
lo_inp executable detection.<br>
Debug information removal for COFF and ELF format.<br>
Configuration file in XML format.

## Usage
The application GUI is accessible by launching the application without arguments.<br>
The command line option -? will display the application usage and arguments list.

## Platforms
Linux<br>
MSYS2/MinGW<br>
Windows 10 64 bits

## Supported format
BIN/BJL<br>
COFF/ABS<br>
ELF<br>
J64/ROM<br>
Jaguar Server (v2 and v3)

## Documentations
Please refer to the Wiki page.

## Projects
CMake<br>
Visual Studio 2022
### Requirements
coff format header<br>
CRC32 function<br>
libelf<br>
Qt5<br>
tinyxml2

## Revision
This version has been converted to C/C++ from JiFFI v1.43 written in GFA Basic.
