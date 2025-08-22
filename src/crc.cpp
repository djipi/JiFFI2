#include "common.h"


uint32_t crc;
crctype titles[256];


void initCRC(void)
{
	int i = 0;
	int j = 0;
	int z;

	//
	// The following table is from shamus' virtual jaguar :P
	//
	titles[i].crc = 0x0509C85E; titles[i].titlename = "Raiden (World) (alt)"; titles[i].flags = FF_ROM | FF_REQ_BIOS; i++;
	titles[i].crc = 0x08849D0F; titles[i].titlename = "Hyper Force (World)"; titles[i].flags = FF_ALPINE | FF_VERIFIED; i++;
	titles[i].crc = 0x08F15576; titles[i].titlename = "Iron Soldier (World) (v1.04)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x0957A072; titles[i].titlename = "Kasumi Ninja (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x0AC83D77; titles[i].titlename = "NBA Jam T.E. (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x0EC5369D; titles[i].titlename = "Evolution - Dino Dudes (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x0F6A1C2C; titles[i].titlename = "Ultra Vortek (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x14915F20; titles[i].titlename = "White Men Can't Jump (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x1660F070; titles[i].titlename = "Power Drive Rally (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x1A20C5C4; titles[i].titlename = "Protector (World)"; titles[i].flags = FF_ROM | FF_VERIFIED | FF_REQ_DSP; i++;
	titles[i].crc = 0x1E451446; titles[i].titlename = "Trevor McFur in the Crescent Galaxy (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x20936557; titles[i].titlename = "Space War 2000"; titles[i].flags = FF_ALPINE | FF_VERIFIED; i++;
	titles[i].crc = 0x27594C6A; titles[i].titlename = "Defender 2000 (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x2BAA92A1; titles[i].titlename = "Space War 2000 (World) (OVERDUMP)"; titles[i].flags = FF_ALPINE; i++;
	titles[i].crc = 0x2E17D5DA; titles[i].titlename = "Bubsy in Fractured Furry Tales (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x31812799; titles[i].titlename = "Raiden (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x3241AB6A; titles[i].titlename = "Towers II"; titles[i].flags = FF_ALPINE; i++;
	titles[i].crc = 0x348E6449; titles[i].titlename = "Double Dragon V - The Shadow Falls (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x3615AF6A; titles[i].titlename = "Fever Pitch Soccer (World) (En,Fr,De,Es,It)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x38A130ED; titles[i].titlename = "Troy Aikman NFL Football (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x40E1A1D0; titles[i].titlename = "Air Cars (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x4471BFA0; titles[i].titlename = "Skyhammer (World)"; titles[i].flags = FF_ALPINE | FF_VERIFIED; i++;
	titles[i].crc = 0x47EBC158; titles[i].titlename = "Theme Park (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x4899628F; titles[i].titlename = "Hover Strike (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x4A08A2BD; titles[i].titlename = "SuperCross 3D (World)"; titles[i].flags = FF_ROM | FF_BAD_DUMP; i++;
	titles[i].crc = 0x544E7A01; titles[i].titlename = "Downfall (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x55A0669C; titles[i].titlename = "[BIOS] Atari Jaguar Developer CD (World)"; titles[i].flags = FF_BIOS; i++;
	titles[i].crc = 0x58272540; titles[i].titlename = "Syndicate (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x5A101212; titles[i].titlename = "Sensible Soccer - International Edition (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x5B6BB205; titles[i].titlename = "Ruiner Pinball (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x5CFF14AB; titles[i].titlename = "Pinball Fantasies (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x5DDF9724; titles[i].titlename = "Protector - Special Edition (World)"; titles[i].flags = FF_ALPINE | FF_VERIFIED; i++;
	titles[i].crc = 0x5E2CDBC0; titles[i].titlename = "Doom (World)"; titles[i].flags = FF_ROM | FF_VERIFIED | FF_REQ_DSP; i++;
	titles[i].crc = 0x5F2C2774; titles[i].titlename = "Battle Sphere (World)"; titles[i].flags = FF_ROM | FF_VERIFIED | FF_REQ_DSP; i++;
	titles[i].crc = 0x61C7EEC0; titles[i].titlename = "Zero 5 (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x61EE6B62; titles[i].titlename = "Arena Football '95"; titles[i].flags = FF_ALPINE | FF_VERIFIED; i++;
	titles[i].crc = 0x67F9AB3A; titles[i].titlename = "Battle Sphere Gold (World)"; titles[i].flags = FF_ROM | FF_REQ_DSP; i++;
	titles[i].crc = 0x687068D5; titles[i].titlename = "[BIOS] Atari Jaguar CD (World)"; titles[i].flags = FF_BIOS; i++;
	titles[i].crc = 0x6B2B95AD; titles[i].titlename = "Tempest 2000 (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	Patches[j].title = i - 1; Patches[j].caption = "Enable Rotary by default (GGN)";
	Patches[j].Offset1 = 0x24ba; Patches[j].NoBytes1 = 4;
	Patches[j].Bytes1[0] = 0x4e; Patches[j].Bytes1[1] = 0x71; Patches[j].Bytes1[2] = 0x4e; Patches[j].Bytes1[3] = 0x71;
	Patches[j].Offset2 = 0x47dc; Patches[j].NoBytes2 = 4;
	Patches[j].Bytes2[0] = 0x4e; Patches[j].Bytes2[1] = 0x71; Patches[j].Bytes2[2] = 0x4e; Patches[j].Bytes2[3] = 0x71;
	Patches[j].Offset3 = 0x4401; Patches[j].NoBytes3 = 1;
	Patches[j].Bytes3[0] = 0x02;
	Patches[j].Offset4 = 0x3054; Patches[j].NoBytes4 = 4;
	Patches[j].Bytes4[0] = 0x4e; Patches[j].Bytes4[1] = 0x71; Patches[j].Bytes4[2] = 0x4e; Patches[j].Bytes4[3] = 0x71;
	j++;
	Patches[j].title = i - 1; Patches[j].caption = "Infinite lives (CJ)";
	Patches[j].Offset1 = 0xc4b3; Patches[j].NoBytes1 = 1;
	Patches[j].Bytes1[0] = 0x00;
	Patches[j].Offset2 = 0xc557; Patches[j].NoBytes2 = 1;
	Patches[j].Bytes2[0] = 0x00;
	j++;

	// Tempest 2000 rotary on
	// was              replaced by
	// 24ba  67 00 00 0c      4e 71 4e 71
	// 3054  67 00 00 20      4e 71 4e 71
	// 4401  02               01
	// 47dc  66 00 00 28      4e 71 4e 71
	//
	// t2k inf lives
	// $0479000100007d3c change to 0479000000007d3c
	// thats @ c4b0
	// and another one at $c554
	//

	titles[i].crc = 0x6EB774EB; titles[i].titlename = "Worms (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x6F8B2547; titles[i].titlename = "Super Burnout (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x732FFAB6; titles[i].titlename = "Soccer Kid (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x817A2273; titles[i].titlename = "Pitfall - The Mayan Adventure (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x83A3FB5D; titles[i].titlename = "Towers II"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x892BC67C; titles[i].titlename = "Flip Out! (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x8975F48B; titles[i].titlename = "Zool 2 (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x89DA21FF; titles[i].titlename = "Phase Zero"; titles[i].flags = FF_ALPINE | FF_VERIFIED | FF_REQ_DSP; i++;
	titles[i].crc = 0x8D15DBC6; titles[i].titlename = "[BIOS] Atari Jaguar Stubulator '94 (World)"; titles[i].flags = FF_BIOS; i++;
	titles[i].crc = 0x8FEA5AB0; titles[i].titlename = "Dragon - The Bruce Lee Story (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x91095DD3; titles[i].titlename = "Brett Hull Hockey"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x95143668; titles[i].titlename = "Trevor McFur in the Crescent Galaxy (World) (alt)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0x97EB4651; titles[i].titlename = "I-War (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xA0A25A67; titles[i].titlename = "Missile Command VR"; titles[i].flags = FF_ALPINE; i++;
	titles[i].crc = 0xA27823D8; titles[i].titlename = "Ultra Vortek (World) (v0.94) (Beta)"; titles[i].flags = FF_ROM; i++;
	titles[i].crc = 0xA7E01FEF; titles[i].titlename = "Mad Bodies (2008)"; titles[i].flags = FF_ROM | FF_REQ_BIOS; i++;
	titles[i].crc = 0xA9F8A00E; titles[i].titlename = "Rayman (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xAEA9D831; titles[i].titlename = "Barkley Shut Up & Jam"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xB14C4753; titles[i].titlename = "Fight for Life (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xB5604D40; titles[i].titlename = "Breakout 2000"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xBA13AE79; titles[i].titlename = "Soccer Kid (World) (alt)"; titles[i].flags = FF_ALPINE; i++;
	titles[i].crc = 0xBCB1A4BF; titles[i].titlename = "Brutal Sports Football (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xBD18D606; titles[i].titlename = "Space War 2000 (World) (alt)"; titles[i].flags = FF_ALPINE; i++;
	titles[i].crc = 0xBDA405C6; titles[i].titlename = "Cannon Fodder (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xBDE67498; titles[i].titlename = "Cybermorph (World) (Rev 1)"; titles[i].flags = FF_ROM | FF_VERIFIED | FF_REQ_DSP; i++;
	titles[i].crc = 0xC2898F6E; titles[i].titlename = "Barkley Shut Up & Jam (alt)"; titles[i].flags = FF_ALPINE; i++;
	titles[i].crc = 0xC36E935E; titles[i].titlename = "Beebris (World)"; titles[i].flags = FF_ALPINE | FF_VERIFIED; i++;
	titles[i].crc = 0xC5562581; titles[i].titlename = "Zoop! (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xC654681B; titles[i].titlename = "Total Carnage (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xC6C7BA62; titles[i].titlename = "Fight for Life (World) (alt)"; titles[i].flags = FF_ROM | FF_BAD_DUMP; i++;
	titles[i].crc = 0xC9608717; titles[i].titlename = "Val d'Isere Skiing and Snowboarding (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xCBFD822A; titles[i].titlename = "Air Cars (World) (alt)"; titles[i].flags = FF_ROM | FF_BAD_DUMP; i++;
	titles[i].crc = 0xCD5BF827; titles[i].titlename = "Attack of the Mutant Penguins (World)"; titles[i].flags = FF_ROM | FF_VERIFIED | FF_REQ_DSP; i++;
	titles[i].crc = 0xD6C19E34; titles[i].titlename = "Iron Soldier 2 (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xD8696F23; titles[i].titlename = "Breakout 2000 (alt)"; titles[i].flags = FF_ALPINE; i++;
	titles[i].crc = 0xDA9C4162; titles[i].titlename = "Missile Command 3D (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xDC187F82; titles[i].titlename = "Alien vs Predator (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xDCCDEF05; titles[i].titlename = "Brett Hull Hockey"; titles[i].flags = FF_ALPINE; i++;
	titles[i].crc = 0xDDFF49F5; titles[i].titlename = "Rayman (Prototype)"; titles[i].flags = FF_ALPINE; i++;
	titles[i].crc = 0xDE55DCC7; titles[i].titlename = "Flashback - The Quest for Identity (World) (En,Fr)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xE28756DE; titles[i].titlename = "Atari Karts (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xE60277BB; titles[i].titlename = "[BIOS] Atari Jaguar Stubulator '93 (World)"; titles[i].flags = FF_BIOS; i++;
	titles[i].crc = 0xE91BD644; titles[i].titlename = "Wolfenstein 3D (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xEA9B3FA7; titles[i].titlename = "Phase Zero"; titles[i].flags = FF_ROM | FF_REQ_DSP; i++;
	titles[i].crc = 0xEC22F572; titles[i].titlename = "SuperCross 3D (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xECF854E7; titles[i].titlename = "Cybermorph (World) (Rev 2)"; titles[i].flags = FF_ROM | FF_REQ_DSP; i++;
	titles[i].crc = 0xEEE8D61D; titles[i].titlename = "Club Drive (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xF4ACBB04; titles[i].titlename = "Tiny Toon Adventures (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	titles[i].crc = 0xFA7775AE; titles[i].titlename = "Checkered Flag (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	Patches[j].title = i - 1; Patches[j].caption = "Steering patch (CJ)";
	Patches[j].Offset1 = 0x6be0; Patches[j].NoBytes1 = 12;
	Patches[j].Bytes1[0] = 0xe6; Patches[j].Bytes1[1] = 0x32; for (z = 2; z < 12; z += 2) { Patches[j].Bytes1[z] = 0x4e; Patches[j].Bytes1[(z + 1)] = 0x71; }
	Patches[j].Offset2 = 0x6bfa; Patches[j].NoBytes2 = 12;
	Patches[j].Bytes2[0] = 0xe6; Patches[j].Bytes2[1] = 0x43; for (z = 2; z < 12; z += 2) { Patches[j].Bytes2[z] = 0x4e; Patches[j].Bytes2[(z + 1)] = 0x71; }
	j++;
	// Checkered flag
	//
	// 6be0 (change to)       e6 32 4e 71 4e 71 4e 71 4e 71 4e 71
	// 6bfa                   e6 43 4e 71 4e 71 4e 71 4e 71 4e 71
	titles[i].crc = 0xFAE31DD0; titles[i].titlename = "Flip Out! (World) (alt)"; titles[i].flags = FF_ROM; i++;
	titles[i].crc = 0xFB731AAA; titles[i].titlename = "[BIOS] Atari Jaguar (World)"; titles[i].flags = FF_BIOS; i++;
	titles[i].crc = 0xFDF37F47; titles[i].titlename = "Memory Track Cartridge (World)"; titles[i].flags = FF_ROM | FF_VERIFIED; i++;
	//
	// The rest of the entries are mine, all mine!!11111onoenoenene
	//
	titles[i].crc = 0xC10408F8; titles[i].titlename = "3DPOLYTT (?)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xBC3C8272; titles[i].titlename = "Arkanna Demo"; titles[i].flags = 0; i++;
	titles[i].crc = 0x6160393C; titles[i].titlename = "ASI test program"; titles[i].flags = 0; i++;
	titles[i].crc = 0x82629A5B; titles[i].titlename = "Asteroids"; titles[i].flags = 0; i++;
	titles[i].crc = 0xB12A8610; titles[i].titlename = "Atomic reloaded"; titles[i].flags = 0; i++;
	titles[i].crc = 0x98A97BFA; titles[i].titlename = "Badcode0"; titles[i].flags = 0; i++;
	titles[i].crc = 0x0AC78B32; titles[i].titlename = "BadCode1"; titles[i].flags = 0; i++;
	titles[i].crc = 0xCD507E35; titles[i].titlename = "BadCode2"; titles[i].flags = 0; i++;
	titles[i].crc = 0xAC95CA6D; titles[i].titlename = "BadCode3"; titles[i].flags = 0; i++;
	titles[i].crc = 0x8F1586A9; titles[i].titlename = "BadCode4m (not working?)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xBA97C724; titles[i].titlename = "BadCode4m with metalwork"; titles[i].flags = 0; i++;
	titles[i].crc = 0x111D4E2C; titles[i].titlename = "Badcode4m (as Badcode4c, but with metalwork)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x22178D94; titles[i].titlename = "Badcode4"; titles[i].flags = 0; i++;
	titles[i].crc = 0x75E3A725; titles[i].titlename = "BadCode4c"; titles[i].flags = 0; i++;
	titles[i].crc = 0xB5A901DF; titles[i].titlename = "BadCode4n (as Badcode4m, but with bug-fixes)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x19B0B734; titles[i].titlename = "Beebris tribute"; titles[i].flags = 0; i++;
	titles[i].crc = 0xA64E2219; titles[i].titlename = "Black hole"; titles[i].flags = 0; i++;
	titles[i].crc = 0x7BE52319; titles[i].titlename = "Black Jag Hyper Power League intro"; titles[i].flags = 0; i++;
	titles[i].crc = 0x9634B653; titles[i].titlename = "CD bypass"; titles[i].flags = 0; i++;
	titles[i].crc = 0x34764742; titles[i].titlename = "Checkcom"; titles[i].flags = 0; i++;
	titles[i].crc = 0x115254E1; titles[i].titlename = "Chroma-Luma Color pick (early version)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x7D839061; titles[i].titlename = "Chroma-Luma Color Pick (Mouse)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x22B676C3; titles[i].titlename = "Chroma-Luma Color Pick"; titles[i].flags = 0; i++;
	titles[i].crc = 0xE4A509E0; titles[i].titlename = "Dance dance Xirius space party"; titles[i].flags = 0; i++;
	titles[i].crc = 0xCA694A33; titles[i].titlename = "Dazed vs"; titles[i].flags = 0; i++;
	titles[i].crc = 0x712D3ACF; titles[i].titlename = "Degz"; titles[i].flags = 0; i++;
	titles[i].crc = 0x6208C06E; titles[i].titlename = "RGC 2006 intro"; titles[i].flags = 0; i++;
	titles[i].crc = 0xB6BD78E8; titles[i].titlename = "Downfall"; titles[i].flags = 0; i++;
	titles[i].crc = 0x48537047; titles[i].titlename = "Drumpad (?)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xBAF04511; titles[i].titlename = "Drumpad2 (?)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xA9D033EF; titles[i].titlename = "Do the same"; titles[i].flags = 0; i++;
	titles[i].crc = 0x8A0913AD; titles[i].titlename = "Full Action Sprites (F.Act.S.)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xD6E6CE82; titles[i].titlename = "Fire"; titles[i].flags = 0; i++;
	titles[i].crc = 0x509826F1; titles[i].titlename = "Frontier build 20060412"; titles[i].flags = 0; i++;
	titles[i].crc = 0x76568F76; titles[i].titlename = "Frontier build 20060602"; titles[i].flags = 0; i++;
	titles[i].crc = 0x9843A7CC; titles[i].titlename = "frontier20060713"; titles[i].flags = 0; i++;
	titles[i].crc = 0xB4C74036; titles[i].titlename = "Gorf 2000"; titles[i].flags = 0; i++;
	titles[i].crc = 0xBDDF030D; titles[i].titlename = "Gpuapfel"; titles[i].flags = 0; i++;
	titles[i].crc = 0x4C7DD2A3; titles[i].titlename = "HMS RAPTOR"; titles[i].flags = 0; i++;
	titles[i].crc = 0x2708D9FE; titles[i].titlename = "Hubble Fade"; titles[i].flags = 0; i++;
	titles[i].crc = 0x72DD1CB8; titles[i].titlename = "Hubble Nebula 2000"; titles[i].flags = 0; i++;
	titles[i].crc = 0x211FD3BE; titles[i].titlename = "Removers intro"; titles[i].flags = 0; i++;
	titles[i].crc = 0x2502DBB7; titles[i].titlename = "Jag_bal4"; titles[i].flags = 0; i++;
	titles[i].crc = 0x7DAE6472; titles[i].titlename = "JagFest Demo 1998"; titles[i].flags = 0; i++;
	titles[i].crc = 0x88F4D5ED; titles[i].titlename = "JagMania (Apr 18 1998)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x01673CBB; titles[i].titlename = "JagMania (Aug 31 1998)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x88AE7BA2; titles[i].titlename = "JagMania (Jan 06)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x88F4D5ED; titles[i].titlename = "JagMania (Jul 8)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xF4737C6D; titles[i].titlename = "JagMania (Sep 10)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x115AA29A; titles[i].titlename = "JagMarble (Jul 13 1997)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x1B57A623; titles[i].titlename = "JagMarble"; titles[i].flags = 0; i++;
	titles[i].crc = 0x1B57A623; titles[i].titlename = "JagMarble"; titles[i].flags = 0; i++;
	titles[i].crc = 0xF8B50435; titles[i].titlename = "27 objects (Jaguar Server Program Examples)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x126D4967; titles[i].titlename = "50Hz (Jaguar Server Program Examples)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x477885BC; titles[i].titlename = "60Hz (Jaguar Server Program Examples)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x04DA8B26; titles[i].titlename = "Jaguar Intro (Jaguar Server Program Examples)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xFD16DBFA; titles[i].titlename = "Jaguar Intro modified (Jaguar Server Program Examples)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xD0768C5E; titles[i].titlename = "Scale (Jaguar Server Program Examples)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x3F284669; titles[i].titlename = "Scale3 (Jaguar Server Program Examples)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x209EC328; titles[i].titlename = "Jaguar Server 1.08 UPDATE - JAGOS"; titles[i].flags = 0; i++;
	titles[i].crc = 0x6912ED13; titles[i].titlename = "Jaguar Server 1.08 UPDATE - KEYB"; titles[i].flags = 0; i++;
	titles[i].crc = 0xF600158C; titles[i].titlename = "Jaguar Tetris"; titles[i].flags = 0; i++;
	titles[i].crc = 0xE8AC685C; titles[i].titlename = "Jaguar connexion 2005 demo"; titles[i].flags = 0; i++;
	titles[i].crc = 0xEFFCA60E; titles[i].titlename = "Jaguar Development Club Demo V1"; titles[i].flags = 0; i++;
	titles[i].crc = 0x8DC5D93C; titles[i].titlename = "Jaguar Development Club Demo V2"; titles[i].flags = 0; i++;
	titles[i].crc = 0xAA25060D; titles[i].titlename = "Jaguar Development Club Demo V3 - E-JagFest Demo"; titles[i].flags = 0; i++;
	titles[i].crc = 0xF2DFDCC6; titles[i].titlename = "Jaguar Development Club Example"; titles[i].flags = 0; i++;
	titles[i].crc = 0x4211EBFB; titles[i].titlename = "Joypad-TeamTap Tester v2"; titles[i].flags = 0; i++;
	titles[i].crc = 0x2A65C0D4; titles[i].titlename = "Joypad-TeamTap Tester"; titles[i].flags = 0; i++;
	titles[i].crc = 0x6D3BAE95; titles[i].titlename = "Jaguar Sound System V1.0 (08.11.2002)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xFFC859F8; titles[i].titlename = "Jaguar Sound System II (10.11.2002)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x58C71C6A; titles[i].titlename = "Jungle Jag"; titles[i].flags = 0; i++;
	titles[i].crc = 0xC62B8115; titles[i].titlename = "Kobayashi Maru"; titles[i].flags = 0; i++;
	titles[i].crc = 0x1A68194C; titles[i].titlename = "Legion Force Jidai Intro Demo 0"; titles[i].flags = 0; i++;
	titles[i].crc = 0x1AE64D8B; titles[i].titlename = "Mandelbrot set"; titles[i].flags = 0; i++;
	titles[i].crc = 0x39181742; titles[i].titlename = "Mandelbrot set"; titles[i].flags = 0; i++;
	titles[i].crc = 0x00028BA0; titles[i].titlename = "Mars demo"; titles[i].flags = 0; i++;
	titles[i].crc = 0xFB08FD76; titles[i].titlename = "Mars demo (192)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xEF5BF180; titles[i].titlename = "Memory Dump"; titles[i].flags = 0; i++;
	titles[i].crc = 0x207FA331; titles[i].titlename = "Martian attack!"; titles[i].flags = 0; i++;
	titles[i].crc = 0x0F80E61A; titles[i].titlename = "My fire"; titles[i].flags = 0; i++;
	titles[i].crc = 0x6BAF3E15; titles[i].titlename = "Nyanjag mono"; titles[i].flags = 0; i++;
	titles[i].crc = 0xE7626244; titles[i].titlename = "Nyanjag stereo"; titles[i].flags = 0; i++;
	titles[i].crc = 0xF0D06B76; titles[i].titlename = "Osmozys"; titles[i].flags = 0; i++;
	titles[i].crc = 0xCE90E88C; titles[i].titlename = "Overlay (?)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x4AD4AA33; titles[i].titlename = "Painter"; titles[i].flags = 0; i++;
	titles[i].crc = 0x3CF18719; titles[i].titlename = "PAULA Preview Demo 2"; titles[i].flags = 0; i++;
	titles[i].crc = 0xDA061A4E; titles[i].titlename = "PAULA Preview Demo"; titles[i].flags = 0; i++;
	titles[i].crc = 0x0B216C1A; titles[i].titlename = "PlaySFX(?)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xA9993C33; titles[i].titlename = "Poly_mmu (?)'"; titles[i].flags = 0; i++;
	titles[i].crc = 0xC0386C2A; titles[i].titlename = "Polygon"; titles[i].flags = 0; i++;
	titles[i].crc = 0xD769C21D; titles[i].titlename = "Project W"; titles[i].flags = 0; i++;
	titles[i].crc = 0x5E372E78; titles[i].titlename = "Raw"; titles[i].flags = 0; i++;
	titles[i].crc = 0xFEC3E046; titles[i].titlename = "Reactris"; titles[i].flags = 0; i++;
	titles[i].crc = 0xB86BB002; titles[i].titlename = "Robot big"; titles[i].flags = 0; i++;
	titles[i].crc = 0xEA11932F; titles[i].titlename = "Rover demo v4"; titles[i].flags = 0; i++;
	titles[i].crc = 0x7446A528; titles[i].titlename = "S.p.a.c.e"; titles[i].flags = 0; i++;
	titles[i].crc = 0xA66136BE; titles[i].titlename = "Superfly DX v1.1"; titles[i].flags = 0; i++;
	titles[i].crc = 0x05B7E30D; titles[i].titlename = "Shoot'em'up"; titles[i].flags = 0; i++;
	titles[i].crc = 0xAD9DA962; titles[i].titlename = "Slam Racer"; titles[i].flags = 0; i++;
	titles[i].crc = 0xB6D3C7BE; titles[i].titlename = "SlamRacer Intro"; titles[i].flags = 0; i++;
	titles[i].crc = 0xD9AE96AE; titles[i].titlename = "Surrounded! (Christmas 2006)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x1D34DF62; titles[i].titlename = "Surrounded! (fixed)"; titles[i].flags = 0; i++;
	titles[i].crc = 0x0402AF98; titles[i].titlename = "Surrounded! 2009"; titles[i].flags = 0; i++;
	titles[i].crc = 0x929A43A1; titles[i].titlename = "Test_gra (?)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xEF594E8C; titles[i].titlename = "Jaguar Tetris"; titles[i].flags = 0; i++;
	titles[i].crc = 0x4750BBFF; titles[i].titlename = "The Assassin Demo - Release 2"; titles[i].flags = 0; i++;
	titles[i].crc = 0x0DB47415; titles[i].titlename = "The Assassin Demo Part 1"; titles[i].flags = 0; i++;
	titles[i].crc = 0x323AA72D; titles[i].titlename = "TNG demo"; titles[i].flags = 0; i++;
	titles[i].crc = 0x4168972C; titles[i].titlename = "Tube"; titles[i].flags = 0; i++;
	titles[i].crc = 0x957D9B50; titles[i].titlename = "Tube SE"; titles[i].flags = 0; i++;
	titles[i].crc = 0x194D30CB; titles[i].titlename = "Tunnel"; titles[i].flags = 0; i++;
	titles[i].crc = 0x6C67A89E; titles[i].titlename = "UFO"; titles[i].flags = 0; i++;
	titles[i].crc = 0x48ABDA30; titles[i].titlename = "Jagtopia: freeboot"; titles[i].flags = 0; i++;
	titles[i].crc = 0x094FE458; titles[i].titlename = "Voxels"; titles[i].flags = 0; i++;
	titles[i].crc = 0xE4FE4ADD; titles[i].titlename = "Warum_T (?)"; titles[i].flags = 0; i++;
	titles[i].crc = 0xAED486CA; titles[i].titlename = "Wolf3D"; titles[i].flags = 0; i++;
	titles[i].crc = 0x1D7A3399; titles[i].titlename = "Yakbig demo"; titles[i].flags = 0; i++;
	titles[i].crc = 0x8D772E00; titles[i].titlename = "Jaglion"; titles[i].flags = 0; i++;
	titles[i].crc = 0x1C73A235; titles[i].titlename = "Jagrotate"; titles[i].flags = 0; i++;
	titles[i].crc = 0x73ADBE01; titles[i].titlename = "Blackout!"; titles[i].flags = 0; i++;
	titles[i].crc = 0x68F86E72; titles[i].titlename = "Dark guardian episode 1: Unknown enemy"; titles[i].flags = 0; i++;
	titles[i].crc = 0x54ac5231; titles[i].titlename = "Blackout demo"; titles[i].flags = 0; i++;
	titles[i].crc = 0xD6974ACE; titles[i].titlename = "CRZ demo"; titles[i].flags = FF_ROM; i++;
	titles[i].crc = 0xE89D0F72; titles[i].titlename = "Ladybug demo 1"; titles[i].flags = FF_ROM; i++;
	titles[i].crc = 0xE1734368; titles[i].titlename = "Ladybug demo 1b"; titles[i].flags = FF_ROM; i++;
	titles[i].crc = 0xE2A294FA; titles[i].titlename = "Ladybug demo 1c"; titles[i].flags = FF_ROM; i++;
	titles[i].crc = 0x549844D6; titles[i].titlename = "Ladybug demo"; titles[i].flags = FF_ROM; i++;
	titles[i].crc = 0xBC62A2BF; titles[i].titlename = "QSOUND demo"; titles[i].flags = FF_ROM; i++;
	titles[i].crc = 0x93EF8837; titles[i].titlename = "Rover demo v2"; titles[i].flags = FF_ROM; i++;
	titles[i].crc = 0x0E46AFAD; titles[i].titlename = "Zero demo 1"; titles[i].flags = FF_ROM; i++;
	titles[i].crc = 0x100A9F46; titles[i].titlename = "Zero demo 4"; titles[i].flags = FF_ROM; i++;
	titles[i].crc = 0xEE3EA164; titles[i].titlename = "Duckie egg"; titles[i].flags = 0; i++;
	NumTitles = i;
	NumPatches = j;
}


uint32_t calculCRC(const char* s, unsigned int len)
{
	uint32_t crc = 0xFFFFFFFF;

	for (unsigned int i = 0; i < len; i++) {
		char ch = s[i];
		for (unsigned int j = 0; j < 8; j++) {
			uint32_t b = (ch ^ crc) & 1;
			crc >>= 1;
			if (b) crc = crc ^ 0xEDB88320;
			ch >>= 1;
		}
	}

	return ~crc;
}
