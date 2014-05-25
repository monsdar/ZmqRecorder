ZmqRecorder
===========

A tool to record and replay network streams that have been sent via ZeroMQ Pub/Sub. This project contains `zmq_recorder` which stores any data it receives from a ZMQ Pub/Sub network. The other tool is `zmq_player` which replays the recorded data. Data is stored with its timestamp and in the incoming order.

### How to build
The build system of ZmqRecorder is the same as in [CxxProf](https://github.com/monsdar/CxxProf). It uses the same Thirdparty too. Have a look at the [build documentation](https://github.com/monsdar/CxxProf/wiki/DevGuide:-How-to-compile-CxxProf) there.

### Copying
Free use of this software is granted under the terms of the GNU Lesser General Public License (LGPL). For details see the files `COPYING` and `COPYING.LESSER` included with the CxxProf distribution.
