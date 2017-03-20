#ifndef KSDKIMPL_H
#define KSDKIMPL_H

#include <list>
#include <functional>

#include <QObject>

#include "Ksdk.h"
#include "Message.h"
#include "IPlugin.h"
#include "IProtocolFactory.h"
#include "IProtocol.h"

typedef std::function<void (KsdkResult result)> KsdkResultCb;
typedef std::function<void (const unsigned int max, const unsigned int value)> KsdkProgressCb;

Q_DECLARE_METATYPE(KsdkResult)
Q_DECLARE_METATYPE(KsdkResultCb)
Q_DECLARE_METATYPE(KsdkProgressCb)


class KsdkImpl : public QObject
{
	Q_OBJECT
public:
	explicit KsdkImpl(const char* serialName);
	virtual ~KsdkImpl();
	void rmTmpDir();

	void send(const KsdkProgram* programArray,
				const unsigned int programCount,
				KsdkProgressCb progressCb,
				KsdkResultCb resultCb);
	void play(const unsigned int index,
				KsdkResultCb resultCb);
	void setBrightness(const unsigned int brightness,
				KsdkResultCb resultCb);
private:
	void sendProgress(qint64 chunk);
	void sendResult(KsdkResult result);
	void sendScf();
	void sendSef();
	void sendDcf();
	void sendSyncTime();
	void sendDisplaySave();
	void sendScreenClose();
	void sendContentClear();
	void sendContentFile();
	void sendContentSave();
	void sendScreenOpen();

	std::list<IPlugin*> plugins;
	IPlugin* protocolPlugin = nullptr;
	IProtocol* protocol = nullptr;
	QString tmpDir;
	QString scf;
	QString sef;
	QString dcf;
	std::list<QString> contents;
	ProgressRecver sendProgressRecver;
	qint64 sendProgressMax = 0;
	qint64 sendProgressValue = 0;
	KsdkProgressCb sendProgressCb = nullptr;
	KsdkResultCb sendResultCb = nullptr;
};

#endif // KSDKIMPL_H
