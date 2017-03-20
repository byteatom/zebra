#ifndef MAINDLG_H
#define MAINDLG_H

#include <vector>
#include <string>

#include <QDialog>
#include <QStringList>
#include <QTimer>

#include "Ksdk.h"

namespace Ui {
class MainDlg;
}

class KsdkPage;

class MainDlg : public QDialog
{
	Q_OBJECT

public:
	explicit MainDlg(QWidget *parent = 0);
	~MainDlg();

	void log(const QString& txt);
	void enableIoUi(bool io);
	void ksdkOpenResult(KsdkResult result);
	void ksdkSendResult(KsdkResult result);
	void ksdkSendProgress(const unsigned int max,
							const unsigned int value);
	void ksdkPlayResult(KsdkResult result);

signals:
	void ksdkOpenResultSig(KsdkResult result);
	void ksdkSendResultSig(KsdkResult result);
	void ksdkSendProgressSig(const unsigned int max,
							const unsigned int value);
	void ksdkPlayResultSig(KsdkResult result);

private:
	void freePrograms();

	Ui::MainDlg *ui;
	QTimer timer;
	std::vector<std::vector<std::wstring>> imagePaths;
	void* obj = nullptr;
	KsdkProgram* programArray = nullptr;
	unsigned int programCount = 0;
	int sentCount = 0;
};

#endif // MAINDLG_H
