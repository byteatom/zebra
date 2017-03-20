cd /d %~dp0

set PATH=%PATH%;..\tool

for %%l in (zh_CN en_US) do (
    mkdir %%l
    for /D %%d in (..\src\*) do (
        lupdate.exe -no-recursive -no-obsolete %%d -ts %%l\%%~nd.ts
    )
)

pause
