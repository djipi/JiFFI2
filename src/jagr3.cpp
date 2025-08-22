#include "common.h"

extern unsigned char _acjagrhead[];
#define _jagrhead _acjagrhead


// 
int create_jagv3(void)
{
	int Error = FILE_ERROR_SUCCESS;
	FILE* file3;

	// create output filename based on the header
	char n[MAX_PATH];
	snprintf(n, sizeof(n), "%s%s_jagserver3_jiffi%i.prg", out, name, chk_JiFFI.Value);

	// Check if file already exists
	if (!chk_overwrite.Value && !fopen_s(&file3, n, "rb") && file3 && !fclose(file3))
	{
		Error = HandleOverwriteMsg(n);
	}

	if (!Error)
	{
		// create the jagserver3 file
		if (!fopen_s(&file3, n, "wb") && file3)
		{
			// write header
			fwrite(_jagrhead, 32, 1, file3);
			// Write v3 header
			fwrite("\0\3", 1, 2, file3);
			unsigned int temp = endianess ? loadadr : SWAP32(loadadr);
			fwrite(&temp, 4, 1, file3);
			temp = endianess ? linj : SWAP32(linj);
			fwrite(&temp, 4, 1, file3);
			temp = endianess ? runadr : SWAP32(runadr);
			fwrite(&temp, 4, 1, file3);
			// Dump the source file
			fwrite(imagefinaladr, linj, 1, file3);
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
