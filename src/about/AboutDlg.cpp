#include "AboutDlg.h"
#include "ui_AboutDlg.h"

#include <QCoreApplication>
#include <QFile>
#include <QImageReader>
#include <QMovie>
#include <QPixmap>

AboutDlg::AboutDlg(QWidget *parent) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint),
	ui(new Ui::AboutDlg)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon(QPixmap("image/icon/about_48.png")));
	ui->name->setText(QCoreApplication::applicationName() + tr(" Editor"));
	QList<QByteArray> formats = QImageReader::supportedImageFormats();
	QString prefix{"image/picture/about."};
	for (auto& format: formats) {
		QString path = prefix + format;
		if (format == "gif") {
			movie = new QMovie{path};
			if (movie->isValid()) {
				ui->image->setMovie(movie);
				movie->start();
				break;
			} else {
				delete movie;
				movie = nullptr;
			}
		} else {
			QPixmap pix{path};
			if (pix.isNull()) continue;
			ui->image->setPixmap(pix);
			break;
		}
	}

	ui->version->setText(tr("Version:").append(qApp->applicationVersion()));
	QFile historyFile{"doc/history.txt"};
	if (historyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		ui->history->setText(historyFile.readAll());
	}
}

AboutDlg::~AboutDlg()
{
	delete ui;
	if (movie) delete movie;
}
