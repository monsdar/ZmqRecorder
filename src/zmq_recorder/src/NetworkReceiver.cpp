
#include "zmq_recorder/NetworkReceiver.h"

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include <sstream>
#include <string>

NetworkReceiver::NetworkReceiver(const std::string& zmqAddress) :
    zmqContext_(new zmq::context_t(1)),
    zmqListener_(new zmq::socket_t(*zmqContext_, ZMQ_SUB)),
    isRunning_(false)
{
    zmqListener_->connect(zmqAddress.c_str());
}

NetworkReceiver::~NetworkReceiver()
{
    isRunning_ = false;
}

void NetworkReceiver::setCallback(ReceiveCallback givenCallback)
{
    callback_ = givenCallback;
}

void NetworkReceiver::startListening()
{
    //do not start listening twice
    if (isRunning_)
    {
        return;
    }

    isRunning_ = true;
    listenThread_.reset( new boost::thread(boost::bind(&NetworkReceiver::receive, this)) );
}

void NetworkReceiver::receive()
{
    while (isRunning_)
    {
        std::string data = s_recv(*zmqListener_);
        if (data.size() != 0)
        {
            if (callback_)
            {
                //TODO: The call to the callback is blocking, it could take a lot of time to
                //      perform it. In that time we cannot receive new data.
                //      Do we need a system here which prevents this?
                callback_(data);
            }
        }

        //let's give this thread a break every now and then... CPU will thank us ;)
        //Sleep is the easiest interruption-point here (see Boost docs for other types of interruption points)
        boost::this_thread::sleep( boost::posix_time::milliseconds(0) );
    }
}
