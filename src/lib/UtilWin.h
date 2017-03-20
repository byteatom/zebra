#ifndef UTILWIN_H
#define UTILWIN_H

#include <functional>

#include <QString>
#include <QImage>

namespace UtilWin {

bool execWait(const QString& path);

QImage getClipboardEmf();

bool isUsbDrive(wchar_t letter);

QString fileVersion(const QString& path);

struct OsVersion {
	int ver;
	int sp;
};

OsVersion osVersion();

QString osName(const OsVersion& ver);

QString os();

}

#endif // UTILWIN_H
