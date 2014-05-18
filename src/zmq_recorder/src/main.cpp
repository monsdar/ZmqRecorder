
#include "zmq_recorder/NetworkReceiver.h"
#include "zmq_recorder/DataStorage.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <map>
#include <iostream>

DataStorage storage_;

void receivedData(const std::string& data)
{
    std::cout << "Received Data..." << std::endl;

    //NOTE: The outcommented code is a very basic way to profile the code.
    //      What about using CxxProf here? How do we need to set up everything for this to work?
    //boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();
    storage_.storeData(data);
    //boost::posix_time::time_duration const diff = boost::posix_time::microsec_clock::local_time() - start;
    //std::cout << "Storing took " << diff.total_milliseconds() << " milliseconds" << std::endl;
}

int main()
{
    //The receiver will connect itself to everything it needs
    NetworkReceiver receiver( std::string("tcp://localhost:15232") ); //TODO: Make this configurable
    receiver.setCallback(receivedData);
    receiver.startListening();
    std::cout << "Listening..." << std::endl;

    //This is needed to keep the main-thread alive while the NetworkReceiver runs
    while(true)
    {
        boost::this_thread::sleep( boost::posix_time::milliseconds(200) );
    }

    return 0;
}
