#!/bin/bash

BASEDIR=$(dirname $BASH_SOURCE)
REPLAY_DATABASE=test.db
REPLAY_ADDRESS=tcp://*:20002
REPLAY_ENVELOPE=ZmqNetworkLib

export LD_LIBRARY_PATH=$BASEDIR/../../../thirdparty/Boost/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BASEDIR/../../../thirdparty/zmq/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BASEDIR/../../../thirdparty/sqlite3/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BASEDIR/../../../thirdparty/sqlite3cc/lib

$BASEDIR/zmq_player

