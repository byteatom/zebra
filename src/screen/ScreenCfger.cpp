#include "ScreenCfger.h"
#include "ui_ScreenCfger.h"

ScreenCfger::ScreenCfger(ScreenAttr *attr):
	Cfger(attr),
	ui(new Ui::ScreenCfger),
	attr{attr}
{
	ui->setupUi(this);
}

ScreenCfger::~ScreenCfger()
{
	delete ui;
}

void ScreenCfger::updateDispInfo(DispInfo &info)
{
	ui->labelCardModel->setText(info.cardModel);
	ui->labelScreenWidth->setText(QString::number(info.screenWidth));
	ui->labelScreenHeight->setText(QString::number(info.screenHeight));
	if (info.baseColor == IScreen::ONE)
		ui->labelModuleColor->setText(tr("One primary color"));
	else if (info.baseColor == IScreen::TWO)
		ui->labelModuleColor->setText(tr("Two primary colors"));
	else if (info.baseColor == IScreen::THREE)
		ui->labelModuleColor->setText(tr("Three primary colors"));
	else
		ui->labelModuleColor->setText(tr("Invalid color mode"));
	ui->labelCommunication->setText(info.communication);
	ui->labelPort->setText(info.port);
}
