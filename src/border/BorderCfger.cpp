#include "BorderCfger.h"
#include "ui_BorderCfger.h"

#include <QDir>

BorderCfger::BorderCfger(BorderAttr *attr) :
    ui(new Ui::BorderCfger),
    attr{attr}
{
    ui->setupUi(this);

    ui->comboBoxDirection->addItem(BorderCfger::tr("Fixed"));
    ui->comboBoxDirection->addItem(BorderCfger::tr("CW"));
    ui->comboBoxDirection->addItem(BorderCfger::tr("CCW"));
    ui->comboBoxDirection->addItem(BorderCfger::tr("Flicker"));
    ui->comboBoxDirection->addItem(BorderCfger::tr("Flicker + CW"));
    ui->comboBoxDirection->addItem(BorderCfger::tr("Flicker + CCW"));

    updataBorderList(attr->borderDir);

    ui->comboBoxStyle->setCurrentIndex(attr->style);
    ui->spinBoxSpeed->setValue(attr->speed);
    ui->comboBoxDirection->setCurrentIndex(attr->direct);

    this->setChecked(attr->checked);
    if (attr->checked) styleChanged(ui->comboBoxStyle->currentIndex());

    connect(ui->comboBoxStyle,
            static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
            this, &BorderCfger::styleChanged);
    connect(ui->spinBoxSpeed,
            static_cast<void (QSpinBox::*)(int value)>(&QSpinBox::valueChanged),
            this, &BorderCfger::speedChanged);
    connect(ui->comboBoxDirection,
            static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
            this, &BorderCfger::directChanged);
    connect(this, &QGroupBox::toggled, this, &BorderCfger::borderChecked);
}

BorderCfger::~BorderCfger()
{
    delete ui;
}

int BorderCfger::updataBorderList(const QString &path)
{
    QDir dir(path);
    QFileInfoList list;

    if (!dir.exists()) return -1;

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    list = dir.entryInfoList();

    ui->comboBoxStyle->blockSignals(true);
    for (int i = ui->comboBoxStyle->count(); i >= 0 ; --i)
        ui->comboBoxStyle->removeItem(i);
    ui->comboBoxStyle->blockSignals(false);

    int fileCnt = list.count();
    if (fileCnt <= 0) return -2;

    //QStringList strlist;
    QFileInfo file;
    QString suffix;
    ui->comboBoxStyle->blockSignals(true);
    for (int i = 0; i < fileCnt; ++i) {
        file = list.at(i);
        suffix = file.suffix();
        if (QString::compare(suffix, QString("bmp"), Qt::CaseInsensitive) == 0
          || QString::compare(suffix, QString("png"), Qt::CaseInsensitive) == 0
          || QString::compare(suffix, QString("jpg"), Qt::CaseInsensitive) == 0) {
            ui->comboBoxStyle->addItem(QIcon(QPixmap(file.filePath())),
                                       QString::number(i + 1), QVariant(file.filePath()));
        }
    }
    ui->comboBoxStyle->blockSignals(false);

    return 0;
}

void BorderCfger::styleChanged(int index)
{
   QMatrix matrix;

   attr->step = 0;
   attr->style = index;

   //attr->borderPath = "image/border/border_" + QString("%1").arg(style.toInt(), 3, 10, QChar('0')) + ".bmp";
   attr->borderPath = ui->comboBoxStyle->itemData(index).toString();
   attr->image = std::move(QImage(attr->borderPath));
   attr->pixUp = std::move(QPixmap::fromImage(attr->image));
   attr->pixRight = std::move(QPixmap::fromImage(attr->image).transformed(matrix.rotate(90)));
   attr->pixDown = std::move(QPixmap::fromImage(attr->image).transformed(matrix.rotate(90)));
   attr->pixLeft = std::move(QPixmap::fromImage(attr->image).transformed(matrix.rotate(90)));
   attr->notify.changed();
}

void BorderCfger::borderChecked(bool check)
{
    attr->checked = check;
    if (attr->checked)
        styleChanged(ui->comboBoxStyle->currentIndex());
    else {
        attr->step = 0;
        attr->notify.changed();
    }
}

void BorderCfger::speedChanged(int speed)
{
    attr->speed = speed;
}

void BorderCfger::directChanged(int direct)
{
    attr->direct = direct;
}

