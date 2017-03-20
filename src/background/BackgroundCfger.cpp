#include "BackgroundCfger.h"
#include "ui_BackgroundCfger.h"
#include <QStringList>
#include <QDir>
#include "FreestyleDialog.h"

BackgroundCfger::BackgroundCfger(BackgroundAttr *attr_) :
    ui(new Ui::BackgroundCfger),
    attr{attr_}
{
	ui->setupUi(this);

    QStringList styleList;
    styleList << BackgroundCfger::tr("Cirque") << BackgroundCfger::tr("FivePointedStar")
              << BackgroundCfger::tr("Babysbreath") << BackgroundCfger::tr("Snow")
              << BackgroundCfger::tr("Arrow") << BackgroundCfger::tr("SideswaySlipLine")
              << BackgroundCfger::tr("FaceToFaceSlipLine") << BackgroundCfger::tr("Diamond")
              << BackgroundCfger::tr("Block") << BackgroundCfger::tr("Circle")
              << BackgroundCfger::tr("CrewelScan") << BackgroundCfger::tr("SingleScan");
    ui->comboBoxStyle->addItems(styleList);

    ui->comboBoxStyle->setCurrentIndex(attr->style);
    ui->spinBoxSpeed->setValue(attr->speed);
    this->setChecked(attr->checked);
    if (attr->checked) styleChanged(ui->comboBoxStyle->currentIndex());

    ui->radioButtonClassic->hide();
    ui->radioButtonFreestyle->hide();
    ui->radioButtonSlide->hide();

    selectRadios.addButton(ui->radioButtonClassic);
    selectRadios.addButton(ui->radioButtonFreestyle);
    selectRadios.addButton(ui->radioButtonSlide);

    if (attr->select == 0) ui->radioButtonClassic->setChecked(true);
    else if (attr->select == 1) ui->radioButtonFreestyle->setChecked(true);
    else if (attr->select == 2) ui->radioButtonSlide->setChecked(true);

    connect(&selectRadios, static_cast<void (QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),
            this, &BackgroundCfger::selectMode);

    connect(ui->comboBoxStyle,
            static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
            this, &BackgroundCfger::styleChanged);
    connect(ui->spinBoxSpeed,
            static_cast<void (QSpinBox::*)(int value)>(&QSpinBox::valueChanged),
            this, &BackgroundCfger::speedChanged);
    connect(this, &QGroupBox::toggled, this, &BackgroundCfger::backgourndChecked);
}

BackgroundCfger::~BackgroundCfger()
{
	delete ui;
}

void BackgroundCfger::backgourndChecked(bool check)
{
    attr->checked = check;
    if (attr->checked)
        styleChanged(ui->comboBoxStyle->currentIndex());
    else {
        attr->notify.changed();
    }
}

void BackgroundCfger::styleChanged(int index)
{
   attr->style = index;

   attr->notify.changed();
}

void BackgroundCfger::speedChanged(int speed)
{
    attr->speed = speed;
}

void BackgroundCfger::selectMode(QAbstractButton *button)
{
    if (button == ui->radioButtonClassic) {
        ui->radioButtonClassic->setChecked(true);
        attr->select = 0;
    } else if (button == ui->radioButtonFreestyle) {
        ui->radioButtonFreestyle->setChecked(true);
        attr->select = 1;
        FreestyleDialog dialog(this);
        dialog.exec();
    } else if (button == ui->radioButtonSlide) {
        ui->radioButtonSlide->setChecked(true);
        attr->select = 2;
    }
}
