
@echo off

REM Copy to install/bin directory and execute

set PATH=%PATH%;%~dp0/../../../thirdparty/boost/bin/
set PATH=%PATH%;%~dp0/../../../thirdparty/zmq/bin/
set PATH=%PATH%;%~dp0/../../../thirdparty/sqlite3/bin/
set PATH=%PATH%;%~dp0/../../../thirdparty/sqlite3cc/bin/

%~dp0\zmq_recorder.exe
