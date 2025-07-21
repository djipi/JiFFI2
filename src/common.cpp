#include "common.h"
#include <QMessageBox>


char inp[MAX_PATH], out[MAX_PATH], name[MAX_PATH];
unsigned int linj;
int limg, injectoffset, i;
unsigned int loadadr, runadr;
int detected_format;
enumjcp jcp_master;
char JCPApp[10];
unsigned int DebugInfoSize;
long offset;							// 0 when ROM file is headerless, $2000 when file has universal header
//BOOL songloaded;
BOOL cli;	// , cli_run;
uint32_t crc;
//char* search = "JiFFI2";
char* image_patch;
char* imagefinaladr;
char* image_orig;
char* imageadr;
//char* pad;
const char* TitleName;
unsigned int NumTitles;
int DetectedTitle, CurrentDetectedTitle;
unsigned int PatchesForCurrentTitle, NumPatches;
PatchData Patches[256];
PatchForm PatchOptions[10];
crctype titles[256];
//int __argc;
//char** __argv;
chk chk_coff, chk_elf, chk_ROM, chk_BJL, chk_jagr2, chk_jagr3, chk_ROMh, chk_J64;
chk chk_uploadROM, chk_resetskunk, chk_uploadbank2;
chk chk_8bitupl, chk_noswitch;
chk chk_overwrite;
chk chk_auto_output_dir;
chk chk_JiFFI;
chk cmd_uploadBJL, cmd_uploadskunk;
chk opt_BJL, opt_ROM;
BOOL endianess;
BOOL EditorAddr;
int NbUseFormat;


// Initialisations
void initJiFFI2(void)
{
	// get the endianess of the host
	uint16_t checkendian = 1;
	endianess = !*(unsigned char*)&checkendian;
	// allocations
	imageadr = image_orig = (char*)malloc(6 * 1024 * 1024);
	imagefinaladr = image_patch = (char*)malloc(6 * 1024 * 1024);
	// values by default
	chk_JiFFI.Value = jiffiver_JiFFI2;
	jcp_master = NULLMASTER;
}


// Closing
void closeJiFFI2(void)
{
	free(image_orig);
	free(image_patch);
}


//
void HandleWriteErrorMsg(char* PtrFilename)
{
	// check command line
	if (!cli)
	{
		// setup the message box
		QMessageBox msgBox;
		QString msg;
		msg.asprintf("File %s cannot be written", PtrFilename);
		msgBox.setText(msg);
		msgBox.exec();
	}
	else
	{
		// print message to command line
		printf("File %s cannot be written\n", PtrFilename);
	}
}


//
int HandleOverwriteMsg(char* PtrFilename)
{
	// check command line
	if (!cli)
	{
		// setup the message box
		QMessageBox msgBox;
		QString msg;
		msg.asprintf("File %s already exists", PtrFilename);
		msgBox.setText(msg);
		msgBox.setInformativeText("Do you want to overwrite it?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		// get the user feedback
		int retVal = msgBox.exec();
		if (retVal != QMessageBox::No)
		{
			return FILE_ERROR_SUCCESS;
		}
	}
	else
	{
		// print message to command line
		printf("File %s already exists\n", PtrFilename);
	}

	//
	return FILE_ERROR_FILE_EXISTS;
}
