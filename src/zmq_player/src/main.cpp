
#include "zmq_player/DataReader.h"
#include "zmq_player/NetworkSender.h"
#include "zmq_player/Player.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <map>
#include <iostream>

boost::shared_ptr<DataReader> reader_;
boost::shared_ptr<NetworkSender> sender_;
Player player_;


std::string getEnv(const std::string& envVariable)
{
    const char * val = std::getenv(envVariable.c_str());
    if (val == 0)
    {
        return "";
    }

    return val;
}

//this method will be called by our Player everytime the data should be send
void triggerData(const std::string& data)
{
    sender_->send(data);
}

int main()
{
    std::string envDatabase = getEnv("REPLAY_DATABASE");
    std::string envAddress = getEnv("REPLAY_ADDRESS");
    std::string envEnvelope = getEnv("REPLAY_ENVELOPE");

    reader_.reset(new DataReader(envDatabase));
    sender_.reset(new NetworkSender(envAddress, envEnvelope));

    std::cout << "Initing the Player..." << std::endl;
    player_.setCallback(triggerData);

    std::cout << "Reading the data..." << std::endl;
    player_.appendNewData(reader_->readData());

    std::cout << "Starting the player..." << std::endl;
    player_.start();

    //This is needed to keep the main-thread alive while the Player runs
    while (player_.isRunning())
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(200));
    }

    return 0;
}
