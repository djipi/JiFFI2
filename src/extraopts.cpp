#include "extraopts.h"
#include "common.h"

extraopts::extraopts(QWidget* parent) : QDialog(parent)
, ui(new Ui::extraopts)
//Status(0)
{
	//ui->setupUi(this);

	//NbExtraOpts = Status = 0;

	//parent->objectName();

	//CheckBox[NbExtraOpts++] = new QCheckBox("");
	//CheckBox[NbExtraOpts++] = new QCheckBox("");
}

// insert check
//void extraopts::InsertCheck(char *Name)
//{
	//CheckBox[NbExtraOpts++] = new QCheckBox("a", this);
	//CheckBox[NbExtraOpts++] = new QCheckBox("b", this);

	//CheckBox[0]->setObjectName(Name);
	//CheckBox[1]->setObjectName(Name);
//}

// remove all checks
//void extraopts::RemoveAllChecks()
//{
//	while (NbExtraOpts--)
//	{
//		delete CheckBox[NbExtraOpts];
//	}
//}

// Close the Ok button
//void extraopts::close()
//{
//	
//	Status = 1;
//}

// Close the Ok button
//void extraopts::CloseOk_click()
//{
//	Status = 2;
//	close();
//}

extraopts::~extraopts()
{
	delete ui;
}
