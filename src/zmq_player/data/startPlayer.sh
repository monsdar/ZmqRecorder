#!/bin/bash

BASEDIR=$(dirname $BASH_SOURCE)

export LD_LIBRARY_PATH=$BASEDIR/../../../thirdparty/Boost/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BASEDIR/../../../thirdparty/zmq/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BASEDIR/../../../thirdparty/sqlite3/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BASEDIR/../../../thirdparty/sqlite3cc/lib

$BASEDIR/zmq_player

