#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Shlwapi.h>
#include <StrSafe.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	const int size = 1024;
	wchar_t name[size];
	GetModuleFileNameW(NULL, name, size);
	PathStripPathW(name);
	PathRemoveExtensionW(name);
	wchar_t cmd[size] = L".\\bin\\kshow.exe ";
	StringCchCatW(cmd, size, name);

	while (true) {
		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);

		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));

		CreateProcessW(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

		WaitForSingleObject(pi.hProcess, INFINITE);

		DWORD exitCode = 0;
		GetExitCodeProcess(pi.hProcess, &exitCode);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (exitCode != 0x12345678) break;
	}
}
