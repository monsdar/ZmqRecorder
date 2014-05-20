
@echo off

REM Copy to install/bin directory and execute

set REPLAY_DATABASE=test.db
set REPLAY_ADDRESS=tcp://*:20002
set REPLAY_ENVELOPE=ZmqNetworkLib

set PATH=%PATH%;%~dp0/../../../thirdparty/boost/bin/
set PATH=%PATH%;%~dp0/../../../thirdparty/zmq/bin/
set PATH=%PATH%;%~dp0/../../../thirdparty/sqlite3/bin/
set PATH=%PATH%;%~dp0/../../../thirdparty/sqlite3cc/bin/

%~dp0\zmq_player.exe
