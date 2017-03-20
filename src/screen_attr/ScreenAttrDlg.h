#ifndef SCREENATTRDLG_H
#define SCREENATTRDLG_H

#include <QDialog>
#include <QString>
#include <QButtonGroup>

#include "IMainWnd.h"
#include "ISetting.h"
#include "IScreen.h"
#include "IProtocol.h"

namespace Ui {
class ScreenAttrDlg;
}

class ScreenAttrDlg : public QDialog
{
	Q_OBJECT

public:
	ScreenAttrDlg(IMainWnd *mainWnd_, ISetting *setting, QWidget *parent = 0);
	virtual ~ScreenAttrDlg();

private slots:
	void save(void);

private:
	int updataLedModule(const QString &usrPath, const QString &ownPath = QString());
	int openLedModule(const QString &path);
	void selectMode(QAbstractButton *button);
	void checkAutoMode(int state);

	QButtonGroup *selectRadios = nullptr;
	IMainWnd *mainWnd = nullptr;
	IScreen *screen = nullptr;
	IProtocol *protocol = nullptr;

	IScreen::ScreenPara setScreen;
	IScreen::Module ledModule;

	Ui::ScreenAttrDlg *ui;
};

#endif // SCREENATTRDLG_H
