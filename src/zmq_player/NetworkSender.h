
#ifndef _NETWORK_SENDER_H_
#define _NETWORK_SENDER_H_

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <zhelpers.hpp>

class NetworkSender
{
public:
    NetworkSender(const std::string& zmqAddress, const std::string& envelope);
    virtual ~NetworkSender();

    /**
     * Sends the given data to the endpoint given in the ctor
     */
    void send(const std::string& data);

private:
    boost::shared_ptr<zmq::context_t> zmqContext_;
    boost::shared_ptr<zmq::socket_t> zmqSender_;
    std::string envelope_;

};

#endif //_NETWORK_SENDER_H_
