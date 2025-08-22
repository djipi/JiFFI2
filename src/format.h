#pragma once

// format list
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

extern int detect_file_format(void);
extern int save_file_format(void);
extern void removepad(void);
extern void padrom(FILE* file3, int extrabytes);
extern unsigned int checkpad(void);
