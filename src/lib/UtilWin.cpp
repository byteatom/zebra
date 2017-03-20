#include "UtilWin.h"

//#include <WinSDKVer.h>
//#define _WIN32_WINNT 0x0501
#include <SDKDDKVer.h>
//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <VersionHelpers.h>

#include "Util.h"

#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Version.lib")

namespace UtilWin {

QImage getClipboardEmf() {
	QImage image;

	if (!OpenClipboard(nullptr)) return image;
	Util::Guard clipGurad{[](){CloseClipboard();}};

	if (!IsClipboardFormatAvailable(CF_ENHMETAFILE)) return image;

	HANDLE clipData = GetClipboardData(CF_ENHMETAFILE);
	if (!clipData) return image;

	HENHMETAFILE enh = CopyEnhMetaFileW((HENHMETAFILE)clipData, NULL);
	if (!enh) return image;
	Util::Guard enhGurad{[enh](){DeleteEnhMetaFile(enh);}};

	HDC dc = GetDC(nullptr);
	if (!dc) return image;
	Util::Guard dcGurad{[dc](){ReleaseDC(nullptr, dc);}};

	ENHMETAHEADER emhHeader;
	ZeroMemory(&emhHeader, sizeof(ENHMETAHEADER));
	if (!GetEnhMetaFileHeader(enh, sizeof(ENHMETAHEADER), &emhHeader)) return image;

	RECT rect;
	float pixX, pixY, mmX, mmY;
	long width, height;
	pixX = (float)GetDeviceCaps(dc, HORZRES);
	pixY = (float)GetDeviceCaps(dc, VERTRES);
	mmX = (float)GetDeviceCaps(dc, HORZSIZE);
	mmY = (float)GetDeviceCaps(dc, VERTSIZE);
	rect.top = (int)((float)emhHeader.rclFrame.top * pixY
					 / (mmY * 100.0f));
	rect.left = (int)((float)emhHeader.rclFrame.left * pixX
					  / (mmX * 100.0f));
	rect.right = (int)((float)emhHeader.rclFrame.right * pixX
					   / (mmX * 100.0f));
	rect.bottom = (int)((float)emhHeader.rclFrame.bottom * pixY
						/ (mmY * 100.0f));
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;

	HDC memDC = CreateCompatibleDC(dc);
	if (!memDC) return image;
	Util::Guard memDcGurad{[memDC](){DeleteDC(memDC);}};

	HBITMAP hbmp = CreateCompatibleBitmap(dc, width, height);
	if (!hbmp) return image;
	Util::Guard bmpGurad{[hbmp](){DeleteObject(hbmp);}};

	HGDIOBJ bmpOld = SelectObject(memDC, hbmp);
	if (!bmpOld) return image;
	Util::Guard bmpOldGurad{[memDC, bmpOld](){SelectObject(memDC, bmpOld);}};

	HBRUSH blackBrush = (HBRUSH)(RGB(255, 255, 255));
	HBRUSH brushOld = (HBRUSH)SelectObject(memDC, blackBrush);
	if (brushOld) return image;
	Util::Guard brushOldGurad{[memDC, brushOld](){SelectObject(memDC, brushOld);}};

	RECT clipRect;
	if (ERROR == GetClipBox(memDC, &clipRect)) return image;
	if (!PatBlt(memDC,
		   clipRect.left,
		   clipRect.top,
		   abs(clipRect.left - clipRect.right),
		   abs(clipRect.top - clipRect.bottom),
		   PATCOPY)) return image;
	brushOldGurad.release();

	if (!PlayEnhMetaFile(memDC, enh, &rect)) return image;

	BITMAP bmp;
	memset(&bmp, 0, sizeof(BITMAP));
	if(!GetObject(hbmp, sizeof(BITMAP), &bmp)) return image;
	LONG bmpSize = bmp.bmHeight * bmp.bmWidthBytes;
	const unsigned char *buf = new unsigned char[bmpSize];
	if (!GetBitmapBits(hbmp, bmpSize, (LPVOID)buf)) {
		delete buf;
		return image;
	}
	image = QImage{buf, bmp.bmWidth, bmp.bmHeight, bmp.bmWidthBytes,
				   QImage::Format_RGB32,
				  [](void* buf){delete buf;}, (void*)buf};

	return image;
}

bool execWait(const QString& path)
{
	std::wstring file = path.toStdWString();
	SHELLEXECUTEINFO exec{0};
	exec.cbSize = sizeof(SHELLEXECUTEINFO);
	exec.fMask = SEE_MASK_NOCLOSEPROCESS;
	exec.hwnd = nullptr;
	exec.lpVerb = nullptr;
	exec.lpFile = file.c_str();
	exec.lpParameters = nullptr;
	exec.lpDirectory = nullptr;
	exec.nShow = SW_SHOW;
	exec.hInstApp = nullptr;
	if (!ShellExecuteExW(&exec)) return false;
	if ((int)(exec.hInstApp) > 32) {
		if (exec.hProcess) {
			WaitForSingleObject(exec.hProcess, INFINITE);
			CloseHandle(exec.hProcess);
		}
		return true;
	} else
		return false;
}

bool isUsbDrive(wchar_t letter)
{
	wchar_t path[] = L"\\\\.\\X:";
	path[4] = letter;

	HANDLE handle = CreateFileW(
							  path,
							  0,                // no access to the drive
							  FILE_SHARE_READ | // share mode
							  FILE_SHARE_WRITE,
							  NULL,             // default security attributes
							  OPEN_EXISTING,    // disposition
							  0,                // file attributes
							  NULL);            // do not copy file attributes

	// setup query
	STORAGE_PROPERTY_QUERY query;
	memset(&query, 0, sizeof(query));
	query.PropertyId = StorageDeviceProperty;
	query.QueryType = PropertyStandardQuery;

	// issue query
	DWORD bytes;
	STORAGE_DEVICE_DESCRIPTOR device;
	STORAGE_BUS_TYPE busType = BusTypeUnknown;

	if (DeviceIoControl(handle,
						IOCTL_STORAGE_QUERY_PROPERTY,
						&query, sizeof(query),
						&device, sizeof(device),
						&bytes, NULL))
	{
		busType = device.BusType;
	}

	CloseHandle(handle);

	return BusTypeUsb == busType;
}

QString fileVersion(const QString& path)
{
	QString version;
	std::wstring file = path.toStdWString();

	DWORD len = GetFileVersionInfoSizeW(file.c_str(), nullptr);
	if (0 == len)
		return version;

	LPVOID data = new BYTE[len];
	Util::Guard dataGuard{[data]() {delete[] data;}};

	if (!GetFileVersionInfoW(file.c_str(), 0, len, data))
		return version;

	VS_FIXEDFILEINFO *info = nullptr;
	UINT infoLen;
	if (!VerQueryValueW(data, L"\\", (LPVOID*)&info, &infoLen))
		return version;

	version = "%1.%2.%3.%4";
	return version.arg((info->dwFileVersionMS >> 16) & 0xFFFF)
				  .arg((info->dwFileVersionMS >>  0) & 0xFFFF)
				  .arg((info->dwFileVersionLS >> 16) & 0xFFFF)
				  .arg((info->dwFileVersionLS >>  0) & 0xFFFF);
}

#ifndef _WIN32_WINNT_WIN10
#define _WIN32_WINNT_WIN10 0x0A00
#endif

OsVersion osVersion()
{
	OsVersion ver{0, 0};

	OSVERSIONINFOEXW osvi = {sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0};

	DWORDLONG const dwlConditionMask =
			VerSetConditionMask(
				VerSetConditionMask(
					VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL),
					VER_MINORVERSION, VER_GREATER_EQUAL),
				VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

#define testVersion(vVer, vSp) \
osvi.dwMajorVersion = HIBYTE(vVer);\
osvi.dwMinorVersion = LOBYTE(vVer);\
osvi.wServicePackMajor = (vSp);\
if (VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask))\
{\
	ver.ver = vVer;\
	ver.sp = vSp;\
	return ver;\
}

