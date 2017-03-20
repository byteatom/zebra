
@echo off

SET VsVersion=14
SET VsDevCmd=C:\Program Files (x86)\Microsoft Visual Studio %VsVersion%.0\Common7\Tools\VsDevCmd.bat
SET zlib=lib\zlib-1.2.8
SET libzip=lib\libzip-1.1.3

if not exist "%zlib%" goto error_zlib_not_found
if not exist "%libzip%" goto error_libzip_not_found
if not exist "%VsDevCmd%" goto error_vs_not_found

call "%VsDevCmd%"

:compile_zlib
if exist "%zlib%\build" goto compile_libzip
echo Compiling zlib...
cd "%zlib%"
mkdir build
cd "build"
REM cmake .. -G"Visual Studio %VsVersion%" -DCMAKE_SYSTEM_VERSION=5.01 -DCMAKE_INSTALL_PREFIX="install"
cmake .. -G"Visual Studio %VsVersion%" -T v140_xp -DCMAKE_INSTALL_PREFIX="install"
if %ERRORLEVEL% GEQ 1 goto error_zlib
msbuild /P:Configuration=Debug INSTALL.vcxproj
if %ERRORLEVEL% GEQ 1 goto error_zlib
msbuild /P:Configuration=Release INSTALL.vcxproj
if %ERRORLEVEL% GEQ 1 goto error_zlib
cd "..\..\.."

:compile_libzip
if exist "%libzip%\build" goto prepare_libzippp
echo Compiling libzip...
cd "%libzip%"
mkdir build
cd "build"
REM cmake .. -G"Visual Studio %VsVersion%" -DCMAKE_SYSTEM_VERSION=5.01 -DCMAKE_PREFIX_PATH="../../%zlib%/build/install"
cmake .. -G"Visual Studio %VsVersion%" -T v140_xp -DCMAKE_PREFIX_PATH="../../%zlib%/build/install"
if %ERRORLEVEL% GEQ 1 goto error_libzip
msbuild /P:Configuration=Debug ALL_BUILD.vcxproj
if %ERRORLEVEL% GEQ 1 goto error_libzip
msbuild /P:Configuration=Release ALL_BUILD.vcxproj
if %ERRORLEVEL% GEQ 1 goto error_libzip
cd "..\..\.."

:prepare_libzippp
if exist "build" goto compile_libzippp
echo Compiling lizippp...
mkdir build
cd "build"
REM cmake .. -G"Visual Studio %VsVersion%" -DCMAKE_SYSTEM_VERSION=5.01 -DCMAKE_PREFIX_PATH="%zlib%/build/install"
cmake .. -G"Visual Studio %VsVersion%" -T v140_xp -DCMAKE_PREFIX_PATH="%zlib%/build/install"
if %ERRORLEVEL% GEQ 1 goto error_libzippp
cd ".."

:compile_libzippp
cd "build"
if exist "libzippp_static.lib" goto package_libzippp
msbuild /P:Configuration=Debug ALL_BUILD.vcxproj
if %ERRORLEVEL% GEQ 1 goto error_libzippp
msbuild /P:Configuration=Release ALL_BUILD.vcxproj
if %ERRORLEVEL% GEQ 1 goto error_libzippp
cd ".."

:package_libzippp
if exist "dist\libzippp_static.lib" goto end
mkdir "dist"
cd "dist"
mkdir release
copy ..\build\Release\libzippp_shared_test.exe release
copy ..\build\Release\libzippp_static_test.exe release
copy ..\build\Release\libzippp.dll release
copy ..\build\Release\libzippp.lib release
copy ..\build\Release\libzippp_static.lib release
copy ..\%zlib%\build\Release\zlib1.dll release
copy ..\%libzip%\build\lib\Release\zip.dll release
mkdir debug
copy ..\build\Debug\libzippp_shared_test.exe debug
copy ..\build\Debug\libzippp_static_test.exe debug
copy ..\build\Debug\libzippp.dll debug
copy ..\build\Debug\libzippp.lib debug
copy ..\build\Debug\libzippp_static.lib debug
copy ..\%zlib%\build\Debug\zlibd1.dll debug
copy ..\%libzip%\build\lib\Debug\zip.dll debug
cd ..

goto end

:error_zlib_not_found
echo [ERROR] The path was not found: %zlib%.
echo         You have to download zlib 1.2.8 and put in the folder %zlib%.
goto end

:error_zlib
echo [ERROR] Unable to compile zlib
goto end

:error_libzip_not_found
echo [ERROR] The path was not found: %libzip%.
echo         You have to download libzip 1.0.1 and put it in the folder %libzip%.
goto end

:error_libzip
echo [ERROR] Unable to compile libzip
goto end

:error_vs_not_found
echo [ERROR] VisualStudio was not found (path not found: %VsDevCmd%).
goto end

:error_libzippp
echo [ERROR] Unable to compile libzippp
goto end

:end
cmd
