@echo off

set BUILD_PATH=%CD%
set SOURCE_PATH= %~dp0
cd /D %SOURCE_PATH%
set SOURCE_PATH=%CD%
cd /D %BUILD_PATH%

set PROJECT=%SOURCE_PATH%\applications.pro
set PROJECT_CONFIG=%BUILD_PATH%\config.in
set PROJECT_CONFIG_OUT=config.pri
set PROJECT_LOG=%BUILD_PATH%\config.log
set RELEASEMODE=debug
set BUILD_UNITTESTS=no
set QT_PATH=
set BOOST_PATH=
set FFMPEG_PATH=
set LAME_PATH=
set QMAKE_CACHE=%BUILD_PATH%\.qmake.cache

if exist "%QMAKE_CACHE%" del /Q %QMAKE_CACHE%
if exist "%PROJECT_CONFIG%" del /Q %PROJECT_CONFIG%
if exist "%PROJECT_LOG%" del /Q %PROJECT_LOG%

echo SOURCE_TREE = %SOURCE_PATH:\=/% > %QMAKE_CACHE%
echo BUILD_TREE = %BUILD_PATH:\=/% >> %QMAKE_CACHE%
set QMAKE_CACHE=


:cmdline_parsing
if "%1" == ""                   goto startProcessing
if "%1" == "-debug"             goto debugTag
if "%1" == "-release"           goto releaseTag
if "%1" == "-qt"                goto qtTag
if "%1" == "-boost"             goto boostTag
if "%1" == "-ffmpeg"            goto ffmpegTag
if "%1" == "-lame"              goto lameTag
if "%1" == "/?"                 goto usage
if "%1" == "-h"                 goto usage
if "%1" == "-help"              goto usage
if "%1" == "--help"             goto usage

echo Unknown option: "%1"
goto usage


:usage
echo Usage: configure.bat [-tests] [-debug] [-release] [boost (dir)] [-ffmpeg (dir)] [-lame (dir)]
    echo.
    echo Options:
    echo.
    echo -debug ............ Build with debugging symbols
    echo -release .......... Build without debugging symbols
    echo -boost ............ Path to Boost library
    echo -ffmpeg ........... Path to FFmpeg library
	echo -lame ............. Path to LAME library
	

if exist "%PROJECT_CONFIG%" del %PROJECT_CONFIG%
goto exitTag


:qtTag
shift
set QT_PATH=%1\
shift
goto cmdline_parsing


:boostTag
shift
set BOOST_PATH=%1
shift
goto cmdline_parsing

:ffmpegTag
shift
set FFMPEG_PATH=%1
shift
goto cmdline_parsing

:lameTag
shift
set LAME_PATH=%1
shift
goto cmdline_parsing

:debugTag
if "%RELEASEMODE%" == "release" set RELEASEMODE=debug
shift
goto cmdline_parsing


:releaseTag
if "%RELEASEMODE%" == "debug"   set RELEASEMODE=release
shift
goto cmdline_parsing


:startProcessing
echo CONFIG += %RELEASEMODE% >> %PROJECT_CONFIG%
set RELEASEMODE=

echo FFMPEG_PATH = %FFMPEG_PATH:\=/% >> %PROJECT_CONFIG%
echo BOOST_PATH = %BOOST_PATH:\=/% >> %PROJECT_CONFIG%
echo LAME_PATH = %LAME_PATH:\=/% >> %PROJECT_CONFIG%
set FFMPEG_PATH=
set BOOST_PATH=
set LAME_PATH=

echo Checking available Qt
call %QT_PATH%qmake -v >> %PROJECT_LOG% 2>&1
if errorlevel 1 goto qmakeNotFound
goto qmakeFound


:qmakeNotFound
echo ... Not found  >> %PROJECT_LOG% 2>&1
if "%QT_PATH%" == "" (
    echo >&2Cannot find 'qmake' in your PATH.
    echo >&2Your PATH is: %PATH%
) else (
    echo >&2Cannot find 'qmake' in %QT_PATH%.
)
echo >&2Aborting.
goto errorTag


:qmakeFound
call %QT_PATH%qmake -query QT_VERSION

if exist %PROJECT_CONFIG_OUT% del %PROJECT_CONFIG_OUT%
ren %PROJECT_CONFIG% %PROJECT_CONFIG_OUT%

echo.
echo Running qmake...
call %QT_PATH%qmake -recursive %PROJECT%
if errorlevel 1 goto qmakeRecError
echo.
echo configure has finished. You may run make to build the project now.
goto exitTag


:qmakeRecError
echo.
echo configure failed.
goto errorTag


:errorTag
set BUILD_PATH=
set PROJECT=
set PROJECT_CONFIG=
set PROJECT_CONFIG_OUT=
set PROJECT_LOG=
set QT_PATH=
set FFMPEG_PATH=
set BOOST_PATH=
set SOURCE_PATH=
set LAME_PATH=
exit /b 1


:exitTag
set BUILD_PATH=
set PROJECT=
set PROJECT_CONFIG=
set PROJECT_CONFIG_OUT=
set PROJECT_LOG=
set QT_PATH=
set FFMPEG_PATH=
set BOOST_PATH=
set SOURCE_PATH=
set LAME_PATH=
exit /b 0
