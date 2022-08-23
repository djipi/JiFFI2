#include "JiFFI2.h"
#include "about.h"
#include "extraopts.h"
#include "qprocess.h"
#include "ui_JiFFI2.h"
#include "ui_about.h"
#include "ui_extraopts.h"


// Format names
const char* FormatName[] = {
    {   "None detected - either BJL, ROM or binary" },
    {   "COFF/ABS"  },
    {   "Jaguar Server v3"  },
    {   "Jaguar Server v2"  },
    {   "ROM created by JiFFI up to v1.1"   },
    {   "ULS"   },
    {   "ROM with header"   },
    {   "Headerless ROM"    },
    {   "ROM created by JiFFI v1.2" },
    {   "Weird ROM" },
    {   "ROM with universal header" },
    {   "ELF"   },
    {   "2MB limit - either BJL, ROM or binary" },
    {   "6MB limit - either ROM or binary" }
};

about* JiFFI2_About;
extraopts* JiFFI2_ExtraOpts;
Ui_extraopts ui_extraopts;


JiFFI2::JiFFI2(QWidget* parent)
    : QMainWindow(parent)
{
    // prepare the extra option window
    JiFFI2_ExtraOpts = new extraopts(this);
    ui_extraopts.setupUi(JiFFI2_ExtraOpts);
    // prepare the about window
    Ui_about ui_about;
    JiFFI2_About = new about(this);
    ui_about.setupUi(JiFFI2_About);
    // display the main UI
    ui.setupUi(this);
    RefreshUI();
    // connect the signals for the main menu selections
    QObject::connect(ui.action_About, SIGNAL(triggered()), this, SLOT(cmd_about_Click()));
    QObject::connect(ui.action_ResetSettings, SIGNAL(triggered()), this, SLOT(cmd_resetsettings_Click()));
}


// Reset settings feature
void JiFFI2::cmd_resetsettings_Click(void)
{
    // reset the output settings
    out[0] = 0;
    chk_overwrite.Value = chk_auto_output_dir.Value = false;
    // reset the BJL settings
    chk_8bitupl.Value = chk_noswitch.Value = false;
    // reset the Skunkboard settings
    chk_uploadROM.Value = chk_uploadbank2.Value = chk_resetskunk.Value = false;
    // refresh UI
    RefreshUI();
}


// About window
void JiFFI2::cmd_about_Click(void)
{
    JiFFI2_About->show();
}


