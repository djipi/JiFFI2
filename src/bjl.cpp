#include "common.h"


// Create BJL file
int create_bjl(void)
{
	int Error = FILE_ERROR_SUCCESS;
	FILE* file3;

	// create output filename based on the header
	char n[MAX_PATH];
	snprintf(n, sizeof(n), "%s%s_load_%06x_run_%06x_jiffi%i.bjl", out, name, loadadr, runadr, chk_JiFFI.Value);

	// Check if file already exists
	if (!chk_overwrite.Value && !fopen_s(&file3, n, "rb") && file3 && !fclose(file3))
	{
		Error = HandleOverwriteMsg(n);
	}

	if (!Error)
	{
		// create the bjl file
		if (!fopen_s(&file3, n, "wb") && file3)
		{
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
