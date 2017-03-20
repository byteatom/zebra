#ifndef SENDDLG_H
#define SENDDLG_H

#include <QDialog>
#include <QTimer>
#include <QButtonGroup>

#include "IScreen.h"

namespace Ui {
class ExportDlg;
}

class ExportDlg : public QDialog
{
	Q_OBJECT

public:
	explicit ExportDlg(QWidget *parent, IScreen* screen);
	~ExportDlg();

private:
	void setWidgetStatus(bool b);
	void setUsbList();
	void toUsb();
	void toDir();
	void build(QString dir);

	Ui::ExportDlg *ui;
	IScreen* screen;
	IScreen::ScreenPara& para;
	IScreen::OnOff& onOff;
	IScreen::SyncTime& sync;
	IScreen::Bright& bright;
	QTimer timer;
	QButtonGroup btgTiming;
	QButtonGroup btgBright;
};

#endif // SENDDLG_H
