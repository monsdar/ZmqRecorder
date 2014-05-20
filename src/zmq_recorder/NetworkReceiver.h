
#ifndef _NETWORK_RECEIVER_H_
#define _NETWORK_RECEIVER_H_

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <zhelpers.hpp>

class NetworkReceiver
{
public:
    typedef boost::function1<void, const std::string&> ReceiveCallback;

public:
    NetworkReceiver(const std::string& zmqAddress, const std::string& envelope);
    virtual ~NetworkReceiver();

    /**
     * The given callback will be called anytime this receiver got new data
     */
    void setCallback(ReceiveCallback givenCallback);
    /**
     * Start listening to the network. This method starts a Thread which then listens
     * to the network. The previously given callback will be called as soon as data is received.
     * If there is no callback given, data will be received but then forgotten immediately.
     */
    void startListening();

private:
    /**
     * Internal receive loop
     */
    void receive();

    /**
     * Determines whether the receiving thread is running
     */
    bool isRunning_;

    ReceiveCallback callback_;
    boost::shared_ptr<boost::thread> listenThread_;
    boost::shared_ptr<zmq::context_t> zmqContext_;
    boost::shared_ptr<zmq::socket_t> zmqListener_;

};

#endif //_NETWORK_RECEIVER_H_
