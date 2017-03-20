#include "ImportInfo.h"
#include "ui_ImportInfo.h"

ImportInfo::ImportInfo(QWidget *parent, const QString& info) :
	QDialog(parent),
	ui(new Ui::ImportInfo)
{
	ui->setupUi(this);

	ui->info->setPlainText(info);

	connect(ui->copy, &QPushButton::clicked, this, &ImportInfo::copy);
	connect(ui->close, &QPushButton::clicked, this, &ImportInfo::reject);
}

ImportInfo::~ImportInfo()
{
	delete ui;
}

void ImportInfo::copy()
{
	ui->info->selectAll();
	ui->info->copy();
}
