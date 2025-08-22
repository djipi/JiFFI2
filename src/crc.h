#pragma once

#include <stdint.h>

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

typedef struct crctype
{
	uint32_t crc;
	const char* titlename;
	long flags;
}
crctype;

extern uint32_t crc;
extern crctype titles[];

extern void initCRC(void);
