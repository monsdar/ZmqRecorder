
#include "zmq_player/DataReader.h"

#include <boost/filesystem.hpp>

DataReader::DataReader( const std::string& dbName )
{
    //check if the db exists
    if (boost::filesystem::exists(dbName))
    {
        connection_.reset(new sqlite::connection(dbName));
    }
}

DataReader::~DataReader()
{
    if (connection_)
    {
        connection_->close();
    }
}

std::vector<zmq_player::Packet> DataReader::readData() const
{
    if (!connection_)
    {
        std::cout << "ERROR: No database loaded..." << std::endl;
        return std::vector<zmq_player::Packet>();
    }

    //This is the list of data which will be filled and returned to the user
    std::vector<zmq_player::Packet> data;

    //Let's read the data from the DB
    try
    {
        sqlite::query dataQuery(*connection_, "SELECT Data, Timestamp FROM Packets;");
        sqlite::query::iterator queryIter = dataQuery.begin();
        for (; queryIter != dataQuery.end(); ++queryIter)
        {
            zmq_player::Packet newPacket;
            newPacket.Data = queryIter->column< std::string >(0);
            newPacket.Timestamp = queryIter->column< boost::int64_t >(1);

            data.push_back(newPacket);
        }
    }
    catch (sqlite::sqlite_error& error)
    {
        std::cout << __FUNCTION__ << ": " << error.what() << std::endl;
    }

    return data;
}
