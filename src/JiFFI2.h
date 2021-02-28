#pragma once

#include "common.h"
#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_JiFFI2.h"

class JiFFI2 : public QMainWindow
{
    Q_OBJECT

    public:
        JiFFI2(QWidget *parent = Q_NULLPTR);
        ~JiFFI2(void);

    private slots:
        void cmd_source_Click(void);
        void cmd2_Click(void);
        void cmd_toggle_Click(void);
        void cmd_create_Click(void);
        void cmd_uploadskunk_Click(void);
        void cmd_uploadBJL_Click(void);
        void cmd_about_Click(void);
        void cmd_resetsettings_Click(void);
        void cmd_extraopts_Click(void);
        void opt_ROM_Click(void);
        void opt_BJL_Click(void);
        void chk_auto_output_dir_Click(bool click);
        void chk_overwrite_click(bool click);
        void chk_J64_click(bool click);
        void chk_ROM_click(bool click);
        void chk_ROMh_click(bool click);
        void chk_COFFABS_click(bool click);
        void chk_BJL_click(bool click);
        void chk_ELF_click(bool click);
        void chk_JAGR2_click(bool click);
        void chk_JAGR3_click(bool click);
        void chk_resetskunk_click(bool click);
        void chk_uploadROM_click(bool click);
        void chk_uploadbank2_click(bool click);
        void chk_8bitupl_click(bool click);
        void chk_noswitch_click(bool click);
        void set_loadadr(void);
        void set_runadr(void);

    private:
        Ui::JiFFI2Class ui;

    private:
        void set_option_click(chk* PtrChk, bool click);
        void GetExtraOptions(void);
        unsigned int HandleSkunkboardBank(void);
        void HandleSkunkboardTempROM0(void);
        void RefreshUI(void);
};
