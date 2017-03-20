#ifndef BRIGHTDLG_H
#define BRIGHTDLG_H

#include <QDialog>
#include <QButtonGroup>
#include "IMainWnd.h"
#include "IScreen.h"
#include "IProtocol.h"

namespace Ui {
class BrightDlg;
}

class BrightDlg : public QDialog
{
	Q_OBJECT

public:
	BrightDlg(IMainWnd *mainWnd_, QWidget *parent = 0);
	virtual ~BrightDlg();

private:
	void selectMode(QAbstractButton *button);
	void getUiAttr();
	void save();
	void send();
	IMainWnd *mainWnd = nullptr;
	IScreen *screen = nullptr;
	IScreen::Bright bright;
	QButtonGroup *radios;

	Ui::BrightDlg *ui;
};

#endif // BRIGHTDLG_H
