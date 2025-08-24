#pragma once

#include <QDialog>
#include "ui_about.h"

class about : public QDialog
{
	Q_OBJECT

public:
	explicit about(QWidget* parent = nullptr);
	~about();

private:
	Ui::about* ui;
};
