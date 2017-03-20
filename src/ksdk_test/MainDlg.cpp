#include "MainDlg.h"
#include "ui_MainDlg.h"

#include <QFileDialog>
#include <QSerialPortInfo>

Q_DECLARE_METATYPE(KsdkResult)

MainDlg* dlg = nullptr;

extern "C" {

void ksdk_SendResult(KsdkResult result)
{
	emit dlg->ksdkSendResultSig(result);
}

void ksdk_SendProgress(const unsigned int max,
						const unsigned int value)
{
	emit dlg->ksdkSendProgressSig(max, value);
}

void ksdk_PlayResult(KsdkResult result)
{
	emit dlg->ksdkPlayResultSig(result);
}

}

MainDlg::MainDlg(QWidget *parent) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint),
	ui(new Ui::MainDlg)
{
	dlg = this;
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	ui->setupUi(this);

	qRegisterMetaType<KsdkResult>();
	connect(this, &MainDlg::ksdkSendResultSig, this, &MainDlg::ksdkSendResult);
	connect(this, &MainDlg::ksdkSendProgressSig, this, &MainDlg::ksdkSendProgress);
	connect(this, &MainDlg::ksdkPlayResultSig, this, &MainDlg::ksdkPlayResult);

	connect(ui->selectFiles, &QPushButton::clicked, [this]() {
		QStringList pathList = QFileDialog::getOpenFileNames(
								this,
								"Select Image Files",
								"",
								"Images (*.png *.bmp *.jpg)",
								nullptr,
								QFileDialog::ReadOnly);
		if (pathList.isEmpty()) return;

		log(QString("selected image files for program %1:").arg(imagePaths.size()));
		std::vector<std::wstring> paths;
		for (const QString& path: pathList) {
			log(path);
			paths.push_back(path.toStdWString());
		}
		imagePaths.push_back(paths);
	});

	connect(ui->clear, &QPushButton::clicked, [this]() {
		imagePaths.clear();
		log("selection cleared");
	});

	connect(&timer, &QTimer::timeout, [this]() {
		ui->serial->blockSignals(true);
		QString previous = ui->serial->currentText();
		ui->serial->clear();
		QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
		for (QSerialPortInfo& info: infos) {
			ui->serial->addItem(info.portName());
		}
		if (!previous.isEmpty())
			ui->serial->setCurrentText(previous);
		ui->serial->blockSignals(false);
		if (ui->serial->currentText() != previous)
			ui->serial->currentTextChanged(ui->serial->currentText());

		if (timer.interval() != 1000)
			timer.setInterval(1000);
	});

	connect(ui->send, &QPushButton::clicked, [this]() {
		if (imagePaths.size() == 0) {
			log("please select image files");
			return;
		}
		freePrograms();
		programCount = imagePaths.size();
		programArray = new KsdkProgram[programCount];
		for (int i = 0; i < programCount; ++i) {
			unsigned int pageCount = imagePaths[i].size();
			programArray[i].pageCount = pageCount;
			programArray[i].pageArray = new KsdkPage[pageCount];
			for (int j = 0; j < pageCount; ++j) {
				KsdkPage& page = programArray[i].pageArray[j];
				page.imagePath = new wchar_t[imagePaths[i][j].size() + 1];
				wcscpy(page.imagePath, imagePaths[i][j].c_str());
				page.style = FadeRandom;
				page.speed = 10;
				page.stay = 1;
			}
		}
		sentCount = 0;
		std::string serial = ui->serial->currentText().toStdString();
		ksdkSend(serial.c_str(), programArray, programCount, ksdk_SendProgress, ksdk_SendResult);
		enableIoUi(false);
	});

	connect(ui->play, &QPushButton::clicked, [this]() {
		std::string serial = ui->serial->currentText().toStdString();
		ksdkPlay(serial.c_str(), ui->playIndex->value(), ksdk_PlayResult);
		enableIoUi(false);
	});

	timer.start();
}

MainDlg::~MainDlg()
{
	freePrograms();
	delete ui;
}

void MainDlg::log(const QString& txt)
{
	ui->log->appendPlainText(txt);
}

void MainDlg::enableIoUi(bool io)
{
	ui->send->setEnabled(io);
	ui->play->setEnabled(io);
}

void MainDlg::ksdkOpenResult(KsdkResult result)
{
	bool ok = (result == KSDK_OK);
	enableIoUi(ok);
	if (ok)
		log("open successful");
	else
		log(QString("open failed: %1").arg(result));
}

void MainDlg::ksdkSendResult(KsdkResult result)
{
	log(QString("send result: %1").arg(result));
	sentCount++;
	if (sentCount < ui->sendCount->value()) {
		std::string serial = ui->serial->currentText().toStdString();
		ksdkSend(serial.c_str(), programArray, programCount, ksdk_SendProgress, ksdk_SendResult);
	} else {
		enableIoUi(true);
	}
}

void MainDlg::ksdkSendProgress(const unsigned int max,
						const unsigned int value)
{
	log(QString("send progress: %1, max: %2").arg(value).arg(max));
}

void MainDlg::ksdkPlayResult(KsdkResult result)
{
	log(QString("play result: %1").arg(result));
	enableIoUi(true);
}

void MainDlg::freePrograms()
{
	if (!programArray || !programCount) return;

	for (unsigned int i = 0; i < programCount; ++i) {
		KsdkPage* pageArray = programArray[i].pageArray;
		if (!pageArray)
			continue;
		for (unsigned int j = 0; j < programArray[i].pageCount; ++j) {
			wchar_t* path = pageArray[j].imagePath;
			if (path)
				delete path;
		}
		delete[] pageArray;
	}
	delete[] programArray;
	programArray = nullptr;
	programCount = 0;
}
