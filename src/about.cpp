#include "about.h"

about::about(QWidget* parent) : QDialog(parent)
, ui(new Ui::about)
{
	ui->setupUi(this);
}

about::~about()
{
	delete ui;
}
