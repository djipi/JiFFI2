#pragma once

#include <QDialog>
#include <qcheckbox.h>

class extraopts : public QDialog
{
	Q_OBJECT

public:
	extraopts(QWidget* parent);
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
};
