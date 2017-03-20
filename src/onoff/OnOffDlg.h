#ifndef ONOFFDLG_H
#define ONOFFDLG_H

#include <QDialog>
#include "IMainWnd.h"
#include "IScreen.h"
#include "IProtocol.h"

namespace Ui {
class OnOffDlg;
}

class OnOffDlg : public QDialog
{
	Q_OBJECT

public:
	OnOffDlg(IMainWnd *mainWnd_, QWidget *parent = 0);
	virtual ~OnOffDlg();

private:
	void openScren();
	void closeScren();
	void send();
	void save();
	IMainWnd *mainWnd = nullptr;
	IScreen *screen = nullptr;
	IScreen::OnOff onOff;

	Ui::OnOffDlg *ui;
};

#endif // ONOFFDLG_H
