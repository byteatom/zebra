#include "Ksdk.h"

#include <QCoreApplication>

#include "KsdkWorker.h"

QCoreApplication* app = nullptr;
KsdkWork* work = nullptr;

void ksdkInitApp()
{
	qRegisterMetaType<KsdkResult>();
	qRegisterMetaType<KsdkProgressCb>();
	qRegisterMetaType<KsdkResultCb>();
	if (!QCoreApplication::instance()) {
		int argc = 1;
		char* argv = "ksdk";
		app = new QCoreApplication{argc, &argv};
	}
}

KSDK_API void ksdkSend(const char* serialPortName,
						const KsdkProgram* programArray,
						const unsigned int programCount,
						KsdkProgressPf progressPf,
						KsdkResultPf resultPf)
{
	ksdkInitApp();
	new KsdkWork{serialPortName, programArray, programCount, progressPf, resultPf};
}

KSDK_API void ksdkPlay(const char* serialPortName,
						const unsigned int index,
						KsdkResultPf resultPf)
{
	ksdkInitApp();
	new KsdkWork{serialPortName, index, resultPf};
}

KSDK_API void ksdkSetBrightness(const char* serialPortName,
						const unsigned int brightness,
						KsdkResultPf resultPf)
{
	if (brightness < 1 || brightness > 16) {
		resultPf(KSDK_ERR_SET_BRIGHTNESS);
		return;
	}
	ksdkInitApp();
	new KsdkWork{serialPortName, resultPf, brightness};
}




