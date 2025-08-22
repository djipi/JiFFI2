#include "common.h"
#include "time.h"


// File header structure for COFF format - 20 bytes
struct COFF_filehdr {
	char f_magic[2];	// magic number
	char f_nscns[2];	// number of sections
	char f_timdat[4];	// time & date stamp
	char f_symptr[4];	// file pointer to symtab
	char f_nsyms[4];	// number of symtab entries
	char f_opthdr[2];	// sizeof(optional hdr)
	char f_flags[2];	// flags
};

// Optional header structure for COFF format - 28 bytes - Required for executable
struct COFF_opthdr
{
	char magic[2];					// type of file
	char vstamp[2];					// version stamp
	char tsize[4];					// text size in bytes, padded to FW bdry
	char dsize[4];					// initialized data size
	char bsize[4];					// uninitialized data size
	char entry[4];					// entry point.
	unsigned char text_start[4];	// base of text used for this file
	char data_start[4];				// base of data used for this file
};

// Section header structure for COFF format - 40 bytes - One for each section
struct COFF_scnhdr {
	char s_name[8];				// section name
	char s_paddr[4];			// physical address, aliased s_nlib
	char s_vaddr[4];			// virtual address
	char s_size[4];				// section size
	unsigned char s_scnptr[4];	// file ptr to raw data for section
	char s_relptr[4];			// file ptr to relocation
	char s_lnnoptr[4];			// file ptr to line numbers
	char s_nreloc[2];			// number of relocation entries
	char s_nlnno[2];			// number of line number entries
	char s_flags[4];			// flags
};

#define COFF_STYP_TEXT	0x20
#define COFF_STYP_DATA	0x40
#define COFF_STYP_BSS	0x80

#define COFF_F_RELFLG	0x1
#define COFF_F_EXEC		0x2
#define COFF_F_LNNO		0x4
#define COFF_F_LSYMS	0x10

#define COFF_F			(COFF_F_RELFLG | COFF_F_LNNO | COFF_F_LSYMS | COFF_F_EXEC)

struct COFF_filehdr	fhdr = {
	// magic number
	0x01, 0x50,
	// number of sections
	0x0, 0x03,
	// time & date stamp
	0x0, 0x0, 0x0, 0x0,
	// file pointer to symtab
	0x0, 0x0, 0x0, 0x0,
	// number of symtab entries
	0x0, 0x0, 0x0, 0x0,
	// sizeof (optional hdr)
	0x0, sizeof(COFF_opthdr),
	// flags
	((COFF_F & 0xff00) >> 8), (COFF_F & 0xff)
};

struct COFF_opthdr ophdr = {
	// type of file
	0x01, 0x50,
	// version stamp
	0x0, 0x1,
	// text size in bytes, padded to FW bdry
	0x0, 0x0, 0x0, 0x0,
	// initialized data size
	0x0, 0x0, 0x0, 0x0,
	// uninitialized data size
	0x0, 0x0, 0x0, 0x0,
	// entry pt.
	0x0, 0x0, 0x0, 0x0,
	// base of text used for this file
	0x0, 0x0, 0x0, (sizeof(COFF_filehdr) + sizeof(COFF_opthdr) + (sizeof(COFF_scnhdr)) * 3),
	// base of data used for this file
	0x0, 0x0, 0x0, 0x0
};

struct COFF_scnhdr shdr[3] = {
	// .text
	{
		// .text string
		0x2e, 0x74, 0x65, 0x78, 0x74, 0x0, 0x0, 0x0,
		// physical address, aliased s_nlib
		0x0, 0x0, 0x0, 0x0,
		// virtual address
		0x0, 0x0, 0x0, 0x0,
		// section size
		0x0, 0x0, 0x0, 0x0,
		// file ptr to raw data for section
		0x0, 0x0, 0x0, (sizeof(COFF_filehdr) + sizeof(COFF_opthdr) + sizeof(shdr)),
		// file ptr to relocation
		0x0, 0x0, 0x0, 0x0,
		// file ptr to line numbers
		0x0, 0x0, 0x0, 0x0,
		// number of relocation entries
		0x0, 0x0,
		// number of line number entries
		0x0, 0x0,
		((COFF_STYP_TEXT & 0xff000000) >> 24), ((COFF_STYP_TEXT & 0x00ff0000) >> 16), ((COFF_STYP_TEXT & 0x0000ff00) >> 8), (COFF_STYP_TEXT & 0x000000ff)
	},
	// .data
	{
		// .data string
		0x2e, 0x64, 0x61, 0x74, 0x61, 0, 0, 0,
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
		// .bss string
		0x2e, 0x62, 0x73, 0x73, 0, 0, 0, 0,
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
};


int create_coff(void)
{
	FILE* file2;
	int Error = FILE_ERROR_SUCCESS;

	// create output filename
	char buffer[MAX_PATH];
	snprintf(buffer, sizeof(buffer), "%s%s_jiffi%i.coff", out, name, chk_JiFFI.Value);

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
			// set time creation in the file header
			time_t TimeStamp, Time;
			Time = time(&TimeStamp);
			*(uint32_t*)fhdr.f_timdat = !endianess ? SWAP32(Time) : Time;
			// write the file header
			fwrite(&fhdr, sizeof(fhdr), 1, file2);
			
			// set the optional header
			*(uint32_t*)ophdr.tsize = !endianess ? SWAP32(linj) : linj;
			*(uint32_t*)ophdr.entry = !endianess ? SWAP32(runadr) : runadr;
			*(uint32_t*)ophdr.text_start = !endianess ? SWAP32(loadadr) : loadadr;
			// write the optional header
			fwrite(&ophdr, sizeof(ophdr), 1, file2);

			// .text section initialisations
			*(uint32_t*)shdr[0].s_paddr = !endianess ? SWAP32(loadadr) : loadadr;
			*(uint32_t*)shdr[0].s_vaddr = !endianess ? SWAP32(loadadr) : loadadr;
			*(uint32_t*)shdr[0].s_size = !endianess ? SWAP32(linj) : linj;
			// write the section
			fwrite(&shdr, sizeof(shdr), 1, file2);
			
			// write the content
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
