#include "SerialCfger.h"
#include "ui_SerialCfger.h"

SerialCfger::SerialCfger(Serial* serial_):
	ui(new Ui::SerialCfger),
	serial{serial_}
{
	ui->setupUi(this);

	connect(ui->ports, &QComboBox::currentTextChanged, [this](const QString& text) {
		serial->port = text;
	});

	QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
	for (QSerialPortInfo& info: infos) {
		ui->ports->addItem(info.portName());
	}

	ui->ports->setCurrentText(serial->port);
}

SerialCfger::~SerialCfger()
{
	delete ui;
}
