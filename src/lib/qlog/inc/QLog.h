#ifndef QLOG_H
#define QLOG_H

#include <string>

#include <QPoint>
#include <QSize>
#include <QRect>
#include <QPixmap>
#include <QBitmap>
#include <QImage>
#include <QTextStream>

#if defined QLOG_LIB
#define LIB_API Q_DECL_EXPORT
#else
#define LIB_API Q_DECL_IMPORT
#pragma comment(lib, "qlog.lib")
#endif

struct QLogData;

class LIB_API QLog
{
public:
	enum Level{
		ENone,
		EEmergency,
		EAlert,
		ECritical,
		EError,
		EWarnning,
		ENotice,
		EInfomation,
		EDebug,
		EALL,
	};

	QLog(Level level);
	~QLog();

	template <typename Type>
	QLog& operator<<(const Type& value) {
		stream << value;
		return *this;
	}

	QLog& operator<<(const char* string);
	QLog& operator<<(const std::string &string);
	QLog& operator<<(const wchar_t* string);
	QLog& operator<<(const std::wstring &string);
	QLog& operator<<(const QPoint &point);
	QLog& operator<<(const QPointF &point);
	QLog& operator<<(const QSize &size);
	QLog& operator<<(const QSizeF &size);
	QLog& operator<<(const QRect &rect);
	QLog& operator<<(const QRectF &rect);
	QLog& operator<<(const QPixmap &pixmap);
	QLog& operator<<(const QBitmap &bitmap);
	QLog& operator<<(const QImage &image);

private:
	QLogData* data;
	QTextStream stream;
};

#define qEmerg QLog(QLog::EEmergency)
#define qAlert QLog(QLog::EAlert)
#define qCriti QLog(QLog::ECritical)
#define qErr QLog(QLog::EError)
#define qWarn QLog(QLog::EWarnning)
#define qNotice QLog(QLog::ENotice)
#define qInfo QLog(QLog::EInfomation)
#define qDbg QLog(QLog::EDebug)

#endif // QLOG_H
