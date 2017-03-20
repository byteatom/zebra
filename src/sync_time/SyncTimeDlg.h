#ifndef SYNCTIMEDLG_H
#define SYNCTIMEDLG_H

#include <QDialog>
#include <QButtonGroup>
#include "IMainWnd.h"
#include "IScreen.h"
#include "IProtocol.h"

namespace Ui {
class SyncTimeDlg;
}

class SyncTimeDlg : public QDialog
{
	Q_OBJECT

public:
	SyncTimeDlg(IMainWnd *mainWnd_, QWidget *parent = 0);
	virtual ~SyncTimeDlg();

private:
	void selectMode(QAbstractButton *button);
	void getUiAttr();
	void showSystemTime();
	void save();
	void send();
	IMainWnd *mainWnd = nullptr;
	IScreen *screen = nullptr;
	IScreen::SyncTime setTime;
	QButtonGroup *selectRadios = nullptr;
	QTimer *timer = nullptr;

	Ui::SyncTimeDlg *ui;
};

#endif // SYNCTIMEDLG_H
