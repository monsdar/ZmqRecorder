
#include "zmq_player/DataReader.h"
#include "zmq_player/NetworkSender.h"
#include "zmq_player/Player.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <map>
#include <iostream>

DataReader reader_("test.db"); //TODO: Make this configurable
Player player_;
NetworkSender sender_(std::string("tcp://*:15232")); //TODO: Make this configurable

//this method will be called by our Player everytime the data should be send
void triggerData(const std::string& data)
{
    sender_.send(data);
}

int main()
{
    std::cout << "Initing the Player..." << std::endl;
    player_.setCallback(triggerData);

    std::cout << "Reading the data..." << std::endl;
    player_.appendNewData(reader_.readData());

    std::cout << "Starting the player..." << std::endl;
    player_.start();

    return 0;
}