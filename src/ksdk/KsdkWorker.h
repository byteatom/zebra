#ifndef KSDKTHREAD_H
#define KSDKTHREAD_H

#include <QThread>
#include <QMutex>

#include "KsdkImpl.h"

class KsdkWork : public QThread
{
	Q_OBJECT
public:
	KsdkWork(const char* serialName,
			const KsdkProgram* programArray,
			const unsigned int programCount,
			KsdkProgressCb progressCb,
			KsdkResultCb resultCb);
	KsdkWork(const char* serialName,
			const unsigned int index,
			KsdkResultCb resultCb);
	KsdkWork(const char* serialName,
			KsdkResultCb resultCb,
			unsigned int brightness);
	virtual ~KsdkWork();

signals:
	void send(const KsdkProgram* programArray,
				const unsigned int programCount,
				KsdkProgressCb progressCb,
				KsdkResultCb resultCb);
	void play(const unsigned int index,
				KsdkResultCb resultCb);
	void setBrightness(unsigned int brightness,
					KsdkResultCb resultCb);
	void resultSig(KsdkResult result);

private:
	char serialName[255];
	KsdkResultCb resultCbProxy = nullptr;
	KsdkResultCb resultCb = nullptr;

	const KsdkProgram* programArray = nullptr;
	const unsigned int programCount = 0;
	KsdkProgressCb progressCb = nullptr;

	const unsigned int index = 0;

	KsdkImpl* impl = nullptr;

	QMutex mutex;

	virtual void run() override;
	void resultSlot(KsdkResult result);
};

#endif // KSDKTHREAD_H
