
#include "zmq_player/Player.h"

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include <sstream>
#include <string>

Player::Player()
{}

Player::~Player()
{}

void Player::appendNewData(const std::vector<zmq_player::Packet>& data)
{
    //do not bother to claim the mutex if the given data is not usable or empty
    if (data.empty())
    {
        return;
    }

    //secure the dataQueue
    boost::mutex::scoped_lock dataLock(dataMutex_);
    dataQueue_.insert(dataQueue_.end(), data.begin(), data.end());
    queueEmptyCondition_.notify_all(); //notify the thread that we finally have data
}

void Player::setCallback(const DataCallback& callback)
{
    callback_ = callback;
}

void Player::start()
{
    //do not run the playThread twice
    if (!playThread_)
    {
        playThread_.reset(new boost::thread(boost::bind(&Player::playLoop, this)));
    }
}

void Player::playLoop()
{
    boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();

    while (true)
    {
        //wait until we have data in our queue
        {
            boost::mutex::scoped_lock dataLock(dataMutex_);
            while (dataQueue_.empty())
            {
                queueEmptyCondition_.wait(dataLock);
            }
        }
        
        //get the elapsed time since we started
        boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::local_time() - start;
        boost::int64_t elapsedTime = diff.total_microseconds();

        //check if the next data in our queue is ready to get triggered
        //the queue needs to get locked
        {
            boost::mutex::scoped_lock dataLock(dataMutex_);
            std::vector<zmq_player::Packet>::iterator currentElement = dataQueue_.begin();
            if (currentElement == dataQueue_.end()) //shouldn't be needed, but you never know...
            {
                continue;
            }
            
            if (currentElement->Timestamp < elapsedTime)
            {
                //TODO: Calling the callback in our locked mutex means that the user
                //      is not allowed to call appendNewData() in the callback!!
                if (callback_)
                {
                    callback_(currentElement->Data);
                }

                //erase the element from our list, it isn't needed anymore
                dataQueue_.erase(currentElement);
            }
        }

        //interruption point to provide a break point for the CPU
        boost::this_thread::sleep(boost::posix_time::microseconds(0));
    }
}
