#include "common.h"

extern unsigned char _acromhead2[];
#define _romhead _acromhead2

const char* HeaderROMName[] =
{
	".rom",
	"_headerless.rom",
	".j64"
};


int create_rom(int header)
{
	int Error = FILE_ERROR_SUCCESS;
	FILE* file3;

	// create output filename based on the header
	char n[MAX_PATH];
	snprintf(n, sizeof(n), "%s%s_jiffi%i%s", out, name, chk_JiFFI.Value, HeaderROMName[header]);

	// Check if file already exists
	if (!chk_overwrite.Value && !fopen_s(&file3, n, "rb") && file3 && !fclose(file3))
	{
		Error = HandleOverwriteMsg(n);
	}

	if (!Error)
	{
		// create the ROM file
		if (!fopen_s(&file3, n, "wb") && file3)
		{
			//progress.Value = 0
			//progress.Max = limg
			// Read romheader and dump it to the file until we hit the first marker (size in bytes-1)
			if ((header == 0) || (header == 2))
			{
				fwrite(_romhead, 0x2012, 1, file3);
			}
			else
			{
				fwrite((_romhead + 0x2000), 0x12, 1, file3);
			}
			// 4 bogus bytes to skip in _romhead
			// write the length of the image
			unsigned int tmp = !endianess ? SWAP32((linj - 1)) : (linj - 1);
			fwrite(&tmp, 4, 1, file3);
			// Read romheader and dump it to the file until we hit the second marker (load adr)
			// 2 bogus bytes to skip
			fwrite((_romhead + 0x2012 + 4), 2, 1, file3);
			// 4 bytes to skip in _romhead
			tmp = !endianess ? SWAP32(loadadr) : loadadr;
			fwrite(&tmp, 4, 1, file3);
			//  Read romheader and dump it to the file until we hit the third marker (run address)
			// 8 bogus bytes to skip
			fwrite((_romhead + 0x2012 + 6), 4, 2, file3);
			tmp = !endianess ? SWAP32(runadr) : runadr;
			fwrite(&tmp, 4, 1, file3);
			// Now dump the input file
			fwrite(imagefinaladr, linj, 1, file3);
			// pad rom
			if ((header == 0) || (header == 2))
			{
				padrom(file3, 8232);
			}
			else
			{
				padrom(file3, 28);
			}
			// close file
			fclose(file3);
		}
		else
		{
			HandleWriteErrorMsg(n);
			Error = FILE_ERROR_ACCESS_DENIED;
		}
	}

	return Error;
}

int convert_rom(int _out)
{
	int Error = FILE_ERROR_SUCCESS;
	FILE* file3;

	// create output filename based on the header
	char n[MAX_PATH];
	snprintf(n, sizeof(n), "%s%s%s", out, name, HeaderROMName[_out]);

	// Check if file already exists
	if (!chk_overwrite.Value && !fopen_s(&file3, n, "rb") && file3 && !fclose(file3))
	{
		Error = HandleOverwriteMsg(n);
	}

	if (!Error)
	{
		// create the ROM file
		if (!fopen_s(&file3, n, "wb") && file3)
		{
			// Only write Universal header if no other header exists
			if (detected_format != format_ROMheader)
			{
				fwrite(_romhead, 1, 0x1000, file3);
			}
			fwrite(imagefinaladr, 1, linj, file3);
			if (!_out || (_out == 2))
			{
				// Only pad when Universal header is added
				if (detected_format != format_ROMheader)
				{
					padrom(file3, 0x2000);
				}
				else
				{
					padrom(file3, 0);
				}
			}
			else
			{
				padrom(file3, 0);
			}

			fclose(file3);
		}
		else
		{
			HandleWriteErrorMsg(n);
			Error = FILE_ERROR_ACCESS_DENIED;
		}
	}

	return Error;
}
