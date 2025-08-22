
#pragma once

typedef struct PatchForm
{
	int index;
	bool selected;
}
PatchForm;

typedef struct PatchData
{
	int title;
	const char* caption;
	int Offset1;
	int NoBytes1;
	unsigned char Bytes1[63];
	int Offset2;
	int NoBytes2;
	unsigned char Bytes2[63];
	int Offset3;
	int NoBytes3;
	unsigned char Bytes3[63];
	int Offset4;
	int NoBytes4;
	unsigned char Bytes4[63];
}
PatchData;

extern PatchData Patches[];
extern PatchForm PatchOptions[];

extern void dopatches(void);
