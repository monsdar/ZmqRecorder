
@echo off

REM Copy to install/bin directory and execute

set RECORD_ADDRESS=tcp://127.0.0.1:20002
set RECORD_ENVELOPE=ZmqNetworkLib

set PATH=%PATH%;%~dp0/../../../thirdparty/boost/bin/
set PATH=%PATH%;%~dp0/../../../thirdparty/zmq/bin/
set PATH=%PATH%;%~dp0/../../../thirdparty/sqlite3/bin/
set PATH=%PATH%;%~dp0/../../../thirdparty/sqlite3cc/bin/

%~dp0\zmq_recorder.exe
