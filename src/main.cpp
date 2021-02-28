#include "JiFFI2.h"
#include "version.h"
#include <QtWidgets/QApplication>
#include "tinyxml2.h"

using namespace tinyxml2;

// lo_inp names list
char* lo_inp[] = {
    { "no lo_inp found" },
    { "lo_inp found and allow BJL transmission" }
};

// jcp messages list
char* JCPMessages[] = {
    { "no jcp found" },
    { "jcp found" },
    { "jcp2 found" }
};

void ConvertCMDLine(void);


int main(int argc, char *argv[])
{
    int Error;
    BOOL Help = false;

    // initialisations
    initJiFFI2();
    initCRC();
    ConvertCMDLine();

    // display application information
    printf("JiFFI2 %i.%i.%i - %s\n", MAJOR, MINOR, PATCH, __DATE__);
    printf(
        "Atari Jaguar file format interchanger.\n"
        "Based upon JiFFI v1.42 written in GFA Basic.\n"
        "\n"
    );
 //   printf("JiFFI2 uses tinyxml2 %i.%i.%i\n", TIXML2_MAJOR_VERSION, TIXML2_MINOR_VERSION, TIXML2_PATCH_VERSION);

    // check command line
    if (__argc > 1)
    {
        for (int p = 1; p < __argc; p++)
        {
            // get the argument
            LOG("%s\n", __argv[p]);
            char* arg = __argv[p];

            // get input filename
            if (!_stricmp(arg, "-I"))
            {
                if (++p < __argc)
                {
                    strcpy_s(inp, MAX_PATH, __argv[p]);
                }
            }
            else
            {
                // get output filename
                if (!_stricmp(arg, "-O"))
                {
                    if (++p < __argc)
                    {
                        strcpy_s(out, MAX_PATH, __argv[p]);
                        if (out[strlen(out) - 1] != '\\')
                        {
                            strcat_s(out, MAX_PATH, "\\");
                        }
                    }
                }
                else
                {
                    // get load address (in hexadecimal)
                    if (!_stricmp(arg, "-LOAD"))
                    {
                        if (++p < __argc)
                        {
                            loadadr = atoi(__argv[p]) * 16;
                        }
                    }
                    else
                    {
                        // get executable address (in hexadecimal)
                        if (!_stricmp(arg, "-RUN"))
                        {
                            if (++p < __argc)
                            {
                                runadr = atoi(__argv[p]) * 16;
                            }
                        }
                        else
                        {
                            if (!_stricmp(arg, "-COFF"))
                            {
                                chk_coff.Value = true;
                            }
                            else
                            {
                                if (!_stricmp(arg, "-ELF"))
                                {
                                    chk_elf.Value = true;
                                }
                                else
                                {
                                    if (!_stricmp(arg, "-BJL"))
                                    {
                                        chk_BJL.Value = true;
                                    }
                                    else
                                    {
                                        if (!_stricmp(arg, "-JAGR2"))
                                        {
                                            chk_jagr2.Value = true;
                                        }
                                        else
                                        {
                                            if (!_stricmp(arg, "-JAGR3"))
                                            {
                                                chk_jagr3.Value = true;
                                            }
                                            else
                                            {
                                                if (!_stricmp(arg, "-ROM"))
                                                {
                                                    chk_ROM.Value = true;
                                                }
                                                else
                                                {
                                                    if (!_stricmp(arg, "-ROMH"))
                                                    {
                                                        chk_ROMh.Value = true;
                                                    }
                                                    else
                                                    {
                                                        if (!_stricmp(arg, "-J64"))
                                                        {
                                                            chk_J64.Value = true;
                                                        }
                                                        else
                                                        {
                                                            if (!_stricmp(arg, "-ALL"))
                                                            {
                                                                chk_coff.Value = true;
                                                                chk_elf.Value = true;
                                                                chk_BJL.Value = true;
                                                                chk_jagr2.Value = true;
                                                                chk_jagr3.Value = true;
                                                                chk_ROM.Value = true;
                                                                chk_ROMh.Value = true;
                                                                chk_J64.Value = true;
                                                            }
                                                            else
                                                            {
                                                                if (!_stricmp(arg, "-TREAT-AS-ROM"))
                                                                {
                                                                    //opt_ROM.Value = True
                                                                }
                                                                else
                                                                {
                                                                    if (!_stricmp(arg, "-OVERWRITE"))
                                                                    {
                                                                        chk_overwrite.Value = true;
                                                                    }
                                                                    else
                                                                    {
                                                                        if (!_stricmp(arg, "-?") || !_stricmp(arg, "-h") || !_stricmp(arg, "-HELP"))
                                                                        {
                                                                            Help = true;
                                                                        }
                                                                        else
                                                                        {
                                                                            if ((!_strnicmp(arg, "-JIFFI", 6)) && (strlen(arg) > 6))
                                                                            {
                                                                                chk_JiFFI.Value = atoi(&arg[6]);
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
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (!Help)
        {
            // files in/out checking
            if (inp[0] && !out[0])
            {
                // destination path is not supplied
                size_t z = strlen(inp);
                while (((inp[--z] != '/') && (inp[z] != '\\')) && z);
                memcpy(&out, &inp, z ? (z + 1) : 0);
            }

            // Assume standard loading address if none specified
            if (!loadadr)
            {
                loadadr = 0x4000;
            }

            // Assume standard runing address if none specified
            if (!runadr)
            {
                runadr = loadadr;
            }

            // Have enough parameters been entered?
            if (!inp[0] || !(chk_coff.Value + chk_elf.Value + chk_BJL.Value + chk_jagr2.Value + chk_jagr3.Value + chk_ROM.Value + chk_ROMh.Value + chk_J64.Value))
            {
                printf("Not enough parameters entered.\nIn order for this to work, you must supply at least -i <filename> and one output format.\n");
                exit(1);
            }
            else
            {
                cli = true;
                //cli_run = true;                
                if (!(Error = detect_file_format()))
                {
                    Error = save_file_format();
                }
            }
            //frm1.Close
        }
        else
        {
            // display the help & options
            printf(
                "Usage:\n"
                "   JiFFI2 -i <filename> [options]\n"
                "\n"
                "   Option            Description\n"
                "   ----------------  -----------------------------------\n"
                "   -?, -h or -help   display help\n"
                "   -all              all formats output\n"
                "   -bjl              bjl format output\n"
                "   -coff             coff/abs format output\n"
                "   -elf              elf format output\n"
                "   -i <filename>     input filename\n"
                "   -j64              j64 format output\n"
                "   -jagr2            jagr v2 format output\n"
                "   -jagr3            jagr v3 format output\n"
                "   -jiffi1           JiFFI1\n"
                "   -jiffi2           JiFFI2 (default)\n"
                "   -load <address>   load address (hex)\n"
                "   -o <path>         ouput directory\n"
                "   -overwrite        overwrite the output file(s)\n"
                "   -rom              rom format output\n"
                "   -romh             rom (without header) format output\n"
                "   -run <address>    run address (hex)\n"
                "   -treat-as-rom     \n"
                "\n"
                "Invoking JiFFI2 with no options will cause it to boot up with the GUI."
                "\n"
            );
        }
    }
    else
    {
        // check if jcp2.exe exists
        FILE* file;
        if (!fopen_s(&file, "jcp2.exe", "r"))
        {
            jcp_master = JCP2MASTER;
            strcpy_s(JCPApp, "jcp2.exe");
            cmd_uploadskunk.Enabled = true;
            fclose(file);
        }
        else
        {
            // check if jcp.exe exists
            if (!fopen_s(&file, "jcp.exe", "r"))
            {
                jcp_master = JCPMASTER;
                strcpy_s(JCPApp, "jcp.exe");
                cmd_uploadskunk.Enabled = true;
                fclose(file);
            }
        }
        // display jcp situation
        printf("%s\n", JCPMessages[jcp_master]);

        // check if lo_inp.exe exists
        if (!fopen_s(&file, "lo_inp.exe", "r"))
        {
            cmd_uploadBJL.Enabled = true;
            fclose(file);
        }
        // display lo_inp situation
        printf("%s\n", lo_inp[cmd_uploadBJL.Enabled]);

        // read configuration file
        if (!fopen_s(&file, "JiFFI2.xml", "rb"))         // MUST be open as "rb"
        {
            XMLDocument* JiFFI2Conf = new(XMLDocument);

            // load configuration file
            XMLError error = JiFFI2Conf->LoadFile(file);
            if (error == XML_SUCCESS)
            {
                // point configuration values
                XMLElement* pConfig = JiFFI2Conf->FirstChildElement("Configuration");
                if (pConfig)
                {
                    // get the auto output check
                    XMLElement* pElement = pConfig->FirstChildElement("AutoOutputDir");
                    pElement->QueryBoolText((bool*)&chk_auto_output_dir.Value);
                    // get the directory name output
                    pElement = pConfig->FirstChildElement("Out");
                    strcpy_s(out, MAX_PATH, pElement->GetText());
                    (out[0] == '0') ? out[0] = 0 : 1;
                    // write the overwrite check
                    pElement = pConfig->FirstChildElement("OverWrite");
                    pElement->QueryBoolText((bool*)&chk_overwrite.Value);
                    // write the upload bank 2 check
                    pElement = pConfig->FirstChildElement("UploadBank2");
                    pElement->QueryBoolText((bool*)&chk_uploadbank2.Value);
                    // write the reset skunkboard check
                    pElement = pConfig->FirstChildElement("ResetSkunkboard");
                    pElement->QueryBoolText((bool*)&chk_resetskunk.Value);
                    // write the upload ROM check
                    pElement = pConfig->FirstChildElement("UploadROM");
                    pElement->QueryBoolText((bool*)&chk_uploadROM.Value);
                    // write the 8bits upload check
                    pElement = pConfig->FirstChildElement("BitsUpload");
                    pElement->QueryBoolText((bool*)&chk_8bitupl.Value);
                    // write the no switch check
                    pElement = pConfig->FirstChildElement("NoSwitch");
                    pElement->QueryBoolText((bool*)&chk_noswitch.Value);
                    // configuration file ok
                    printf("\nConfiguration file has been read\n");
                }
                else
                {
                    // config not readable
                    printf("\nConfiguration file not compatible\n");
                }
            }
            else
            {
                // config error
                printf("\nConfiguration file return %s\n", JiFFI2Conf->ErrorStr());
            }
            // close file
            delete JiFFI2Conf;
            fclose(file);
        }
        else
        {
            // no configuration file
            printf("\nConfiguration file not found\n");
        }

        // Qt window
        QApplication a(argc, argv);
        JiFFI2 w;
        w.show();
        a.exec();

        // create configuration file
        if (!fopen_s(&file, "JiFFI2.xml", "w"))
        {
            XMLDocument* JiFFI2Conf = new(XMLDocument);

            // insert XML version
            JiFFI2Conf->InsertEndChild(JiFFI2Conf->NewDeclaration());
            // prepare configuration values
            XMLNode* pConfig = JiFFI2Conf->InsertEndChild(JiFFI2Conf->NewElement("Configuration"));
            pConfig->InsertFirstChild(JiFFI2Conf->NewComment("JiFFI2 configuration values"));
            // insert the auto output check
            XMLElement* pElement = JiFFI2Conf->NewElement("AutoOutputDir");
            pElement->SetText(chk_auto_output_dir.Value);
            pConfig->InsertEndChild(pElement);
            // write the directory name output
            pElement = JiFFI2Conf->NewElement("Out");
            pElement->SetText(*out ? out : "0");
            pConfig->InsertEndChild(pElement);
            // write the overwrite check
            pElement = JiFFI2Conf->NewElement("OverWrite");
            pElement->SetText(chk_overwrite.Value);
            pConfig->InsertEndChild(pElement);
            // write the upload bank 2 check
            pElement = JiFFI2Conf->NewElement("UploadBank2");
            pElement->SetText(chk_uploadbank2.Value);
            pConfig->InsertEndChild(pElement);
            // write the reset skunkboard check
            pElement = JiFFI2Conf->NewElement("ResetSkunkboard");
            pElement->SetText(chk_resetskunk.Value);
            pConfig->InsertEndChild(pElement);
            // write the upload ROM check
            pElement = JiFFI2Conf->NewElement("UploadROM");
            pElement->SetText(chk_uploadROM.Value);
            pConfig->InsertEndChild(pElement);
            // write the 8bits upload check
            pElement = JiFFI2Conf->NewElement("BitsUpload");
            pElement->SetText(chk_8bitupl.Value);
            pConfig->InsertEndChild(pElement);
            // write the no switch check
            pElement = JiFFI2Conf->NewElement("NoSwitch");
            pElement->SetText(chk_noswitch.Value);
            pConfig->InsertEndChild(pElement);
            // save file
            JiFFI2Conf->SaveFile(file);

            // close file
            delete JiFFI2Conf;
            fclose(file);
        }
    }

    //
    closeJiFFI2();
}


// Command line conversion in separate arguments
// Originaly used due to one unique command line
void ConvertCMDLine(void)
{
#if 0
    Local i As Int = 0, j As Int = 0
        'Print "---" + _DosCmd$ + "---"
        Local cmd$ = Trim(_DosCmd$) + " "
        'cmd$ = "i:\svn\reboot\JiFFI\source>JiFFI_1.3.Exe -i c:\Users\gn\Desktop\UFO.BIN -o ""c:\$WINDOWS.~BT\jag\vj2\software"" -load 10000 -run 12000 -rom -all -overwrite -nyan -yaarrr "
        Local LargeArg As Boolean = False
        Local a$

        ReDim __argv(__argmax)

        __argv(0) = App.FileName
        // Anführungszeichen rund um App.FileName entfernen,
        // falls vorhanden
        If Left$(__argv(0), 1) = #34
        __argv(0) = Mid(__argv(0), 2)
        EndIf
        If Right$(__argv(0), 1) = #34
        __argv(0) = Left(__argv(0), Len(__argv(0)) - 1)
        EndIf
        '##E#

        If Left$(cmd$, 1) < > """"
        i = InStr(cmd$, #32)
        Else
        i = InStr(cmd$, #34, 2) : LargeArg = True
        cmd$ = Mid$(cmd$, 2) // Leerzeichen am Anfang abschneiden
        EndIf

        While i > 0
        j++
        If LargeArg
        // Space und Leerzeichen am Ende abschneiden
        a$ = Left$(cmd$, i - 2)
        If Len(a)
        __argv(j) = Left$(cmd$, i - 2)
        Else
        j--
        EndIf
        LargeArg = False
        Else
        // nur Leerzeichen am Ende abschneiden
        a$ = Left$(cmd$, i - 1)
        If Len(a)
        __argv(j) = Left$(cmd$, i - 1)
        Else
        j--
        EndIf
        EndIf

        Exit If(i + 1) > Len(cmd$)
        cmd$ = Mid$(cmd$, i + 1)

        If Left$(cmd$, 1) < > """"
        i = InStr(cmd$, #32)
        Else
        i = InStr(cmd$, #34, 2) : LargeArg = True
        cmd$ = Mid$(cmd$, 2) // Leerzeichen am Anfang abschneiden
        EndIf
        Wend

        // Anzahl der Argumente zurückgeben; ggf. 0
        __argc = j
#endif
}
