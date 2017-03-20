cd %~dp0

del app.7z
7z.exe a app.7z ..\release\*  -xr!data -xr!log

del skin.zip
7z.exe a skin.zip .\skin\*

set OUT_DIR=..\installer
mkdir %OUT_DIR%
for %%e in (..\release\*.exe) do (
	NSIS3\makensis.exe /V4 /INPUTCHARSET UTF8 /DAPP_NAME=%%~ne /DOUT_DIR=%OUT_DIR% app.nsi
)

del app.7z
del skin.zip

@pause