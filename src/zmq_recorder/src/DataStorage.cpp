
#include "zmq_recorder/DataStorage.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#include <boost/filesystem.hpp>

#include <stdlib.h> // needed for exit()

DataStorage::DataStorage() :
    start_( boost::posix_time::microsec_clock::local_time() ),
    isRunning_(false)
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

    //start the storagethread
    storageThread_.reset(new boost::thread(boost::bind(&DataStorage::storageLoop, this)));
}

DataStorage::~DataStorage()
{
    isRunning_ = false;

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

    boost::mutex::scoped_lock lock(storageMutex_);
    dataQueue_.push_back(data);
    queueEmptyCondition_.notify_all(); //notify the thread that we finally have data
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

void DataStorage::storageLoop()
{
    isRunning_ = true;
    while (isRunning_)
    {
        //wait until we have data in our queue
        {
            boost::mutex::scoped_lock dataLock(storageMutex_);
            while (dataQueue_.empty())
            {
                queueEmptyCondition_.wait(dataLock);
            }
        }

        //lock the data again for as long as we're storing it
        unsigned int numPackets = 0;
        {
            boost::mutex::scoped_lock dataLock(storageMutex_);
            //create a deferred transaction for the following commands
            //Simple explanation: If we do not wrap everything into a transaction here the performance would be VERY bad
            sqlite::transaction_guard< > transactionGuard(*connection_);

            std::vector<std::string>::iterator dataIter = dataQueue_.begin();
            for (; dataIter != dataQueue_.end(); ++dataIter)
            {
                updateData(*dataIter);
            }
            numPackets = dataQueue_.size();
            dataQueue_.clear();

            //commit the transaction (or it will rollback)
            transactionGuard.commit();
        }

        std::cout << "Stored " << numPackets << " packet(s)" << std::endl; 

        //sleep some time, this avoids that we do too many transmissions to the database at once
        //it also frees up some time for the CPU
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
}

boost::int64_t DataStorage::getTimestamp()
{
    boost::posix_time::time_duration const diff = boost::posix_time::microsec_clock::local_time() - start_;
    return diff.total_microseconds();
}
