#include "ColorWidget.h"
#include "ui_ColorWidget.h"
#include "KGMarco.h"

ColorWidget::ColorWidget(int *color, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorWidget)
{
    ui->setupUi(this);
    ui->pushButton_1->setStyleSheet("background-color: rgb(255, 0, 0);");
    ui->pushButton_2->setStyleSheet("background-color: rgb(0, 255, 0);");
    ui->pushButton_3->setStyleSheet("background-color: rgb(255, 255, 0);");
    ui->pushButton_4->setStyleSheet("background-color: rgb(0, 0, 255);");
    ui->pushButton_5->setStyleSheet("background-color: rgb(0, 255, 255);");
    ui->pushButton_6->setStyleSheet("background-color: rgb(255, 0, 255);");
    ui->pushButton_7->setStyleSheet("background-color: rgb(255, 255, 255);");
    ui->pushButton_8->setStyleSheet("background-color: rgb(128, 128, 128);");
    ui->pushButton_8->setText("?");

    connect(ui->pushButton_1, &QPushButton::clicked, this, [this, color](){ *color = KG_COLOR_RED; close(); });
    connect(ui->pushButton_2, &QPushButton::clicked, this, [this, color](){ *color = KG_COLOR_GREEN; close(); });
    connect(ui->pushButton_3, &QPushButton::clicked, this, [this, color](){ *color = KG_COLOR_YELLOW; close(); });
    connect(ui->pushButton_4, &QPushButton::clicked, this, [this, color](){ *color = KG_COLOR_BLUE; close(); });
    connect(ui->pushButton_5, &QPushButton::clicked, this, [this, color](){ *color = KG_COLOR_CYAN; close(); });
    connect(ui->pushButton_6, &QPushButton::clicked, this, [this, color](){ *color = KG_COLOR_MAGENTA; close(); });
    connect(ui->pushButton_7, &QPushButton::clicked, this, [this, color](){ *color = KG_COLOR_WHITE; close(); });
    connect(ui->pushButton_8, &QPushButton::clicked, this, [this, color](){ *color = KG_COLOR_RANDOM; close(); });
}

ColorWidget::~ColorWidget()
{
    delete ui;
}

void ColorWidget::closeEvent(QCloseEvent *e)
{
    parentWidget()->close();
}
