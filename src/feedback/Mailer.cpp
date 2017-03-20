#include "Mailer.h"

#include <list>
#include <QRegularExpression>
#include <QMimeDatabase>
#include <QDirIterator>

#include "smtp/SmtpMime"
#include "libzippp.h"
#include "Feedback.h"
#include "FeedLog.h"

using namespace libzippp;

void Mailer::run()
{
	SmtpClient smtp;

	QString mailServer = Feedback::setting->preset().value("mailServer").toString();
	if (!mailServer.isEmpty()) {
		smtp.setHost(mailServer);
	} else {
		feedErr << "invalid mail server";
		return;
	}

	QString mailUser = Feedback::setting->preset().value("mailUser").toString();
	if (!mailUser.isEmpty()) {
		smtp.setUser(mailUser);
	} else {
		feedErr << "invalid mail user";
		return;
	}
	EmailAddress from(mailUser);	//must be same as user

	QString mailPassword = Feedback::setting->preset().value("mailPassword").toString();
	if (!mailPassword.isEmpty()) {
		smtp.setPassword(mailPassword);
	} else {
		feedErr << "invalid mail password";
		return;
	}

	QString mailTo = Feedback::setting->preset().value("mailTo").toString();
	if (mailTo.isEmpty()) {
		feedErr << "invalid mail to";
		return;
	}
	EmailAddress to(mailTo);

	int mailPort = Feedback::setting->preset().value("mailPort").toInt();
	if (mailPort != 0) {
		smtp.setPort(mailPort);
	}

	QString mailProtocol = Feedback::setting->preset().value("mailProtocol").toString();
	if (!mailServer.isEmpty()) {
		if (mailProtocol == "tcp")
			smtp.setConnectionType(smtp.TcpConnection);
		else if (mailProtocol == "ssl")
			smtp.setConnectionType(smtp.SslConnection);
		else if (mailProtocol == "tls")
			smtp.setConnectionType(smtp.TlsConnection);
	}
	QString mailAuth = Feedback::setting->preset().value("mailAuth").toString();
	if (!mailAuth.isEmpty()) {
		if (mailAuth == "plain")
			smtp.setAuthMethod(smtp.AuthPlain);
		else if (mailAuth == "login")
			smtp.setAuthMethod(smtp.AuthLogin);
	}

	MimeMessage mail;
	mail.setFrom(&from);
	mail.addTo(&to);

	QRegularExpression mailRegex{"(^[A-Z0-9._%+-]+)@(?:[A-Z0-9-]+\\.)+[A-Z]{2,6}$",
								QRegularExpression::CaseInsensitiveOption};
	QRegularExpressionMatch replyToMatch = mailRegex.match(replyTo);
	if (replyToMatch.hasMatch())
		mail.setReplyTo(replyTo);

	mail.setSubject(subject);

	MimeHtml htmlPart;
	htmlPart.setHtml(html);
	mail.addPart(&htmlPart);

	QMimeDatabase db;
	QRegularExpression imageRegex{"cid:(image[0-9]+)"};
	QRegularExpressionMatchIterator it = imageRegex.globalMatch(html);
	while (it.hasNext()) {
		QRegularExpressionMatch imageMatch = it.next();
		QString path = images.value(imageMatch.captured(0));
		QFile* imageFile = new QFile(path);
		MimeInlineFile* imagePart = new MimeInlineFile(imageFile);
		imageFile->setParent(imageFile);
		imagePart->setContentId(imageMatch.captured(1));
		imagePart->setContentType(db.mimeTypeForFile(path).name());
		mail.addPart(imagePart);
		imagePart->setParent(&mail);
	}

	for (QString& path: attachmentPaths) {
		QFile* attachFile = new QFile(path);
		MimeAttachment* attachPart = new MimeAttachment(attachFile);
		attachFile->setParent(attachPart);
		mail.addPart(attachPart);
		attachPart->setParent(&mail);
	}

	QString logZipPath{Feedback::setting->tempDir() + "log.zip"};
	ZipArchive logZipArch(logZipPath.toStdString());
	logZipArch.open(ZipArchive::NEW);
	QDirIterator dirIt{"log", QDir::Files, QDirIterator::Subdirectories};
	while (dirIt.hasNext()) {
		std::string relativePath = QDir().relativeFilePath(dirIt.next()).toStdString();
		logZipArch.addFile(relativePath, relativePath);
	}
	logZipArch.close();

	QFile* logZipFile = new QFile(logZipPath);
	MimeAttachment* logPart = new MimeAttachment(logZipFile);
	logZipFile->setParent(logPart);
	mail.addPart(logPart);
	logPart->setParent(&mail);

	if (!smtp.connectToHost()) {
		emit result(tr("Failed to connect to host!"));
		return;
	}

	if (!smtp.login()) {
		emit result(tr("Failed to login!"));
		return;
	}

	if (!smtp.sendMail(mail)) {
		emit result(tr("Failed to send mail!"));
		return;
	}

	smtp.quit();

	emit result(QString());
}
