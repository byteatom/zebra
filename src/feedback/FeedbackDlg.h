#ifndef FEEDBACKDLG_H
#define FEEDBACKDLG_H

#include <QDialog>

namespace Ui {
class FeedbackDlg;
}

class Mailer;

class FeedbackDlg : public QDialog
{
	Q_OBJECT

public:
	explicit FeedbackDlg(QWidget *parent = 0);
	~FeedbackDlg();

	QString appendix();
	void send();
	void result(const QString& status);

private:
	Ui::FeedbackDlg *ui;
};

#endif // FEEDBACKDLG_H
