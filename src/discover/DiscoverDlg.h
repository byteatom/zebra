#ifndef DISCOVER_H
#define DISCOVER_H

#include <list>

#include <QDialog>
#include <QSettings>
#include <QTimer>

#include "IMainWnd.h"
#include "IScreen.h"
#include "SocketUdp.h"
#include "SerialPort.h"
#include "CardItem.h"

namespace Ui {
class DiscoverDlg;
}

class DiscoverDlg : public QDialog
{
	Q_OBJECT
public:
	DiscoverDlg(IMainWnd *mainWnd_, QWidget *parent = 0);
	virtual ~DiscoverDlg();

private:
	void start();
	void stop();

	void addCard(CardInfo *card);
	CardInfo* getCard(int row);
	void itemChanged(QTableWidgetItem *item);

	virtual void accept() override;

	Ui::DiscoverDlg *ui;

	IMainWnd *mainWnd = nullptr;
	IScreen *screen = nullptr;
	QSettings settings;
	QTimer timer;
	std::list<SocketUdp*> socketUdps;
	std::list<SerialPort*> serialPorts;
};

#endif // DISCOVER_H
