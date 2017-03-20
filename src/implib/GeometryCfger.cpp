#include "GeometryCfger.h"
#include "ui_GeometryCfger.h"

GeometryCfger::GeometryCfger(QWidget *parent) :
	QGroupBox{parent},
	ui(new Ui::GeometryCfger)
{
	ui->setupUi(this);
}

GeometryCfger::~GeometryCfger()
{
	boxRectConn.disconnect();
	delete ui;
}

void GeometryCfger::init(RegionAttr* attr_)
{
	attr = attr_;

	load();
	auto slot = [this](){load();};
	attr->geometry.connect(slot);
	boxRectConn = attr->box->contentRect().connect(slot);

	connect(ui->x, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &GeometryCfger::xChanged);
	connect(ui->y, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &GeometryCfger::yChanged);
	connect(ui->width, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &GeometryCfger::widthChanged);
	connect(ui->height, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &GeometryCfger::heightChanged);
}

void GeometryCfger::load()
{
	ui->x->blockSignals(true);
	ui->y->blockSignals(true);
	ui->width->blockSignals(true);
	ui->height->blockSignals(true);

	const KRect& boxRect = attr->box->contentRect().ref();
	const KRect& geometry = attr->geometry.ref();

	int max;
	int min;

	max = boxRect.rightReal() - geometry.size->width();
	if (ui->x->maximum() != max) ui->x->setMaximum(max);
	min = boxRect.point->x();
	if (ui->x->minimum() != min) ui->x->setMinimum(min);
	ui->x->setValue(geometry.point->x());

	max = boxRect.bottomReal() - geometry.size->height();
	if (ui->y->maximum() != max) ui->y->setMaximum(max);
	min = boxRect.point->y();
	if (ui->y->minimum() != min) ui->y->setMinimum(min);
	ui->y->setValue(geometry.point->y());

	max = boxRect.rightReal() - geometry.point->x();
	if (ui->width->maximum() != max) ui->width->setMaximum(max);
	ui->width->setValue(geometry.size->width());

	max = boxRect.bottomReal() - geometry.point->y();
	if (ui->height->maximum() != max) ui->height->setMaximum(max);
	ui->height->setValue(geometry.size->height());

	ui->x->blockSignals(false);
	ui->y->blockSignals(false);
	ui->width->blockSignals(false);
	ui->height->blockSignals(false);
}

void GeometryCfger::save()
{
	attr->geometry->setRect(ui->x->value(), ui->y->value(), ui->width->value(), ui->height->value());
}

void GeometryCfger::xChanged(int x)
{
	ui->width->setMaximum(attr->box->contentRect()->rightReal() - x);
	save();
}

void GeometryCfger::yChanged(int y)
{
	ui->height->setMaximum(attr->box->contentRect()->bottomReal() - y);
	save();
}

void GeometryCfger::widthChanged(int width)
{
	ui->x->setMaximum(attr->box->contentRect()->rightReal() - width);
	save();
}

void GeometryCfger::heightChanged(int height)
{
	ui->y->setMaximum(attr->box->contentRect()->bottomReal() - height);
	save();
}

