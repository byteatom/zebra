#ifndef SENDDLG_H
#define SENDDLG_H

#include <functional>

#include <QDialog>

#include "IScreen.h"
#include "ISetting.h"

namespace Ui {
class SendDlg;
}

class SendDlg : public QDialog
{
	Q_OBJECT

public:
	explicit SendDlg(QWidget *parent, IScreen* screen, ISetting* setting);
	~SendDlg();

private:
	Ui::SendDlg *ui;

	virtual void done(int ret) override;

	IScreen* screen;
	ISetting* setting;
	IProtocol* protocol;
	BoolRecver boolRecver;
	ProgressRecver progressRecver;

	void start();
	void setProgress(qint64 chunk);
	void fail(const QString& status);
	void finish();

	void sendInfoGet();
	void build();
	void sendScf();
	void sendSef();
	void sendDcf();
	void sendSyncTime();
	void sendDisplaySave();
	void sendScreenClose();
	void sendContentClear();
	void sendContentFile();
	void sendContentSave();
	void sendScreenOpen();
	void sendBrightness();

	QString scf;
	QString sef;
	QString dcf;
	std::list<QString> contents;
	qint64 progressMax;
	qint64 progress;
	QString statusFileSending{tr("Sending file %1 ......")};
	QString statusFileFailed{tr("Failed to send file %1 !")};
};

#endif // SENDDLG_H
