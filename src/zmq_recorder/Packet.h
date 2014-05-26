
#ifndef _PACKET_H_
#define _PACKET_H_

#include <boost/cstdint.hpp>
#include <string>

namespace zmq_recorder
{
    struct Packet
    {
        boost::int64_t Timestamp;
        std::string Data;
    };
}

#endif //_PACKET_H_