	testVersion(_WIN32_WINNT_WIN10,		0);
	testVersion(_WIN32_WINNT_WINBLUE,	0);
	testVersion(_WIN32_WINNT_WIN8,		0);
	testVersion(_WIN32_WINNT_WIN7,		1);
	testVersion(_WIN32_WINNT_WIN7,		0);
	testVersion(_WIN32_WINNT_VISTA,		2);
	testVersion(_WIN32_WINNT_VISTA,		1);
	testVersion(_WIN32_WINNT_VISTA,		0);
	testVersion(_WIN32_WINNT_WS03,		0);
	testVersion(_WIN32_WINNT_WINXP,		3);
	testVersion(_WIN32_WINNT_WINXP,		2);
	testVersion(_WIN32_WINNT_WINXP,		1);
	testVersion(_WIN32_WINNT_WINXP,		0);

	return ver;
}

QString osName(const OsVersion& ver)
{
	QString name{"Windows "};

	switch (ver.ver) {
	case _WIN32_WINNT_WIN10:
		name.append("10");
		break;
	case _WIN32_WINNT_WINBLUE:
		name.append("8.1");
		break;
	case _WIN32_WINNT_WIN8:
		name.append("8");
		break;
	case _WIN32_WINNT_WIN7:
		name.append("7");
		break;
	case _WIN32_WINNT_VISTA:
		name.append("VISTA");
		break;
	case _WIN32_WINNT_WS03:
		name.append("Server 2003");
		break;
	case _WIN32_WINNT_WINXP:
		name.append("XP");
		break;
	default:
		name.append("unsupported version");
		break;
	}
	if (ver.sp > 0)
		name.append(" SP").append(QString::number(ver.sp));

	return name;
}

QString os()
{
	OsVersion ver = osVersion();

	QString os{osName(ver)};

	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	os.append(", ");
	switch (sysInfo.wProcessorArchitecture) {
	case PROCESSOR_ARCHITECTURE_AMD64:
		os.append("64bit");
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		os.append("32bit");
		break;
	default:
		os.append("unsupported arch");
		break;
	}

	OSVERSIONINFOEXW osInfo{sizeof(osInfo)};
	if (GetVersionExW((LPOSVERSIONINFOW)&osInfo))
	{
		int getVer = MAKEWORD(osInfo.dwMinorVersion, osInfo.dwMajorVersion);
		if (getVer != ver.ver
			|| osInfo.wServicePackMajor != ver.sp)
		{
			os.append(", compat mode: ").append(osName({getVer, osInfo.wServicePackMajor}));
		}
	}

	return os;
}

}