// Set the information in the UI
void JiFFI2::RefreshUI(void)
{
    // set for the files (source and output path) information
    ui.label_sourcefile->setText(QString::fromLatin1("%1").arg(inp));
    ui.label_outputpath->setText(QString::fromLatin1("%1").arg(out));
    ui.cmd2->setEnabled(!chk_auto_output_dir.Value);
    ui.chk_auto_output_dir->setCheckState(chk_auto_output_dir.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_auto_output_dir->setEnabled(inp[0]);

    // set addresses (load & run) information
    ui.label->setEnabled(inp[0]);
    ui.ed_loadadr->setText((inp[0] && loadadr) ? QString::fromLatin1("%1").arg(QString::number(loadadr, 16)) : NULL);
    ui.ed_loadadr->setEnabled(inp[0] && EditorAddr);
    ui.label_2->setEnabled(inp[0]);
    ui.ed_runadr->setText((inp[0] && runadr) ? QString::fromLatin1("%1").arg(QString::number(runadr, 16)) : NULL);
    ui.ed_runadr->setEnabled(inp[0] && EditorAddr);

    // set the state of each format, this state can also be affected by the toggle on/off feature
    NbUseFormat = chk_coff.Value + chk_elf.Value + chk_ROM.Value + chk_BJL.Value + chk_jagr2.Value + chk_jagr3.Value + chk_ROMh.Value + chk_J64.Value;
    ui.chk_coff->setCheckState(chk_coff.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_coff->setEnabled(out[0] && chk_coff.Enabled);
    ui.chk_elf->setCheckState(chk_elf.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_elf->setEnabled(out[0] && chk_elf.Enabled);
    ui.chk_ROM->setCheckState(chk_ROM.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_ROM->setEnabled(out[0] && chk_ROM.Enabled);
    ui.chk_BJL->setCheckState(chk_BJL.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_BJL->setEnabled(out[0] && chk_BJL.Enabled);
    ui.chk_jagr2->setCheckState(chk_jagr2.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_jagr2->setEnabled(out[0] && chk_jagr2.Enabled);
    ui.chk_jagr3->setCheckState(chk_jagr3.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_jagr3->setEnabled(out[0] && chk_jagr3.Enabled);
    ui.chk_ROMh->setCheckState(chk_ROMh.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_ROMh->setEnabled(out[0] && chk_ROMh.Enabled);
    ui.chk_J64->setCheckState(chk_J64.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_J64->setEnabled(out[0] && chk_J64.Enabled);

    // set the detected format information
    ui.lbl_format->setText(inp[0] ? QString::fromLatin1("%1").arg(FormatName[detected_format]) : NULL);
    ui.label_5->setEnabled(inp[0]);
    ui.lbl_title->setText(inp[0] ? QString::fromLatin1("%1").arg(TitleName) : NULL);
    ui.label_6->setEnabled(TitleName ? true : false);
    ui.lbl_CRC32->setText(crc ? QString::fromLatin1("%1").arg(QString::number(crc, 16).toUpper()) : NULL);
    ui.label_7->setEnabled(crc);
    ui.lbl_debuginfosize->setText(DebugInfoSize ? QString::fromLatin1("%1 bytes").arg(QString::number(DebugInfoSize, 10)) : NULL);
    ui.label_8->setEnabled(DebugInfoSize);

    // set the ROM & BJL options capabilities
    ui.label_3->setEnabled(opt_ROM.Enabled | opt_BJL.Enabled);
    ui.opt_ROM->setEnabled(opt_ROM.Enabled ? true : false);
    ui.opt_ROM->setAutoExclusive(false);
    ui.opt_ROM->setChecked(opt_ROM.Value ? Qt::Checked : Qt::Unchecked);
    ui.opt_BJL->setEnabled(opt_BJL.Enabled ? true : false);
    ui.opt_BJL->setAutoExclusive(false);
    ui.opt_BJL->setChecked(opt_BJL.Value ? Qt::Checked : Qt::Unchecked);

    // set the create files capabilities
    ui.cmd_create->setEnabled((inp[0] && out[0] && NbUseFormat));
    ui.chk_overwrite->setCheckState(chk_overwrite.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_overwrite->setEnabled(ui.cmd_create->isEnabled());

    // set the extra options capabilities
    ui.cmd_extraopts->setEnabled((PatchesForCurrentTitle > 0) ? true : false);

    // set the jcp2 upload capability
    ui.cmd_uploadskunk_Click->setEnabled(cmd_uploadskunk.Enabled && detected_format);
    ui.chk_uploadbank2->setEnabled(cmd_uploadskunk.Enabled && detected_format);
    ui.chk_resetskunk->setEnabled(cmd_uploadskunk.Enabled && detected_format);
    ui.chk_uploadROM->setEnabled(cmd_uploadskunk.Enabled && detected_format);
    ui.chk_uploadbank2->setCheckState(chk_uploadbank2.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_resetskunk->setCheckState(chk_resetskunk.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_uploadROM->setCheckState(chk_uploadROM.Value ? Qt::Checked : Qt::Unchecked);

    // set the lo_inp / BJL upload capability
    ui.cmd_uploadBJL_Click->setEnabled(cmd_uploadBJL.Enabled && detected_format);
    ui.chk_8bitupl->setEnabled(cmd_uploadBJL.Enabled && detected_format);
    ui.chk_noswitch->setEnabled(cmd_uploadBJL.Enabled && detected_format);
    ui.chk_8bitupl->setCheckState(chk_8bitupl.Value ? Qt::Checked : Qt::Unchecked);
    ui.chk_noswitch->setCheckState(chk_noswitch.Value ? Qt::Checked : Qt::Unchecked);

    // set the toggle on/off capabilities
    ui.cmd_toggle->setEnabled(out[0] && (chk_coff.Enabled | chk_elf.Enabled | chk_ROM.Enabled | chk_BJL.Enabled | chk_jagr2.Enabled | chk_jagr3.Enabled | chk_ROMh.Enabled | chk_J64.Enabled));
}


// Get output path
void JiFFI2::cmd2_Click(void)
{
    // get the directory name
    QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), NULL, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (directory.size())
    {
        // append a '\' to the directory name
        strcpy_s(out, MAX_PATH, directory.toUtf8().data());
        strstr(out, "\\") ? strcat_s(out, MAX_PATH, "\\") : strcat_s(out, MAX_PATH, "/");
        // refresh the UI
        RefreshUI();
    }
}


// Get the source file and his detection format
void JiFFI2::cmd_source_Click(void)
{
    // get the filename
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select a valid input file"), "", tr("*.cof; *.elf; *.coff; *.bin; *.abs; *.jag; *.bjl; *.rom; *.j64; *.pr2; *.pr3; *.prg);; *.cof; *.coff; *.abs;; *.elf;; *.pr2; *.pr3; *.prg"));
    if (fileName.size())
    {
        // information updates
        opt_ROM.Value = opt_BJL.Value = chk_coff.Value = chk_elf.Value = chk_ROM.Value = chk_BJL.Value = chk_jagr2.Value = chk_jagr3.Value = chk_ROMh.Value = chk_J64.Value = false;
        opt_ROM.Enabled = opt_BJL.Enabled = chk_coff.Enabled = chk_elf.Enabled = chk_ROM.Enabled = chk_BJL.Enabled = chk_jagr2.Enabled = chk_jagr3.Enabled = chk_ROMh.Enabled = chk_J64.Enabled = false;
        EditorAddr = false;

        // file format detection
        strcpy_s(inp, MAX_PATH, fileName.toUtf8().data());
        int Error = detect_file_format();
        if (Error)
        {
            // display error message
            QMessageBox msgBox;
            switch (Error)
            {
            case FILE_ERROR_FILE_CANNOTREAD:      
                msgBox.setText("File cannot be read");
                break;

            case FILE_ERROR_FILE_TOOBIG:
                msgBox.setText("File size is too big to be considered as a ROM");
                break;
            }
            msgBox.exec();
            inp[0] = 0;
        }
        else
        {
            // get the output directory
            if (chk_auto_output_dir.Value)
            {
                // output directory based on the inp directory
                size_t z = strlen(inp);
                while (((inp[--z] != '/') && (inp[z] != '\\')) && z);
                memset(&out, 0, MAX_PATH);
                memcpy(&out, &inp, z ? (z + 1) : 0);
            }

            // setup following the detected format
            switch (detected_format)
            {
            case format_2MB:
                chk_elf.Enabled = chk_coff.Enabled = chk_ROM.Enabled = chk_BJL.Enabled = chk_jagr3.Enabled = chk_ROMh.Enabled = chk_J64.Enabled = true;
                opt_ROM.Enabled = opt_BJL.Value = opt_BJL.Enabled = EditorAddr = true;
                break;

            case format_6MB:
                chk_ROM.Enabled = chk_ROMh.Enabled = chk_J64.Enabled = true;
                opt_ROM.Enabled = opt_ROM.Value = EditorAddr = true;
                break;

            case format_ROMheaderless:
                chk_ROM.Enabled = chk_J64.Enabled = true;
                break;

            case format_ROMheader:
            case format_ROMUnivHeader:
                chk_ROMh.Enabled = chk_J64.Enabled = true;
                break;

            case format_JiFFIROM1:
            case format_JiFFIROM2:
                chk_elf.Enabled = chk_coff.Enabled = chk_BJL.Enabled = chk_jagr3.Enabled = chk_ROMh.Enabled = chk_J64.Enabled = true;
                break;

            case format_JAGR2:
                chk_elf.Enabled = chk_coff.Enabled = chk_ROM.Enabled = chk_BJL.Enabled = chk_jagr3.Enabled = chk_ROMh.Enabled = chk_J64.Enabled = true;
                break;

            case format_JAGR3:
                chk_elf.Enabled = chk_coff.Enabled = chk_ROM.Enabled = chk_BJL.Enabled = chk_ROMh.Enabled = chk_J64.Enabled = true;
                break;

            case format_COFF:
                chk_elf.Enabled = chk_ROM.Enabled = chk_BJL.Enabled = chk_jagr3.Enabled = chk_ROMh.Enabled = chk_J64.Enabled = true;
                break;

            case format_ELF:
                chk_coff.Enabled = chk_ROM.Enabled = chk_BJL.Enabled = chk_jagr3.Enabled = chk_ROMh.Enabled = chk_J64.Enabled = true;
                break;

            default:
                break;
            }
        }

        // refresh the UI
        RefreshUI();
    }
}


// Handle toggle on/off
void JiFFI2::cmd_toggle_Click(void)
{
    // set the toggle for each format
    chk_coff.Value = chk_coff.Enabled ? (chk_coff.Value = !chk_coff.Value) : false;
    chk_elf.Value = chk_elf.Enabled ? (chk_elf.Value = !chk_elf.Value) : false;
    chk_ROM.Value = chk_ROM.Enabled ? (chk_ROM.Value = !chk_ROM.Value) : false;
    chk_BJL.Value = chk_BJL.Enabled ? (chk_BJL.Value = !chk_BJL.Value) : false;
    chk_jagr2.Value = chk_jagr2.Enabled ? (chk_jagr2.Value = !chk_jagr2.Value) : false;
    chk_jagr3.Value = chk_jagr3.Enabled ? (chk_jagr3.Value = !chk_jagr3.Value) : false;
    chk_ROMh.Value = chk_ROMh.Enabled ? (chk_ROMh.Value = !chk_ROMh.Value) : false;
    chk_J64.Value = chk_J64.Enabled ? (chk_J64.Value = !chk_J64.Value) : false;
    // refresh the UI to display the enabled format to create
    RefreshUI();
}


// Select option as ROM
void JiFFI2::opt_ROM_Click(void)
{
    // set the availability of each format
    opt_BJL.Value = chk_coff.Enabled = chk_elf.Enabled = chk_BJL.Enabled = chk_jagr2.Enabled = chk_jagr3.Enabled = false;
    opt_ROM.Value = chk_ROM.Enabled = chk_ROMh.Enabled = chk_J64.Enabled = true;
    //ui.ed_loadadr->setEnabled(false);
    //ui.ed_runadr->setEnabled(false);
    EditorAddr = false;
    // refresh the UI
    RefreshUI();
}


// Select option as BJL
void JiFFI2::opt_BJL_Click(void)
{
    // set the availability of each format
    opt_BJL.Value = chk_coff.Enabled = chk_elf.Enabled = chk_ROM.Enabled = chk_BJL.Enabled = chk_jagr2.Enabled = chk_jagr3.Enabled = chk_ROMh.Enabled = chk_J64.Enabled = true;
    opt_ROM.Value = false;
    //ui.ed_loadadr->setEnabled(true);
    //ui.ed_runadr->setEnabled(true);
    EditorAddr = true;
    // refresh the UI
    RefreshUI();
}


// Create the export
void JiFFI2::cmd_create_Click(void)
{
    QMessageBox msgBox;
    unsigned int retVal = QMessageBox::Yes;
    //int Error = 0;
#if 0
    // check the source file
    if (!inp[0])
    {
        msgBox.setText("Source file not found");
        msgBox.exec();
    }
    else
#endif
    {
//        If !cli_run
//            out$ = lbl_out.Text
//            EndIf
#if 0
        // check the destination file
        if (!out[0])
        {
            msgBox.setText("No destination path");
            msgBox.exec();
        }
        else
#endif
        {
#if 0
            '  cmd_create.Enabled = False
                frm1.Enabled = False
                Dim a$

                '
                ' Figure out load and run addresses
                '
                a$ = ed_load.Text
                If Left$(a$, 1) < > "$" And Upper$(Left$(a$, 2)) < > "&H"
                a$ = "$" + a$
                EndIf
                loadadr = Val(a$)
                a$ = ed_run.Text
                If Left$(a$, 1) < > "$" And Upper$(Left$(a$, 2)) < > "&H"
                a$ = "$" + a$
                EndIf
                runadr = Val(a$)
                If detected_format <> format_ROMheaderless And detected_format <> format_ROMheader And(detected_format = format_undetected And opt_BJL.Value = 1)
                If loadadr < $4000 Or loadadr > $1ff000
                MsgBox "Load address must be bigger than $4000 and less than $1ff000", MB_ICONSTOP + MB_OK, "Critical error"
                cmd_create.Enabled = True
                Return
                EndIf
                If runadr < $4000 Or runadr > $1ff000
                MsgBox "Run address must be bigger than $4000 and less than $1ff000", MB_ICONSTOP + MB_OK, "Critical error"
                cmd_create.Enabled = True
                Return
                EndIf
                EndIf
#endif
                // check valid load and run address depend the detected format
                if ((detected_format != format_ROMheaderless) && (detected_format != format_ROMheader) && (detected_format == format_undetected) && (opt_BJL.Value = true))
                {
                    // check load address
                    if ((loadadr < 0x4000) || (loadadr > 0x1ff000))
                    {
                        // load address won't fit
                        msgBox.setText("Load address must be bigger than $4000 and lesser than $1ff000");
                        msgBox.exec();
                        retVal = QMessageBox::No;
                    }
                    else
                    {
                        // check run address
                        if ((runadr < 0x4000) || (runadr > 0x1ff000))
                        {
                            // run address won't fit
                            msgBox.setText("Run address must be bigger than $4000 and lesser than $1ff000");
                            msgBox.exec();
                            retVal = QMessageBox::No;
                        }
                    }
                }

                // export to the requested formats
                if (retVal != QMessageBox::No)
                {
                    GetExtraOptions();
                    save_file_format();

                    //
                    msgBox.setText("All operations have been completed");
                    msgBox.exec();
                }
#if 0
                frm1.Enabled = True
                cmd_create.Enabled = True
                cmd_source.Default = True

                If !cli_run
                MsgBox "All operations done!", MB_ICONINFORMATION + MB_OK, "Information"
                EndIf

                Return
#endif
        }
    }
    //  (ui.chk_overwrite->checkState()
}


// Handle the option selection
void JiFFI2::set_option_click(chk* PtrChk, bool click)
{
    PtrChk->Value = click;
    RefreshUI();
}


// Handle the no switch option for the BJL
void JiFFI2::chk_noswitch_click(bool click)
{
    set_option_click(&chk_noswitch, click);
}


// Handle the 8 bit upload option for the BJL
void JiFFI2::chk_8bitupl_click(bool click)
{
    set_option_click(&chk_8bitupl, click);
}


// Handle the upload to bank 2 option for the Skunkboard
void JiFFI2::chk_uploadbank2_click(bool click)
{
    set_option_click(&chk_uploadbank2, click);
}


// Handle the upload ROM option for the Skunkboard
void JiFFI2::chk_uploadROM_click(bool click)
{
    set_option_click(&chk_uploadROM, click);
}


// Handle the resetskunk option for the Skunkboard
void JiFFI2::chk_resetskunk_click(bool click)
{
    set_option_click(&chk_resetskunk, click);
}


// Handle the ELF format option
void JiFFI2::chk_ELF_click(bool click)
{
    set_option_click(&chk_elf, click);
}


// Handle the Jag server v3 format option
void JiFFI2::chk_JAGR3_click(bool click)
{
    set_option_click(&chk_jagr3, click);
}


// Handle the Jag server v2 format option
void JiFFI2::chk_JAGR2_click(bool click)
{
    set_option_click(&chk_jagr2, click);
}


// Handle the COFF / ABS format option
void JiFFI2::chk_COFFABS_click(bool click)
{
    set_option_click(&chk_coff, click);
}


// Handle the BJL format option
void JiFFI2::chk_BJL_click(bool click)
{
    set_option_click(&chk_BJL, click);
}


// Handle the ROM format option
void JiFFI2::chk_ROM_click(bool click)
{
    set_option_click(&chk_ROM, click);
}


// Handle the ROM (headerless) format option
void JiFFI2::chk_ROMh_click(bool click)
{
    set_option_click(&chk_ROMh, click);
}


// Handle the J64 format option
void JiFFI2::chk_J64_click(bool click)
{
    set_option_click(&chk_J64, click);
}


// Handle the overwrite option
void JiFFI2::chk_overwrite_click(bool click)
{
    // get the overwrite status
    chk_overwrite.Value = click;
}


// Handle the auto-output path
void JiFFI2::chk_auto_output_dir_Click(bool click)
{
    // check the state of the output directory creation
    if ((chk_auto_output_dir.Value = click))
    {
        // output directory based on the inp directory
        size_t z = strlen(inp);
        while (((inp[--z] != '/') && (inp[z] != '\\')) && z);
        memset(&out, 0, MAX_PATH);
        memcpy(&out, &inp, z ? (z + 1) : 0);
    }
    // refresh the UI
    RefreshUI();
}


// Uplocad to BJL
void JiFFI2::cmd_uploadBJL_Click(void)
{
    QMessageBox msgBox;
#if 0
    Sub cmd_uploadBJL_Click
        Dim flash$, opt$, temp$
        If detected_format = format_ROMheader Or detected_format = format_ROMheaderless Or detected_format = format_ROMUnivHeader
        MsgBox "Try as you might, ROM files won't work with BJL!", MB_ICONSTOP + MB_OK, "Critical error"
        Return
        EndIf
        If !Exist(inp$)
        MsgBox "Source file not found!", MB_ICONSTOP + MB_OK, "Error"
        Return
        EndIf
        temp$ = ed_load.Text
        If Left$(temp$, 1) < > "$" And Upper$(Left$(temp$, 2)) < > "&H"
        temp$ = "$" + temp$
        EndIf
        loadadr = Val(temp$)
        If loadadr < $4000 Or loadadr > $1ff000
        MsgBox "Load address must be bigger than $4000 and less than $1ff000", MB_ICONSTOP + MB_OK, "Critical error"
        Return
        EndIf
        If !Exist(App.Path + "\Lo_inp.exe")
        MsgBox "Lo_inp.exe not found!", MB_ICONSTOP + MB_OK, "Critical error"
        Return
        EndIf

        'Upload a homebrew (convert to BJL temporarily)
        out$ = TempFileName("")
        BMove imageadr, imagefinaladr, linj         'make a copy of the image
        dopatches
        temp$ = name$
        name$ = ""
        create_bjl                                  ' Make a BJL
        name$ = temp$
        flash$ = out$ + "_load_" + Hex$(loadadr) + "_run_" + Hex$(runadr) + "_jiffi.bjl"
        If chk_8bitupl.Value
        opt$ = "-8"
        EndIf
        If chk_noswitch.Value
        opt$ = opt$ + " -n"
        EndIf
        opt$ = opt$ + " -b " + Hex$(loadadr)
        start_process_and_wait(App.Path + "\Lo_inp.exe " + opt$ + " " + flash$)
        Kill flash$

        End Sub

#endif
#if 0
    // check the source file
    if (!inp[0])
    {
        // no source file
        msgBox.setText("Source file not found");
        msgBox.exec();
    }
    else
#endif
    {
        // check BJL rom compatibility
        if ((detected_format == format_ROMheader) || (detected_format == format_ROMheaderless) || (detected_format == format_ROMUnivHeader))
        {
            // rom not compatible with BJL
            msgBox.setText("ROM files won't work with BJL");
            msgBox.exec();
        }
        else
        {
            // check load address
            if ((loadadr < 0x4000) || (loadadr > 0x1ff000))
            {
                // load address won't fit
                msgBox.setText("Upload via BJL requires a load address bigger than $4000 and lesser than $1ff000");
                msgBox.exec();
            }
            else
            {
                // create the BJL temporary file
                memcpy(imagefinaladr, imageadr, linj);
                dopatches();
                create_bjl();
                // create the file name
                char file[MAX_PATH], buffer[10];
                sprintf_s(file, "%s%s_load_%06x_run_%06x_jiffi%i.bjl", out, name, loadadr, runadr, chk_JiFFI.Value);
                printf("\nCreate temporary file %s\n", file);
                // create the command line
                QString cmdline = "lo_inp.exe " + QString(chk_8bitupl.Value ? "-8 " : "") + QString(chk_noswitch.Value ? "-n " : "") + (QString("-b ") + QString(!_itoa_s(loadadr, buffer, 16) ? buffer : "ERROR")) + QString(" ") + QString(file);
                printf("%s\n", cmdline.toUtf8().constData());
                // execute the command line
                QProcess* process = new QProcess(this);
                process->execute(cmdline);
                // remove the BJL temporry file
                if (!remove(file))
                {
                    printf("The temporary file has been deleted successfully\n");
                }
                else
                {
                    printf("The temporary file cannot be deleted\n");
                }
                delete process;
            }
        }
    }
}


//
unsigned int JiFFI2::HandleSkunkboardBank(void)
{
    // flash a ROM file directly (create a new ROM in case there are patches to be applied)
    QMessageBox msgBox;
    QString msg;
    msg.sprintf("Please note that flashing a ROM will permanently erase your Skunkboard's bank.");
    msgBox.setText(msg);
    msgBox.setInformativeText("Erase Skunkboard bank?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    // get the user feedback
    return msgBox.exec();
}


//
void JiFFI2::HandleSkunkboardTempROM0(void)
{
    char file[MAX_PATH], cmdline[MAX_PATH];
    QProcess* process = new QProcess(this);

    // create the ROM (type 0) temporarily file
    memcpy(imagefinaladr, imageadr, linj);
    dopatches();
    create_rom(0);
    // create the file name
    sprintf_s(file, "%s%s_jiffi%i.rom", out, name, chk_JiFFI.Value);
    printf("\nCreate temporary file %s\n", file);
    // create the command line
    sprintf_s(cmdline, "%s -f %s %s", JCPApp, (chk_uploadbank2.Value ? "-2" : ""), file);
    printf("%s\n", cmdline);
    // execute the command line
    process->execute(cmdline);
    // remove the COFF temporry file
    if (!remove(file))
    {
        printf("The temporary file has been deleted successfully\n");
    }
    else
    {
        printf("The temporary file cannot be deleted\n");
    }
}


// Upload to Skunkboard
void JiFFI2::cmd_uploadskunk_Click(void)
{
    QMessageBox msgBox;
    char cmdline[MAX_PATH];
    QProcess* process = new QProcess(this);
    unsigned int retVal;
#if 0
    Sub cmd_uploadskunk_Click
        Dim flash$, temp$, bank2$

        If !Exist(inp$)
        MsgBox "Source file not found!", MB_ICONSTOP + MB_OK, "Error"
        Return
        EndIf
        If !Exist(App.Path + "\jcp2.exe")
        MsgBox "jcp2.exe not found!", MB_ICONSTOP + MB_OK, "Critical error"
        Return
        EndIf

        If chk_uploadbank2.Value = True
        bank2$ = " -2"
        EndIf

        If chk_resetskunk.Value
        start_process_and_wait(App.Path + "\jcp2.exe -r")
        EndIf


        If detected_format = format_ROMheader Or detected_format = format_ROMheaderless Or detected_format = format_ROMUnivHeader
            ' Flash a ROM file directly (create a new ROM in case there are patches to be applied)
            If MsgBox("Please note that flashing a ROM" + Chr$(13) + "will permanently erase your Skunkboard's bank." + Chr$(13) + "Press Yes only if you're SURE you understand this!", MB_ICONSTOP + MB_YESNO, "Erase Skunkboard bank?") = IDNO
            Return
            EndIf

            out$ = TempFileName("")
            BMove imageadr, imagefinaladr, linj         'make a copy of the image
            dopatches
            temp$ = name$
            name$ = ""
            convert_rom(0)                              ' Make a ROM with universal header from homebrew
            name$ = temp$
            flash$ = out$ + "_jiffi.rom"
            start_process_and_wait(App.Path + "\jcp2.exe -f " + bank2$ + flash$)
            Kill flash$
        ElseIf chk_uploadROM.Value Or(detected_format = format_undetected And opt_ROM.Value = True)
            ' Flash a homebrew as ROM (conversion to ROM temporarily)
            If MsgBox("Please note that flashing a ROM" + Chr$(13) + "will permanently erase your Skunkboard's" + Chr$(13) + "bank. Press Yes only if you're SURE you understand this!", MB_ICONSTOP + MB_YESNO, "Erase Skunkboard bank?") = IDNO
            Return
            EndIf

            '
            ' Figure out load and run addresses
            '
            temp$ = ed_load.Text
            If Left$(temp$, 1) < > "$" And Upper$(Left$(temp$, 2)) < > "&H"
            temp$ = "$" + temp$
            EndIf
            loadadr = Val(temp$)
            temp$ = ed_run.Text
            If Left$(temp$, 1) < > "$" And Upper$(Left$(temp$, 2)) < > "&H"
            temp$ = "$" + temp$
            EndIf
            runadr = Val(temp$)
            If detected_format <> format_ROMheaderless And detected_format <> format_ROMheader And(detected_format = format_undetected And opt_BJL.Value = 1)
                If loadadr < $4000 Or loadadr > $1ff000
                MsgBox "Load address must be bigger than $4000 and less than $1ff000", MB_ICONSTOP + MB_OK, "Critical error"
                cmd_create.Enabled = True
                Return
                EndIf
                If runadr < $4000 Or runadr > $1ff000
                MsgBox "Run address must be bigger than $4000 and less than $1ff000", MB_ICONSTOP + MB_OK, "Critical error"
                cmd_create.Enabled = True
                Return
                EndIf
           EndIf


            out$ = TempFileName("")
            BMove imageadr, imagefinaladr, linj         'make a copy of the image
            dopatches
            temp$ = name$
            name$ = ""
            create_rom(0)                               ' Make a ROM with universal header from homebrew
            name$ = temp$
            flash$ = out$ + "_jiffi.rom"
            start_process_and_wait(App.Path + "\jcp2.exe -f " + bank2$ + flash$)
            Kill flash$
        Else
            'Upload a homebrew (convert to COFF temporarily)
            out$ = TempFileName("")
            BMove imageadr, imagefinaladr, linj         'make a copy of the image
            dopatches
            temp$ = name$
            name$ = ""
            create_coff                                 ' Make a COFF to assist jcp
            name$ = temp$
            flash$ = out$ + "_jiffi.coff"
            start_process_and_wait(App.Path + "\jcp2.exe " + flash$)
            Kill Yes, flash$
        EndIf


        End Sub
#endif
#if 0
    // check the source file
    if (!inp[0])
    {
        msgBox.setText("Source file not found");
        msgBox.exec();
    }
    else
#endif
    {
        // reset the Skunkboard
        if (chk_resetskunk.Value)
        {
            // create the command line
            sprintf_s(cmdline, "%s -r", JCPApp);
            printf("%s\n", cmdline);
            // execute the command line
            process->execute(cmdline);
        }

        if ((detected_format == format_ROMheader) || (detected_format == format_ROMheaderless) || (detected_format == format_ROMUnivHeader))
        {
            retVal = HandleSkunkboardBank();
            if (retVal != QMessageBox::No)
            {
                // send the ROM to Skunkboard
                HandleSkunkboardTempROM0();
            }
        }
        else
        {
            if (chk_uploadROM.Value || ((detected_format == format_undetected) && opt_ROM.Value == (BOOL)true))
            {
                retVal = HandleSkunkboardBank();
                if (retVal != QMessageBox::No)
                {
                    if ((detected_format != format_ROMheaderless) && (detected_format != format_ROMheader) && ((detected_format == format_undetected) && (opt_BJL.Value == (BOOL)true)))
                    {
                        // check load address
                        if ((loadadr < 0x4000) || (loadadr > 0x1ff000))
                        {
                            // load address won't fit
                            msgBox.setText("Load address must be bigger than $4000 and lesser than $1ff000");
                            msgBox.exec();
                            retVal = QMessageBox::No;
                        }
                        else
                        {
                            if ((runadr < 0x4000) || (runadr > 0x1ff000))
                            {
                                // run address won't fit
                                msgBox.setText("Run address must be bigger than $4000 and lesser than $1ff000");
                                msgBox.exec();
                                retVal = QMessageBox::No;
                            }
                        }
                    }

                    // send the ROM to Skunkboard
                    if (retVal != QMessageBox::No)
                    {
                        HandleSkunkboardTempROM0();
                    }
                }
            }
            else
            {
                // create the COFF temporarily file
                memcpy(imagefinaladr, imageadr, linj);
                dopatches();
                create_coff();
                // create the file name
                char file[MAX_PATH];
                sprintf_s(file, "%s%s_jiffi%i.coff", out, name, chk_JiFFI.Value);
                printf("\nCreate temporary file %s\n", file);
                // create the command line
                sprintf_s(cmdline, "%s %s", JCPApp, file);
                printf("%s\n", cmdline);
                // execute the command line
                process->execute(cmdline);
                // remove the COFF temporry file
                if (!remove(file))
                {
                    printf("The temporary file has been deleted successfully\n");
                }
                else
                {
                    printf("The temporary file cannot be deleted\n");
                }
            }
        }
    }

    delete process;
}


// Get extra options selection
void JiFFI2::GetExtraOptions(void)
{
    PatchOptions[0].selected = (ui_extraopts.checkBox->checkState() == Qt::Unchecked) ? false : true;
    PatchOptions[1].selected = (ui_extraopts.checkBox_2->checkState() == Qt::Unchecked) ? false : true;
    PatchOptions[2].selected = (ui_extraopts.checkBox_3->checkState() == Qt::Unchecked) ? false : true;
    PatchOptions[3].selected = (ui_extraopts.checkBox_4->checkState() == Qt::Unchecked) ? false : true;
    PatchOptions[4].selected = (ui_extraopts.checkBox_5->checkState() == Qt::Unchecked) ? false : true;
}


// Handle the title extra options
void JiFFI2::cmd_extraopts_Click(void)
{
    // check detected title change
    if (CurrentDetectedTitle != DetectedTitle)
    {
        CurrentDetectedTitle = DetectedTitle;
#if 1
        // display the captions
        printf("\n");
        for (unsigned int i = 0; i < PatchesForCurrentTitle; i++)
        {
            printf("%i - %s\n", PatchOptions[i].index, Patches[PatchOptions[i].index].caption);
        }
#endif
        // checkboxes initialisations
        ui_extraopts.checkBox->setText(QString::fromUtf8(Patches[PatchOptions[0].index].caption));
        ui_extraopts.checkBox->setCheckState(Qt::Unchecked);
        ui_extraopts.checkBox_2->setVisible(PatchOptions[1].index);
        ui_extraopts.checkBox_2->setText(QString::fromUtf8(Patches[PatchOptions[1].index].caption));
        ui_extraopts.checkBox_2->setCheckState(Qt::Unchecked);
        ui_extraopts.checkBox_3->setVisible(PatchOptions[2].index);
        ui_extraopts.checkBox_3->setText(QString::fromUtf8(Patches[PatchOptions[2].index].caption));
        ui_extraopts.checkBox_3->setCheckState(Qt::Unchecked);
        ui_extraopts.checkBox_4->setVisible(PatchOptions[3].index);
        ui_extraopts.checkBox_4->setText(QString::fromUtf8(Patches[PatchOptions[3].index].caption));
        ui_extraopts.checkBox_4->setCheckState(Qt::Unchecked);
        ui_extraopts.checkBox_5->setVisible(PatchOptions[4].index);
        ui_extraopts.checkBox_5->setText(QString::fromUtf8(Patches[PatchOptions[4].index].caption));
        ui_extraopts.checkBox_5->setCheckState(Qt::Unchecked);
    }
    // show the UI
    JiFFI2_ExtraOpts->show();
}


// Handle the load address
void JiFFI2::set_loadadr(void)
{
    QMessageBox msgBox;
    bool ok;

    // get load address value
    unsigned int _loadadr = ui.ed_loadadr->text().toUInt(&ok, 16);
    // check the change in the address
    if (_loadadr != loadadr)
    {
        // check address validity
        if (ok && (_loadadr >= 0x400) && (_loadadr < 0xe00000))
        {
            // check address alignement
            if (_loadadr & 1)
            {
                // alignement not correct
                msgBox.setText("Address cannot be odd and must be aligned");
                msgBox.exec();
            }
            else
            {
                // addresse correction
                loadadr = _loadadr;
            }
        }
        else
        {
            // address doesn't fit
            msgBox.setText("Address must be bigger than $400 and lesser than $e00000");
            msgBox.exec();
        }

        // refresh UI to display only the address
        ui.ed_loadadr->setText(QString::fromLatin1("%1").arg(QString::number(loadadr, 16)));
        //RefreshUI();
    }
}


// Handle the run address
void JiFFI2::set_runadr(void)
{
    QMessageBox msgBox;
    bool ok;

    // get executable address value
    unsigned int _runadr = ui.ed_runadr->text().toUInt(&ok, 16);
    // check the change in the address
    if (_runadr != runadr)
    {
        // check address validity
        if (ok && (_runadr >= 0x400) && (_runadr < 0xe00000))
        {
            // check address alignement
            if (_runadr & 1)
            {
                // alignement not correct
                msgBox.setText("Address cannot be odd and must be aligned");
                msgBox.exec();
            }
            else
            {
                // address correction
                runadr = _runadr;
            }
        }
        else
        {
             // address doesn't fit
             msgBox.setText("Address must be bigger than $400 and lesser than $e00000");
             msgBox.exec();
        }

        // refresh UI to display only the address
        ui.ed_runadr->setText(QString::fromLatin1("%1").arg(QString::number(runadr, 16)));
        //RefreshUI();
    }
}


JiFFI2::~JiFFI2(void)
{
    delete JiFFI2_ExtraOpts;
    delete JiFFI2_About;
}
