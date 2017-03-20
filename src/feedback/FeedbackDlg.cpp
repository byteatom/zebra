#include "FeedbackDlg.h"
#include "ui_FeedbackDlg.h"

#include <QMessageBox>
#include <QSysInfo>

#include "Mailer.h"
#include "UtilWin.h"

FeedbackDlg::FeedbackDlg(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FeedbackDlg)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon{"image/icon/feedback_64.png"});

	connect(ui->commit, &QPushButton::clicked, this, &FeedbackDlg::send);
	connect(ui->close, &QPushButton::clicked, this, &FeedbackDlg::reject);
}

FeedbackDlg::~FeedbackDlg()
{
	delete ui;
}

QString FeedbackDlg::appendix()
{
	QString info;
	info.append("<p>OS: ").append(QSysInfo::prettyProductName())
		.append(" ").append(QSysInfo::currentCpuArchitecture())
		.append(" v").append(QSysInfo::kernelVersion()).append("</p>");
	info.append("<p>AppPath: ").append(qApp->applicationFilePath()).append("</p>");
	return info;
}

void FeedbackDlg::send()
{
	if (ui->email->text().isEmpty()
		&& ui->phone->text().isEmpty()
		&& ui->im->text().isEmpty()) {
		QMessageBox::information(this, tr("Prompt"),
						tr("Please provide us with you Email, Phone and/or IM."));
		return;
	}

	ui->commit->setEnabled(false);
	ui->status->setStyleSheet("color: blue");
	ui->status->setText(tr("Sending......"));

	Mailer* mailer = new Mailer;

	mailer->replyTo = ui->email->text();

	mailer->subject.append(qApp->applicationName())
			.append(" ")
			.append(qApp->applicationVersion())
			.append(" From");
	QString email = ui->email->text();
	if (!email.isEmpty())
		mailer->subject.append(" [Email: ")
				.append(email)
				.append("]");
	QString phone = ui->phone->text();
	if (!phone.isEmpty())
		mailer->subject.append(" [Phone: ")
				.append(phone)
				.append("]");
	QString im = ui->im->text();
	if (!im.isEmpty())
		mailer->subject.append(" [")
				.append(ui->ims->currentText())
				.append(": ")
				.append(im)
				.append("]");

	mailer->html = ui->content->toHtml();
	mailer->html.insert(mailer->html.lastIndexOf("</body>"), appendix());

	ui->content->exportImages(mailer->images);
	connect(mailer, &Mailer::result, this, &FeedbackDlg::result);
	connect(mailer, &Mailer::finished, mailer, &QObject::deleteLater);

	mailer->start();
}

void FeedbackDlg::result(const QString& status)
{
	if (status.isEmpty()) {
		ui->status->setStyleSheet("color: green");
		ui->status->setText(tr("Succeed"));
	} else {
		ui->status->setStyleSheet("color: red");
		ui->status->setText(status);
	}
	ui->commit->setEnabled(true);
}
