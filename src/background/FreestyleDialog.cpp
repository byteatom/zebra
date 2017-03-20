#include "FreestyleDialog.h"
#include "ui_FreestyleDialog.h"

FreestyleDialog::FreestyleDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FreestyleDialog)
{
	ui->setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	connect(ui->pushButtonCancel, &QPushButton::clicked, this, &QDialog::accept);
}

FreestyleDialog::~FreestyleDialog()
{
	delete ui;
}
