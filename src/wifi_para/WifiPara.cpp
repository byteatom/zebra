#include "WifiParaDlg.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QString lang{QLocale::system().name()};
	if (argc > 1)
		lang = argv[1];
	if (lang != QLocale(QLocale::English, QLocale::UnitedStates).name()) {
		QTranslator* trans = new QTranslator;
		if (trans->load(QString("language/wifi_para_%1.qm").arg(lang)))
			QCoreApplication::installTranslator(trans);
		else
			delete trans;
	}

	WifiParaDlg w;
	w.show();

	return a.exec();
}
