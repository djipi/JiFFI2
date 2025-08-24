#include "common.h"
#include "gelf.h"
#include "libelf.h"

extern unsigned char _acelfhead[];
#define _elfhead _acelfhead
extern unsigned char _acelfstringtable[16];
#define StringTable _acelfstringtable 

// Sections header
Elf32_Shdr Shdr[3] = {
	// NULL
	{
	},
	// string table
	{
		SWAP32(1),																		// sh_name
		SWAP32(SHT_STRTAB),																// sh_type
		SWAP32(0),																		// sh_flags
		SWAP32(0),																		// sh_addr
		SWAP32((sizeof(Elf32_Ehdr) + (sizeof(Elf32_Shdr) * 3) + sizeof(Elf32_Phdr))),	// sh_offset
		SWAP32(sizeof(_acelfstringtable)),												// sh_size
		SWAP32(SHN_UNDEF),																// sh_link
		SWAP32(SHN_UNDEF),																// sh_info
		SWAP32(1),																		// sh_addralign
		SWAP32(0)																		// sh_entsize
	},
	// Executable
	{
		SWAP32(9),																									// sh_name
		SWAP32(SHT_PROGBITS),																						// sh_type
		SWAP32((SHF_ALLOC + SHF_EXECINSTR)),																		// sh_flags
		0,																											// sh_addr
		SWAP32((sizeof(Elf32_Ehdr) + (sizeof(Elf32_Shdr) * 3) + sizeof(Elf32_Phdr) + sizeof(_acelfstringtable))),	// sh_offset
		0,																											// sh_size
		SWAP32(SHN_UNDEF),																							// sh_link
		SWAP32(SHN_UNDEF),																							// sh_info
		SWAP32(2),																									// sh_addralign
		SWAP32(0)																									// sh_entsize
	}
};

// Program header
Elf32_Phdr Phdr[1] = {
	// Executable
	{
		SWAP32(PT_LOAD),																							// p_type
		SWAP32((sizeof(Elf32_Ehdr) + (sizeof(Elf32_Shdr) * 3) + sizeof(Elf32_Phdr) + sizeof(_acelfstringtable))),	// p_offset
		0,																											// p_vaddr
		0,																											// p_paddr
		0,																											// p_filesz
		0,																											// p_memsz
		SWAP32((PF_R + PF_X)),																						// p_flags
		SWAP32(2)																									// p_align
	}
};


int create_elf(void)
{
	int Error = FILE_ERROR_SUCCESS;
	FILE* file3;

	// create output filename based on the header
	char n[MAX_PATH];
	snprintf(n, sizeof(n), "%s%s_jiffi%i.elf", out, name, chk_JiFFI.Value);

	// Check if file already exists
	if (!chk_overwrite.Value && !fopen_s(&file3, n, "rb") && file3 && !fclose(file3))
	{
		Error = HandleOverwriteMsg(n);
	}

	if (!Error)
	{
		// create the elf file
		if (!fopen_s(&file3, n, "wb") && file3)
		{
			// sections header initialisations
			Shdr[2].sh_addr = !endianess ? SWAP32(loadadr) : loadadr;
			Shdr[2].sh_size = !endianess ? SWAP32(linj) : linj;
			// programm header initialisations
			Phdr[0].p_vaddr = Phdr[0].p_paddr = !endianess ? SWAP32(loadadr) : loadadr;
			Phdr[0].p_filesz = Phdr[0].p_memsz = !endianess ? SWAP32(linj) : linj;
			// header initialisations
			((Elf32_Ehdr*)_elfhead)->e_entry = !endianess ? SWAP32(runadr) : runadr;
			// write the elf header
			fwrite(_elfhead, sizeof(Elf32_Ehdr), 1, file3);
			// write the program header
			fwrite(Phdr, sizeof(Elf32_Phdr), 1, file3);
			// write the sections header
			fwrite(Shdr, sizeof(Elf32_Shdr), 3, file3);
			// write the String Table
			fwrite(StringTable, sizeof(_acelfstringtable), 1, file3);
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
