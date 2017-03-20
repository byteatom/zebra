setlocal EnableDelayedExpansion
cd /d %~dp0

set PATH=%PATH%;..\tool

for /D %%d in (*) do (
    set files=
    for %%f in (%%d\*) do (
        if /i not %%f=="wifipara" set files=!files! %%f
    )
    lrelease !files! -qm ..\debug\language\kshow_%%d.qm
    copy /y ..\debug\language\kshow_%%d.qm ..\release\language\kshow_%%d.qm
)

for /D %%d in (*) do (
    lrelease %%d\wifi_para.ts -qm ..\debug\language\wifi_para_%%d.qm
    copy /y ..\debug\language\wifi_para_%%d.qm ..\release\language\wifi_para_%%d.qm
)

pause