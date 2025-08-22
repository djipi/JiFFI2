#include "common.h"


PatchData Patches[256];
PatchForm PatchOptions[10];


// Apply patches (if available and selected)
void dopatches(void)
{
	int i, j, offset;

	for (i = 0; i < 10; i++)
	{
		if (PatchOptions[i].selected)
		{
			if (Patches[PatchOptions[i].index].NoBytes1 > 0)
			{
				offset = Patches[PatchOptions[i].index].Offset1;

				for (j = 0; j < (Patches[PatchOptions[i].index].NoBytes1 - 1); j++)
				{
					*(imagefinaladr + offset + j) = Patches[PatchOptions[i].index].Bytes1[j];
				}
			}
						
			if (Patches[PatchOptions[i].index].NoBytes2 > 0)
			{
				offset = Patches[PatchOptions[i].index].Offset2;

				for (j = 0; j < Patches[PatchOptions[i].index].NoBytes2 - 1; j++)
				{
					*(imagefinaladr + offset + j) = Patches[PatchOptions[i].index].Bytes2[j];
				}
			}

			if (Patches[PatchOptions[i].index].NoBytes3 > 0)
			{
				offset = Patches[PatchOptions[i].index].Offset3;

				for (j = 0; j < Patches[PatchOptions[i].index].NoBytes3 - 1; j++)
				{
					*(imagefinaladr + offset + j) = Patches[PatchOptions[i].index].Bytes3[j];
				}
			}

			if (Patches[PatchOptions[i].index].NoBytes4 > 0)
			{
				offset = Patches[PatchOptions[i].index].Offset4;

				for (j = 0; j < Patches[PatchOptions[i].index].NoBytes4 - 1; j++)
				{
					*(imagefinaladr + offset + j) = Patches[PatchOptions[i].index].Bytes4[j];
				}
			}
		}
	}
}
