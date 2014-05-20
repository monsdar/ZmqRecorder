
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Packet.h"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

class Player
{
public:
    typedef boost::function1<void, const std::string&> DataCallback;

public:
    Player();
    virtual ~Player();

    /**
     * Sets the datacallback
     * This callback will be called everytime the given data is triggered
     *
     * WARNING: Attempting to call appendNewData() from the callback will result in a Deadlock
     *
     */
    void setCallback(const DataCallback& callback);

    /**
     * Appends new data to the data queue
     * New data needs to be later in time, as all elements will be processed serially
     */
    void appendNewData(const std::vector<zmq_player::Packet>& data);

    /**
     * This starts playing the given data
     * It is possible to append new data during the play, once triggered data will be
     * forgotten (this allows to add data more memory friendly)
     */
    void start();

    /**
     * Return true if the player is currently running
     */
    bool isRunning();

private:

    /**
     * This loop runs and checks if the data got triggered
     */
    void playLoop();

    /**
     * Secures the dataQueue
     */
    boost::mutex dataMutex_;
    /**
     * This is a condition variable which is needed to wait until we have data in our queue
     */
    boost::condition_variable queueEmptyCondition_;
    boost::shared_ptr<boost::thread> playThread_;
    std::vector<zmq_player::Packet> dataQueue_;
    DataCallback callback_;
};

#endif //_PLAYER_H_
