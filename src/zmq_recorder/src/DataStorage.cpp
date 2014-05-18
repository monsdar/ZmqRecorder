
#include "zmq_recorder/DataStorage.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#include <boost/filesystem.hpp>

#include <stdlib.h> // needed for exit()

DataStorage::DataStorage() :
    start_( boost::posix_time::microsec_clock::local_time() )
{
    //create a new database for each session
    std::string dbName = "session_" + getCurrentDateStr() + ".db";
    bool isInitNeeded = !boost::filesystem::exists(dbName);
    connection_.reset(new sqlite::connection(dbName));

    //NOTE: Init is always needed, because we're creating a new database at each startup
    //      Perhaps we should treat it like an error if there is no init needed...
    if (isInitNeeded)
    {
        initDatabase(dbName);
    }
}

DataStorage::~DataStorage()
{
    if (connection_)
    {
        connection_->close();
    }
}

std::string DataStorage::getCurrentDateStr()
{
    boost::posix_time::ptime currentTime = boost::posix_time::second_clock::local_time();
    static std::locale loc(std::cout.getloc(), new boost::posix_time::time_facet("%d%b%Y_%H%M%S"));
    std::ostringstream dtStream;
    dtStream.imbue(loc);
    dtStream << currentTime;

    return dtStream.str();
}

void DataStorage::initDatabase(const std::string& filename)
{
    //create a deferred transaction for the following commands
    //Simple explanation: If we do not wrap everything into a transaction here the performance would be VERY bad
    sqlite::transaction_guard< > transactionGuard(*connection_);

    try
    {
        //Create table for the Packets
        sqlite::command initPacketCommand(*connection_,
            "CREATE TABLE Packets ( "
            "   PacketId INTEGER PRIMARY KEY AUTOINCREMENT, "
            "   Data TEXT, "
            "   Timestamp INTEGER"
            ")");

        initPacketCommand.exec();
    }
    catch (sqlite::sqlite_error& error)
    {
        //If something went wrong during initialization of the database we should fail hard and early
        std::cout << __FUNCTION__ << ": " << error.what() << std::endl;
        exit(1);
    }

    //commit the transaction (or it will rollback)
    transactionGuard.commit();
}

void DataStorage::storeData(const std::string& data)
{
    if (!connection_)
    {
        std::cout << "ERROR: No database loaded..." << std::endl;
        return;
    }

    //create a deferred transaction for the following commands
    //Simple explanation: If we do not wrap everything into a transaction here the performance would be VERY bad
    sqlite::transaction_guard< > transactionGuard(*connection_);

    //Add the data to the packets
    updateData(data);

    //commit the transaction (or it will rollback)
    transactionGuard.commit();
}

void DataStorage::updateData(const std::string& data)
{
    try
    {
        sqlite::command insertCommand(*connection_,
            "INSERT INTO Packets ( "
            "Data, "
            "Timestamp) "
            "VALUES ( "
            "?, " //Data
            "? " //Timestamp
            ");");

        insertCommand.bind(1, data);
        insertCommand.bind(2, getTimestamp()); //this gets the current timestamp

        //We do not use the error-code here, but it is good to know that there is one if we need it
        int errorNum = insertCommand.exec();
    }
    catch (sqlite::sqlite_error& error)
    {
        std::cout << __FUNCTION__ << ": " << error.what() << std::endl;
    }
}

boost::int64_t DataStorage::getTimestamp()
{
    boost::posix_time::time_duration const diff = boost::posix_time::microsec_clock::local_time() - start_;
    return diff.total_milliseconds();
}
