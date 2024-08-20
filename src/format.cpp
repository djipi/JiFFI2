#include "common.h"
#include "libelf/libelf.h"
#include "libelf/gelf.h"
#include "crc32.h"


// Check ROM padding if found
unsigned int checkpad(void)
{
	return strstr(image_orig, search_pad) ? jiffiver_JiFFI1 : (strstr(image_orig, search_pad2) ? jiffiver_JiFFI2 : 0);
}


// Remove ROM padding if found
void removepad(void)
{
	// search JiFFI pad
	char* temp = strstr(image_orig, search_pad);
	if (temp)
	{
		linj = (unsigned int)(temp - image_orig);
	}
	else
	{
		// search JiFFI2 pad
		temp = strstr(image_orig, search_pad2);
		if (temp)
		{
			linj = (unsigned int)(temp - image_orig);
		}
	}
}


// ROM padding
void padrom(FILE* file3, int extrabytes)
{
	int byteswritten = linj + extrabytes;
	unsigned int size = -1;

	// pad to 1mb ROM
	if (byteswritten < 1048576)
	{
		size = (1048576 - byteswritten);
	}
	else
	{
		// pad to 2mb ROM
		if ((byteswritten >= 1048576) && (byteswritten < 2097152))
		{
			size = (2097152 - byteswritten);
		}
		else
		{
			// pad to 4mb ROM
			if ((byteswritten >= 2097152) && (byteswritten < 4194304))
			{
				size = (4194304 - byteswritten);
			}
			else
			{
				// pad to 6mb ROM
				if ((byteswritten >= 4194304) && (byteswritten < 6291456))
				{
					size = (6291456 - byteswritten);
				}
			}
		}
	}

	// write the pad
	if (offset != -1)
	{
		// pad depend the JiFFI version
		switch (chk_JiFFI.Value)
		{
			// JiFFI 1
		case jiffiver_JiFFI1:
			fwrite(pad, size, 1, file3);
			break;

			// JiFFI2
		case jiffiver_JiFFI2:
			fwrite(pad2, size, 1, file3);
			break;

		default:
			break;
		}
	}
}


// Save new formats
int save_file_format(void)
{
	int Error = FILE_ERROR_SUCCESS;

	// make a copy of the image for patches
	memcpy(imagefinaladr, imageadr, linj);
	dopatches();

	// Create COFF
	if (chk_coff.Value)
	{
		Error = create_coff();
	}

	// Create ELF
	if (chk_elf.Value)
	{
		Error = create_elf();
	}

	// Create ROM
	if (chk_ROM.Value)
	{
		if ((detected_format == format_ROMheaderless) || (detected_format == format_ROMheader) || ((detected_format == format_undetected) && (opt_BJL.Value == 0)))
		{
			Error = convert_rom(0);
		}
		else
		{
			Error = create_rom(0);
		}
	}

	// Create BJL
	if (chk_BJL.Value)
	{
		Error = create_bjl();
	}

	// Create JAGR2
	if (chk_jagr2.Value)
	{
		Error = create_jagv2();
	}

	// Create JAGR3
	if (chk_jagr3.Value)
	{
		Error = create_jagv3();
	}

	// Create ROM headerless
	if (chk_ROMh.Value)
	{
		if ((detected_format == format_ROMheaderless) || (detected_format == format_ROMheader) || ((detected_format == format_undetected) && (opt_BJL.Value == 0)))
		{
			Error = convert_rom(1);
		}
		else
		{
			Error = create_rom(1);
		}
	}

	// Create J64
	if (chk_J64.Value)
	{
		if ((detected_format == format_ROMheaderless) || (detected_format == format_ROMheader) || ((detected_format == format_undetected) && (opt_BJL.Value == 0)))
		{
			convert_rom(2);
		}
		else
		{
			create_rom(2);
		}
	}

	// return
	return Error;
}


