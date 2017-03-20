#ifndef WIFIPARADLG_H
#define WIFIPARADLG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class WifiParaDlg;
}

class WifiParaDlg : public QDialog
{
	Q_OBJECT

public:
	explicit WifiParaDlg(QWidget *parent = 0);
	~WifiParaDlg();

private:
	Ui::WifiParaDlg *ui;

	QTimer timer;

	void setUsbList();
	void save();
};

#endif // WIFIPARADLG_H
