#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>

class QMovie;

namespace Ui {
class AboutDlg;
}

class AboutDlg : public QDialog
{
	Q_OBJECT

public:
	explicit AboutDlg(QWidget *parent);
	virtual ~AboutDlg();

private:
	Ui::AboutDlg *ui;
	QMovie *movie = nullptr;
};

#endif // ABOUTDLG_H
