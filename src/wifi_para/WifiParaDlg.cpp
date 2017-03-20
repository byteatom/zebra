#include "WifiParaDlg.h"
#include "ui_WifiParaDlg.h"

#include <stdint.h>

#include <QStorageInfo>
#include <QMessageBox>
#include <QFile>

#include "UtilWin.h"

#include "SsidValidator.h"
#include "PasswordValidator.h"

WifiParaDlg::WifiParaDlg(QWidget *parent) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint),
	ui(new Ui::WifiParaDlg)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	ui->ssid->setValidator(new SsidValidator{ui->ssid});
	ui->password->setValidator(new PasswordValidator{ui->password});
	ui->save->setEnabled(false);

	timer.setInterval(1000);
	connect(&timer, &QTimer::timeout, this, &WifiParaDlg::setUsbList);
	timer.start();

	connect(ui->save, &QPushButton::clicked, this, &WifiParaDlg::save);
}

WifiParaDlg::~WifiParaDlg()
{
	delete ui;
}

void WifiParaDlg::setUsbList()
{
	ui->usbList->clear();
	QList<QStorageInfo> drives = QStorageInfo::mountedVolumes();
	for (QStorageInfo& info: drives) {
		QString rootPath = info.rootPath();
		rootPath.chop(1);
		std::wstring letters = rootPath.toStdWString();
		if (!UtilWin::isUsbDrive(letters[0])) continue;
		//QString name = info.name();
		ui->usbList->addItem(rootPath/*.append(name)*/);
	}
	ui->save->setEnabled(ui->usbList->count() > 0);
}

void WifiParaDlg::save()
{
	QByteArray ssid = ui->ssid->text().toUtf8();
	if (ssid.isEmpty()) {
		QMessageBox::warning(this, tr("Warning"),
					tr("The SSID is empty"));
		return;
	}
	QByteArray password = ui->password->text().toUtf8();
	if (password.size() < 8) {
		QMessageBox::warning(this, tr("Warning"),
					tr("The password should contain at least 8 printable ASCII charactors"));
		return;
	}

#pragma pack(push, 1)
	struct WifiFile{
		char magic[3] = {'W', 'I', 'F'};
		uint8_t version = 1;
		uint32_t length = 0;
		uint8_t reserved[2] = {0, 0};
		char ssid[64];
		char password[64];
	} wifiFile;
#pragma pack(pop)
	wifiFile.length = sizeof(WifiFile);

	memcpy(wifiFile.ssid, ssid.data(), ssid.size());
	wifiFile.ssid[ssid.size()] = 0;
	memcpy(wifiFile.password, password.data(), password.size());
	wifiFile.password[password.size()] = 0;

	QFile file{ui->usbList->currentText().append("/ALL.wif")};
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		QMessageBox::warning(this, tr("Warning"),
					tr("Failed to write file"));
		return;
	}

	if (sizeof(wifiFile) != file.write((const char*)&wifiFile, sizeof(wifiFile))) {
		QMessageBox::warning(this, tr("Warning"),
					tr("Failed to write file"));
		return;
	}
	ui->status->setText(tr("Success"));
}
