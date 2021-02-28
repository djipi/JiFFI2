
#pragma once

//#include <Windows.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include "patches.h"
#include "crc.h"
#include "format.h"
#include "coff.h"
#include "elf.h"
#include "bjl.h"
#include "jagr2.h"
#include "jagr3.h"
#include "rom.h"

// Arguments
#define __argmax 50
// Logs
//#define LOG(...) printf(...)
#define LOG(...)
// Processors
#define LITTLE_INDIAN				0
#define BIG_INDIAN					1
// Pads
#define search_pad					"JiFFI by Reboot (c) "
#define search_pad2					"JiFFI2 by Djipi 2020!"
// Errors code
#define FILE_ERROR_SUCCESS			0 // (0x0) - inspired from Windows error code
#define FILE_ERROR_ACCESS_DENIED	5 //(0x5) - inspired from Windows error code
#define FILE_ERROR_FILE_EXISTS		80 // (0x50) - inspired from Windows error code
#define FILE_ERROR_FILE_TOOBIG		0xff
#define FILE_ERROR_FILE_CANNOTREAD	0xfe
// Memory
#define SWAP32(value)				(((value & 0xff) << 24) | ((value & 0xff00) << 8) | ((value & 0xff0000) >> 8) | ((value & 0xff000000) >> 24))

typedef unsigned int BOOL;
typedef unsigned char BYTE;
#define MAX_PATH	256

typedef enum enumjcp
{
	NULLMASTER,
	JCPMASTER,
	JCP2MASTER
}enumjcp;

typedef enum
{
	jiffiver_undetected = 0,
	jiffiver_JiFFI1,
	jiffiver_JiFFI2
}
enum_jiffiver;

typedef enum
{
	format_undetected = 0,
	format_COFF,
	format_JAGR3,
	format_JAGR2,
	format_JiFFIROM1,
	format_ULS,
	format_ROMheader,
	format_ROMheaderless,
	format_JiFFIROM2,
	format_WeirdROM,
	format_ROMUnivHeader,
	format_ELF,
	format_2MB,
	format_6MB
}
enum_format;

typedef enum
{
	FF_ROM = 0x01,
	FF_ALPINE = 0x02,
	FF_BIOS = 0x04,
	FF_REQ_DSP = 0x08,
	FF_REQ_BIOS = 0x10,
	FF_NON_WORKING = 0x20,
	FF_BAD_DUMP = 0x40,
	FF_VERIFIED = 0x80
}
enum_FF;

typedef struct PatchForm
{
	int index;
	BOOL selected;
}
PatchForm;

typedef struct PatchData
{
	int title;
	char* caption;
	int Offset1;
	int NoBytes1;
	BYTE Bytes1[63];
	int Offset2;
	int NoBytes2;
	BYTE Bytes2[63];
	int Offset3;
	int NoBytes3;
	BYTE Bytes3[63];
	int Offset4;
	int NoBytes4;
	BYTE Bytes4[63];
}
PatchData;

typedef struct crctype
{
	uint32_t crc;
	char* titlename;
	long flags;
}
crctype;

typedef struct chk
{
	BOOL Enabled;
	BOOL Value;
}
chk;

extern PatchData Patches[];
extern PatchForm PatchOptions[];
extern crctype titles[];
extern char* imagefinaladr;
extern unsigned int NumTitles;
extern unsigned int NumPatches, PatchesForCurrentTitle;
extern char inp[], out[], name[];
extern unsigned int loadadr, runadr;
extern chk chk_coff, chk_elf, chk_ROM, chk_BJL, chk_jagr2, chk_jagr3, chk_ROMh, chk_J64;
extern chk chk_uploadROM, chk_resetskunk, chk_uploadbank2;
extern chk chk_8bitupl, chk_noswitch;
extern chk chk_overwrite;
extern chk chk_auto_output_dir;
extern chk chk_JiFFI;
extern chk cmd_uploadBJL, cmd_uploadskunk;
extern chk opt_BJL, opt_ROM;
extern int NbUseFormat;
extern unsigned int linj;
extern int detected_format;
extern enumjcp jcp_master;
extern char JCPApp[10];
extern unsigned int DebugInfoSize;
extern BOOL cli;	// , cli_run;
extern char* imageadr;
extern BOOL endianess;
extern BOOL EditorAddr;
extern char* image_orig;
extern long offset;
extern uint32_t crc;
extern int DetectedTitle, CurrentDetectedTitle;
extern char* TitleName;
extern unsigned char _acpad[];
#define pad _acpad
extern unsigned char _acpad2[];
#define pad2 _acpad2

extern void initJiFFI2(void);
extern void closeJiFFI2(void);
extern int HandleOverwriteMsg(char* PtrFilename);
extern void HandleWriteErrorMsg(char* PtrFilename);
extern int save_file_format(void);
