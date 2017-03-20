#include "RegionCfger.h"
#include "ui_RegionCfger.h"

RegionCfger::RegionCfger(RegionAttr *attr) :
	Cfger(attr),
	ui(new Ui::RegionCfger),
	attr{attr}
{
	ui->setupUi(this);

	ui->geometry->init(attr);
}

RegionCfger::~RegionCfger()
{
	delete ui;
}

void RegionCfger::appendBorder()
{
	append(attr->border->newCfger());
}

void RegionCfger::appendGlow()
{
	append(attr->glow->newCfger());
}
