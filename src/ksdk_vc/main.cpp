#include <Windows.h>

#include <stdio.h>

#include "Ksdk.h"

HANDLE event = nullptr;

void ksdk_Result(KsdkResult result)
{
	printf("ksdk_Result %d\r\n", result);
	SetEvent(event);
}

void ksdk_Progress(const unsigned int max,
						const unsigned int value)
{
	printf("ksdk_Progress %d-%d\r\n", max, value);
}

int main(int argc, char *argv[])
{
	for (int u = 0; u < 1; ++u) {
	event = CreateEventW(nullptr, true, false, nullptr);

	unsigned int programCount = 1;
	KsdkProgram* programArray = new KsdkProgram[programCount];
	for (int i = 0; i < programCount; ++i) {
		unsigned int pageCount = 1;
		programArray[i].pageCount = pageCount;
		programArray[i].pageArray = new KsdkPage[pageCount];
		for (int j = 0; j < pageCount; ++j) {
			KsdkPage& page = programArray[i].pageArray[j];
			page.imagePath = new wchar_t[255];
			wcscpy(page.imagePath, L"about_48.png");
			page.style = FadeRandom;
			page.speed = 10;
			page.stay = 1;
		}
	}

	//ksdkSend("COM4", programArray, programCount, ksdk_Progress, ksdk_Result);

	//ksdkPlay("COM4", 0, ksdk_Result);

	ksdkSetBrightness("COM4", 8, ksdk_Result);

	DWORD w = WaitForSingleObject(event, INFINITE);
	//Sleep(10000);

	for (unsigned int i = 0; i < programCount; ++i) {
		KsdkPage* pageArray = programArray[i].pageArray;
		if (!pageArray)
			continue;
		for (unsigned int j = 0; j < programArray[i].pageCount; ++j) {
			wchar_t* path = pageArray[j].imagePath;
			if (path)
				delete path;
		}
		delete[] pageArray;
	}
	delete[] programArray;
	programArray = nullptr;
	programCount = 0;

	CloseHandle(event);
	event = nullptr;
	}

	return 0;
}