// File format detection
int detect_file_format(void)
{
	unsigned char* ptrload;
	unsigned int temp2, temp;
	int Error = FILE_ERROR_SUCCESS;
	FILE* File;

	//if (!cli_run)
	//{
	//	ed_load = ""
	//	ed_run = ""
	//}

	// check valid file
	if (!fopen_s(&File, inp, "rb") && File)
	{
		// get the file name
		size_t i, j = i = strlen(inp);
		while ((inp[--j] != '.') && j);
		while (((inp[--i] != '/') && (inp[i] != '\\')) && i);
		memcpy(name, &inp[(i ? ++i : i)], (j - i));

		// get file size
		fseek(File, 0, SEEK_END);
		linj = ftell(File);
		rewind(File);

		// load file
		if ((ptrload = (unsigned char*)malloc(linj)))
		{
			// read & close file
			fread(ptrload, 1, linj, File);
			fclose(File);
			// initialisations
			detected_format = format_undetected;
			loadadr = runadr = DebugInfoSize = 0;

			// check for ELF header & ELF library version
			if ((linj > 4) && (ptrload[0] == ELFMAG0) && (ptrload[1] == ELFMAG1) && (ptrload[2] == ELFMAG2) && (ptrload[3] == ELFMAG3) && (elf_version(EV_CURRENT) != EV_NONE))
			{
				//ElfMem = (Elf*)ptr;
				if (((((((Elf32_Ehdr*)ptrload)->e_machine) & 0xFF) == EM_68K) && ((((Elf32_Ehdr*)ptrload)->e_type) == ET_EXEC)) || (!endianess && (((((Elf32_Ehdr*)ptrload)->e_machine) & 0xFF00) == (EM_68K << 8)) && ((((Elf32_Ehdr*)ptrload)->e_type) == (ET_EXEC << 8))))
				{
					unsigned char* ptr = NULL;
					size_t NbrSect;
					Elf* ElfMem = elf_memory((char*)ptrload, linj);
					if (!elf_getshdrnum(ElfMem, &NbrSect))
					{
						GElf_Ehdr ElfEhdr, * PtrGElfEhdr;
						Elf_Scn* PtrElfScn;
						if (((PtrGElfEhdr = gelf_getehdr(ElfMem, &ElfEhdr)) != NULL) && ((PtrElfScn = elf_getscn(ElfMem, 0)) != NULL))
						{
							// init offsets low/high
							size_t offsetlow, offsethigh = 0;
							// get load address
							loadadr = -1;
							// loop on the ELF information to get useful parts and loading address
							GElf_Shdr GElfShdr, * PtrGElfShdr;
							GElf_Phdr GElfPhdr, * PtrGElfPhdr;
							for (int index = 0; (PtrElfScn != NULL); PtrElfScn = elf_nextscn(ElfMem, PtrElfScn), index = 0)
							{
								PtrGElfShdr = gelf_getshdr(PtrElfScn, &GElfShdr);
								switch (PtrGElfShdr->sh_type)
								{
									// Program data
								case SHT_PROGBITS:
									if ((PtrGElfShdr->sh_flags & (SHF_ALLOC | SHF_WRITE | SHF_EXECINSTR)))
									{
										while ((PtrGElfPhdr = gelf_getphdr(ElfMem, index++, &GElfPhdr)) ? (PtrGElfPhdr->p_offset != PtrGElfShdr->sh_offset) : false);
										if (PtrGElfPhdr)
										{
											if (PtrGElfPhdr->p_paddr < loadadr)
											{
												loadadr = (unsigned int)(PtrGElfPhdr->p_paddr);
												if (ptr < (ptrload + PtrGElfPhdr->p_offset))
												{
													ptr = (ptrload + (offsetlow = PtrGElfPhdr->p_offset));
												}
											}
											if (PtrGElfPhdr->p_offset >= offsethigh)
											{
												offsethigh = PtrGElfPhdr->p_offset + PtrGElfShdr->sh_size;
											}
										}
									}
									//else
									//{
									//	linj -= (unsigned int)(PtrGElfShdr->sh_size);
									//}
									break;
									// Symbol table
								case SHT_SYMTAB:
									// String table
								case SHT_STRTAB:
									// Section header table entry unused
								case SHT_NULL:
									// Program space with no data (bss)
								case SHT_NOBITS:
									// reduce the size with the section's size
								default:
									//linj -= (unsigned int)(PtrGElfShdr->sh_size);
									break;
								}
							}
							// get run address
							//runadr = !endianess ? (uint32_t)PtrGElfEhdr->e_entry : ((((uint32_t)PtrGElfEhdr->e_entry & 0xff000000) >> 24) | (((uint32_t)PtrGElfEhdr->e_entry & 0x00ff0000) >> 8) | (((uint32_t)PtrGElfEhdr->e_entry & 0x0000ff00) << 8) | (((uint32_t)PtrGElfEhdr->e_entry & 0x000000ff) << 24));
							runadr = (uint32_t)PtrGElfEhdr->e_entry;
							// copy the used code + data
							memcpy(imageadr, ptr, (linj = offsethigh-offsetlow));
							// ELF format considered correct
							detected_format = format_ELF;
						}
					}
				}
			}
			else
			{
				// check for COFF header
				unsigned char* ptr = ptrload;
				int temp = endianess ? *(short int*)ptr : ((*ptr << 8) | *(ptr + 1));
				if ((temp == 0x601b) || (temp == 0x150))
				{
					// get the debug info size
					DebugInfoSize = !endianess ? (*(ptr + 8) << 24) | (*(ptr + 9) << 16) | (*(ptr + 10) << 8) | *(ptr + 11) : *(uint32_t*)ptr;
					DebugInfoSize = DebugInfoSize ? ((DebugInfoSize < linj) ? (linj - DebugInfoSize) : 0) : 0;
					// check the COFF magic number
					if (temp == 0x150)
					{
						ptr += 0x24;
					}
					else
					{
						ptr += 0x16;
					}
					// get the executable and load address
					runadr = loadadr = !endianess ? (*ptr << 24) | (*(ptr + 1) << 16) | (*(ptr + 2) << 8) | *(ptr + 3) : *(uint32_t*)ptr;
					// copy the binary
					if (temp == 0x150)
					{
						ptr += (0xa8 - 0x24);
						linj -= 0xa8;
					}
					else
					{
						ptr += (0x24 - 0x16);
						linj -= 0x24;
					}
					// load the image to ram
					linj -= DebugInfoSize;
					memcpy(imageadr, ptr, linj);
					detected_format = format_COFF;
				}
				else
				{
					// Check for JAGR header
					unsigned char* ptr = ptrload;
					int temp = endianess ? *(short int*)ptr : ((*ptr << 8) | *(ptr + 1));
					if (temp == 0x601a)
					{
						ptr += 0x1c;
						uint32_t temp2= !endianess ? (*ptr << 24) | (*(ptr + 1) << 16) | (*(ptr + 2) << 8) | *(ptr + 3) : *(uint32_t*)ptr;
						if (temp2 == 0x4a414752)
						{
							// JAGR
							ptr += 4;
							temp = endianess ? *(short int*)ptr : ((*ptr << 8) | *(ptr + 1));
							if (temp == 3)
							{
								// v3 information
								loadadr = !endianess ? (*(ptr + 2) << 24) | (*(ptr + 3) << 16) | (*(ptr + 4) << 8) | *(ptr + 5) : *(uint32_t*)(ptr + 2);
								runadr = !endianess ? (*(ptr + 10) << 24) | (*(ptr + 11) << 16) | (*(ptr + 12) << 8) | *(ptr + 13) : *(uint32_t*)(ptr + 6);
								detected_format = format_JAGR3;
								ptr += (0x2e - 0x20);
								linj -= 0x2e;
								memcpy(imageadr, ptr, linj);
							}
							else
							{
								if (temp == 2)
								{
									// v2 information
									runadr = loadadr = !endianess ? (*(ptr + 2) << 24) | (*(ptr + 3) << 16) | (*(ptr + 4) << 8) | *(ptr + 5) : *(uint32_t*)(ptr + 2);
									detected_format = format_JAGR2;
									ptr += (0x2a - 0x20);
									linj -= 0x2a;
									memcpy(imageadr, ptr, linj);
								}
							}
						}
					}
					else
					{
						// Check for ROM files created from the JiFFI original program (the old v1.1 format)
						if (linj > 0x2020)
						{
							unsigned char* ptr = ptrload;
							offset = -1;
							uint32_t temp2 = !endianess ? (*(ptr + 0x2014) << 24) | (*(ptr + 0x2015) << 16) | (*(ptr + 0x2016) << 8) | *(ptr + 0x2017) : *(uint32_t*)(ptr + 0x2014);
							if (temp2 == 0x6cfa4ef9)
							{
								offset = 0x2000;
							}
							else
							{
								temp2 = !endianess ? (*(ptr + 0x14) << 24) | (*(ptr + 0x15) << 16) | (*(ptr + 0x16) << 8) | *(ptr + 0x17) : *(uint32_t*)(ptr + 0x14);
								if (temp2 == 0x6cfa4ef9)
								{
									offset = 0;
								}
							}

							if (offset != -1)
							{
								// JiFFI ROM
								loadadr = !endianess ? (*(ptr + offset + 6) << 24) | (*(ptr + offset + 7) << 16) | (*(ptr + offset + 8) << 8) | *(ptr + offset + 9) : *(uint32_t*)(ptr + offset + 6);
								runadr = !endianess ? (*(ptr + offset + 0x18) << 24) | (*(ptr + offset + 0x19) << 16) | (*(ptr + offset + 0x1a) << 8) | *(ptr + offset + 0x1b) : *(uint32_t*)(ptr + offset + 0x18);
								detected_format = format_JiFFIROM1;
								// JiFFI rom <=v1.1, so get actual file size from the copy routine
								linj = !endianess ? (*(ptr + offset + 0xc) << 24) | (*(ptr + offset + 0xd) << 16) | (*(ptr + offset + 0xe) << 8) | *(ptr + offset + 0xf) : *(uint32_t*)(ptr + offset + 0xc);
								linj = (linj + 1) * 2;
								ptr += (offset + 0x1c);
								// load the image to ram
								memcpy(imageadr, ptr, linj);
								// remove ROM padding if found
								removepad();
							}
						}

						if (!detected_format)
						{
							// Check for ROM files created from this program (v1.2 format)
							if (linj > 0x2028)
							{
								unsigned char* ptr = ptrload;
								offset = -1;
								uint32_t temp2 = !endianess ? (*(ptr + 0x2020) << 24) | (*(ptr + 0x2021) << 16) | (*(ptr + 0x2022) << 8) | *(ptr + 0x2023) : *(uint32_t*)(ptr + 0x2020);
								if (temp2 == 0x6cfa4ef9)
								{
									offset = 0x2000;
								}
								else
								{
									temp2 = !endianess ? (*(ptr + 0x20) << 24) | (*(ptr + 0x21) << 16) | (*(ptr + 0x22) << 8) | *(ptr + 0x23) : *(uint32_t*)(ptr + 0x20);
									if (temp2 == 0x6cfa4ef9)
									{
										offset = 0;
									}
								}

								if (offset != -1)
								{
									// new JiFFI ROM
									loadadr = !endianess ? (*(ptr + offset + 0x18) << 24) | (*(ptr + offset + 0x19) << 16) | (*(ptr + offset + 0x1a) << 8) | *(ptr + offset + 0x1b) : *(uint32_t*)(ptr + offset + 0x18);
									runadr = !endianess ? (*(ptr + offset + 0x24) << 24) | (*(ptr + offset + 0x25) << 16) | (*(ptr + offset + 0x26) << 8) | *(ptr + offset + 0x27) : *(uint32_t*)(ptr + offset + 0x24);
									detected_format = format_JiFFIROM2;
									// JiFFI rom, so get actual file size from the copy routine
									linj = !endianess ? (*(ptr + offset + 0x12) << 24) | (*(ptr + offset + 0x13) << 16) | (*(ptr + offset + 0x14) << 8) | *(ptr + offset + 0x15) : *(uint32_t*)(ptr + offset + 0x12);
									linj++;
									ptr += (offset + 0x28);
									// load the image to ram
									memcpy(imageadr, ptr, linj);
									// remove ROM padding if found
									removepad();
								}
							}

							if (!detected_format)
							{
								// Check for ROM files (with header or not)
								unsigned char* ptr = ptrload;
								uint32_t temp2 = !endianess ? (*ptr << 24) | (*(ptr + 1) << 16) | (*(ptr + 2) << 8) | *(ptr + 3) : *(uint32_t*)ptr;
								if (temp2 == 0xf642233c)
								{
									temp2 = !endianess ? (*(ptr + 4) << 24) | (*(ptr + 5) << 16) | (*(ptr + 6) << 8) | *(ptr + 7) : *(uint32_t*)(ptr + 4);
									if (temp2 == 0x0dac1d8d)
									{
										// ROM with universal header
										detected_format = format_ROMUnivHeader;
										ptr += 0x2000;
										linj -= 0x2000;
										// load the image to ram
										memcpy(imageadr, ptr, linj);
										// remove ROM padding if found
										removepad();
									}
								}

								if (!detected_format)
								{
									// only do the next check if the image is at least 8k (which is the size of the header)
									if (linj >= 8192)
									{
										// search for any kind of ROM header
										unsigned char* ptr = ptrload;
										// since ROM headers are mosly filled with $ff, we're scanning the first 8k for occurances, and if we find enough, it's safe to assume that this is hopefully an actual ROM header
										int temp = 0;
										for (int i = 0; i < 8192 / 4 - 1; i++)
										{
											temp2 = !endianess ? (*(ptr + (i * 4)) << 24) | (*(ptr + (i * 4) + 1) << 16) | (*(ptr + (i * 4) + 2) << 8) | *(ptr + (i * 4) + 3) : *(uint32_t*)(ptr + (i * 4));
											if (temp2 == 0xffffffff)
											{
												temp++;
											}
										}

										if (temp > 1500)
										{
											// ROM with some kind of header
											detected_format = format_ROMheader;
											// load the image to ram
											memcpy(imageadr, ptrload, linj);
											// remove ROM padding if found
											removepad();
										}
									}

									if (!detected_format)
									{
										if ((linj == (1024 * 1024 - 8192)) || (linj == (2 * 1024 * 1024 - 8192)) || (linj == (4 * 1024 * 1024 - 8192)) || (linj == (1024 * 1024)) || (linj == (2 * 1024 * 1024)) || (linj == (1024 * 1024)))
										{
											// headerless ROM
											detected_format = format_ROMheaderless;
											// load the image to ram
											memcpy(imageadr, ptrload, linj);
											// remove ROM padding if found
											removepad();
										}
										else
										{
											// could be a file that loads to ram (BJL)
											if (linj <= 2080768)
											{
												detected_format = format_2MB;
												//chk_coff.Enabled = chk_elf.Enabled = chk_ROM.Enabled = chk_BJL.Enabled = chk_jagr2.Enabled = chk_jagr3.Enabled = chk_ROMh.Enabled = chk_J64.Enabled = True;
												//ed_load.Enabled = ed_run.Enabled = true;
												//if (!cli_run)
												//{
												//	ed_load.Text = "4000"
												//	ed_run.Text = "4000"
												//}
												loadadr = runadr = 0x4000;
												//opt_BJL.Value = True;
												//opt_ROM.Enabled = opt_BJL.Enabled = true;
												// Load the image to ram
												memcpy(imageadr, ptrload, linj);
												// Remove ROM padding if found
												removepad();
											}
											else
											{
												if (linj <= (6 * 1024 * 1024))
												{
													// could be a ROM file
													detected_format = format_6MB;
													//chk_coff.Enabled = chk_elf.Enabled = chk_BJL.Enabled = chk_jagr2.Enabled = chk_jagr3.Enabled = False;
													//chk_ROM.Enabled = chk_ROMh.Enabled = chk_J64.Enabled = True;
													//ed_load.Enabled = ed_run.Enabled = false;
													//if (!cli_run)
													//{
													//	ed_load.Text = "";
													//	ed_run.Text = "";
													//}
													//opt_ROM.Value = true;
													//opt_ROM.Enabled = opt_BJL.Enabled = false;
													// Load the image to ram
													memcpy(imageadr, ptrload, linj);
													// Remove ROM padding if found
													removepad();
												}
												else
												{
													// cannot be considered as a ROM
													Error = FILE_ERROR_FILE_TOOBIG;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

			//if (!detected_format)
			if (Error)
			{
				crc = 0;
				//inp[0] = 0;
				//ed_load.Enabled = ed_run.Enabled = opt_BJL.Enabled = opt_ROM.Enabled = false;
				//GoTo detect_getout
			}
			else
			{
				//ed_load.Enabled = ed_run.Enabled = opt_BJL.Enabled = opt_ROM.Enabled = false;
				// Try to detect tile from CRC32
				crc = crc32(imageadr, linj);
				for (TitleName = NULL, temp = 0, DetectedTitle = -1; (temp < NumTitles) && !TitleName; temp++)
				{
					if (crc == titles[temp].crc)
					{
						TitleName = titles[temp].titlename;
						DetectedTitle = temp;
					}
				}

				// If title was detected, check to see if there are any patches for it and populate the patch window
				for (int temp = 0; temp < 10; temp++)
				{
					PatchOptions[temp].index = 0;
					PatchOptions[temp].selected = false;
				}
				for (temp = temp2 = 0; temp < NumPatches; temp++)
				{
					if (Patches[temp].title == DetectedTitle)
					{
						//PatchOptions[temp2].selected = true;
						PatchOptions[temp2++].index = temp;
					}
				}
				PatchesForCurrentTitle = temp2;
				// pad the binary to have alignment on 2 bytes 
				*(imageadr + linj) = 0;
				linj = (linj + 1) & 0xfffffffe;
			}

			free(ptrload);
		}
	}
	else
	{
		Error = FILE_ERROR_FILE_CANNOTREAD;
	}

	// return
	return Error;
}
