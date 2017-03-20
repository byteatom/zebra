#ifndef MAILER_H
#define MAILER_H

#include <QThread>
#include <QString>
#include <QHash>
#include <list>

class Mailer : public QThread
{
	Q_OBJECT

private:
	virtual void run() override;

public:
	QString replyTo;
	QString subject;
	QString html;
	QHash<QString, QString> images;
	std::list<QString> attachmentPaths;

signals:
	void result(const QString& status);
};

#endif // MAILER_H
