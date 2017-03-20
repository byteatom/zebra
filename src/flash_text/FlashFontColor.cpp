#include "FlashFontColor.h"
#include "ui_FlashFontColor.h"

FlashFontColor::FlashFontColor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlashFontColor)
{
    ui->setupUi(this);
}

FlashFontColor::~FlashFontColor()
{
    delete ui;
}
