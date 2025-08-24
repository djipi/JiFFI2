#pragma once

#include <QDialog>
#include <qcheckbox.h>
#include "ui_extraopts.h"

class extraopts : public QDialog
{
	Q_OBJECT

public:
	explicit extraopts(QWidget* parent = nullptr);
	//void InsertCheck(char* Name);
	//void RemoveAllChecks();
	~extraopts();

//public:
	//unsigned int Status;
	//unsigned int NbExtraOpts;
	//QCheckBox* CheckBox[10];

//private slots:
	//void CloseOk_click();
	//void close();

private:
	Ui::extraopts* ui;  // pointeur ui
};
