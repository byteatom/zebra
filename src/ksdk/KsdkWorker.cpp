#include "KsdkWorker.h"

KsdkWork::KsdkWork(const char* serialName_,
				   const KsdkProgram* programArray,
				   const unsigned int programCount,
				   KsdkProgressCb progressCb,
				   KsdkResultCb resultCb_):
	programArray{programArray},
	programCount{programCount},
	progressCb{progressCb},
	resultCb{resultCb_},
	resultCbProxy{[this](KsdkResult result) {resultSlot(result);}}
{
	connect(this, &KsdkWork::finished, this, &QObject::deleteLater);
	memcpy(serialName, serialName_, sizeof(serialName));
	mutex.lock();
	start();
	mutex.lock();
	emit send(programArray, programCount, progressCb, resultCbProxy);
}

KsdkWork::KsdkWork(const char* serialName_,
				   const unsigned int index,
				   KsdkResultCb resultCb_):
	index{index},
	resultCb{resultCb_},
	resultCbProxy{[this](KsdkResult result) {resultSlot(result);}}
{
	connect(this, &KsdkWork::finished, this, &QObject::deleteLater);
	memcpy(serialName, serialName_, sizeof(serialName));
	mutex.lock();
	start();
	mutex.lock();
	emit play(index, resultCbProxy);
}

KsdkWork::KsdkWork(const char* serialName_,
				   KsdkResultCb resultCb_,
				   unsigned int brightness):
	resultCb{resultCb_},
	resultCbProxy{[this](KsdkResult result) {resultSlot(result);}}
{
	connect(this, &KsdkWork::finished, this, &QObject::deleteLater);
	memcpy(serialName, serialName_, sizeof(serialName));
	mutex.lock();
	start();
	mutex.lock();
	emit setBrightness(brightness, resultCbProxy);
}

KsdkWork::~KsdkWork()
{
	if (impl) {
		delete impl;
		impl = nullptr;
	}
}

void KsdkWork::run()
{
	try {
		impl =  new KsdkImpl{serialName};
	} catch (KsdkResult r) {
		resultCbProxy(r);
		mutex.unlock();
		return;
	} catch (...) {
		resultCbProxy(KSDK_ERR_OPEN);
		mutex.unlock();
		return;
	}
	connect(this, &KsdkWork::send, impl, &KsdkImpl::send);
	connect(this, &KsdkWork::play, impl, &KsdkImpl::play);
	connect(this, &KsdkWork::setBrightness, impl, &KsdkImpl::setBrightness);

	mutex.unlock();
	exec();

	if (impl) {
		delete impl;
		impl = nullptr;
	}
}

void KsdkWork::resultSlot(KsdkResult result)
{
	if (resultCb) resultCb(result);
	quit();
}
