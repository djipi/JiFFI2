#include "common.h"
#include "libcoff.h"
#include "time.h"


#define COFF_F	(COFF_F_RELFLG | COFF_F_EXEC)

typedef struct coffh
{
	COFF_FILHDR	fhdr;
	COFF_AOUTHDR ahdr;
	COFF_SCNHDR	shdr[3];
}
coffh;

coffh TabCoffHdr = {
	{
		0x1, 0x50,
		0x0, 0x03,
		0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0,
		((COFF_AOUTSZ & 0xff00) >> 8), (COFF_AOUTSZ & 0xff),
		((COFF_F & 0xff00) >> 8), (COFF_F & 0xff)
	},
	{
		((COFF_ZMAGIC & 0xff00) >> 8), (COFF_ZMAGIC & 0xff),		// magic number
		0x1, 0x07,													// version stamp
	},
	{
		// .text
		{
			0x2e, 0x74, 0x65, 0x78, 0x74, 0, 0, 0,		// .text
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0,
			0, 0,
			((COFF_STYP_TEXT & 0xff000000) >> 24), ((COFF_STYP_TEXT & 0x00ff0000) >> 16), ((COFF_STYP_TEXT & 0x0000ff00) >> 8), (COFF_STYP_TEXT & 0x000000ff)
		},
		// .data
		{
			0x2e, 0x64, 0x61, 0x74, 0x61, 0, 0, 0,		// .data
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0,
			0, 0,
			((COFF_STYP_DATA & 0xff000000) >> 24), ((COFF_STYP_DATA & 0x00ff0000) >> 16), ((COFF_STYP_DATA & 0x0000ff00) >> 8), (COFF_STYP_DATA & 0x000000ff)
		},
		// .bss
		{
			0x2e, 0x62, 0x73, 0x73, 0, 0, 0, 0,			// .bss
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0,
			0, 0,
			((COFF_STYP_BSS & 0xff000000) >> 24), ((COFF_STYP_BSS & 0x00ff0000) >> 16), ((COFF_STYP_BSS & 0x0000ff00) >> 8), (char)(COFF_STYP_BSS & 0x000000ff)
		}
	}
};


int create_coff(void)
{
	FILE* file2;
	int Error = FILE_ERROR_SUCCESS;

	// create output filename
	char buffer[MAX_PATH];
	sprintf_s(buffer, "%s%s_jiffi%i.coff", out, name, chk_JiFFI.Value);

	// Check if file already exists
	if (!chk_overwrite.Value && !fopen_s(&file2, buffer, "rb") && file2 && !fclose(file2))
	{
		Error = HandleOverwriteMsg(buffer);
	}

	if (!Error)
	{
		// create the coff file
		if (!fopen_s(&file2, buffer, "wb") && file2)
		{
			// set time creation
			time_t TimeStamp, Time;
			Time = time(&TimeStamp);
			*(uint32_t*)TabCoffHdr.fhdr.f_timdat = !endianess ? SWAP32(Time) : Time;
			// header initialisations
			*(uint32_t*)TabCoffHdr.ahdr.tsize = !endianess ? SWAP32(linj) : linj;
			*(uint32_t*)TabCoffHdr.ahdr.text_start = !endianess ? SWAP32(runadr) : runadr;
			*(uint32_t*)TabCoffHdr.ahdr.entry = !endianess ? SWAP32(loadadr) : loadadr;
			// write file and "optional" headers
			fwrite(&TabCoffHdr, sizeof(TabCoffHdr), 1, file2);
			// Dump the file
			fwrite(imagefinaladr, 1, linj, file2);
			fclose(file2);
		}
		else
		{
			HandleWriteErrorMsg(buffer);
			Error = FILE_ERROR_ACCESS_DENIED;
		}
	}

	return Error;
}
