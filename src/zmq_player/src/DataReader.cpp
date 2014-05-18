
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

std::vector<std::string> DataReader::readData() const
{
    if (!connection_)
    {
        std::cout << "ERROR: No database loaded..." << std::endl;
        return std::vector<std::string>();
    }

    //This is the list of data which will be filled and returned to the user
    std::vector<std::string> data;

    //Let's read the data from the DB
    try
    {
        sqlite::query dataQuery(*connection_, "SELECT Data, Timestamp FROM Packets;");
        sqlite::query::iterator queryIter = dataQuery.begin();
        for (; queryIter != dataQuery.end(); ++queryIter)
        {
            data.push_back(queryIter->column< std::string >(0));

            //TODO: The following reads the timestamp
            //boost::int64_t timestamp = queryIter->column< boost::int64_t >(1);
        }
    }
    catch (sqlite::sqlite_error& error)
    {
        std::cout << __FUNCTION__ << ": " << error.what() << std::endl;
    }

    return data;
}
