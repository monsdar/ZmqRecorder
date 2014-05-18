
#include "zmq_player/NetworkSender.h"
#include "zmq_player/DataReader.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <map>
#include <iostream>

int main()
{
    std::cout << "Binding the sender..." << std::endl;
    NetworkSender sender( std::string("tcp://*:15232") ); //TODO: Make this configurable

    std::cout << "Reading the data..." << std::endl;
    DataReader reader("test.db"); //TODO: Make this configurable
    //TODO: The following ignores the timestamps completely
    std::vector<std::string> data = reader.readData();

    std::cout << "Sending the data..." << std::endl;
    std::vector<std::string>::iterator dataIter = data.begin();
    for (; dataIter != data.end(); ++dataIter)
    {
        sender.send(*dataIter);
    }

    return 0;
}
