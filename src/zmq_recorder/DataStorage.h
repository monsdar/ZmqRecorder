
#ifndef _DATA_STORAGE_H_
#define _DATA_STORAGE_H_

#include "zmq_recorder/Packet.h"

#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <sqlite3cc.h>

class DataStorage
{
public:
    /**
     * Creates a new database and stores the needed tables in it
     *  * Activities
     *  * Marks
     *  * Plots
     */
    DataStorage();
    virtual ~DataStorage();

    /**
     * Stores the given data into the database
     */
    void storeData(const std::string& data);

private:
    /**
     * Initializes a new database with the given name for
     *  * Activities
     *  * Marks
     *  * Plots
     */
    void initDatabase(const std::string& filename);
    /**
     * This function returns the current DateStr in the following form:
     * DDMonYYYY_HHMMSS -> 31Apr2014_235959
     */
    std::string getCurrentDateStr();
    /**
     * Returns the current time in microseconds as a int64_t timestamp
     */
    boost::int64_t getTimestamp();
    /**
     * Adds the given data to the DB
     */
    void updateData(const zmq_recorder::Packet& data);
    /**
     * Stores the data
     */
    void storageLoop();

    /**
     * This is the connection to the database
     */
    boost::shared_ptr<sqlite::connection> connection_;

    /**
     * Stores the time when the recorder has been started
     */
    boost::shared_ptr<boost::posix_time::ptime> start_;

    /**
     * This thread stores the given data periodically
     */
    boost::shared_ptr<boost::thread> storageThread_;
    /**
     * Secures the data
     */
    boost::mutex storageMutex_;
    /**
     * Holds the data until it is stored
     */
    std::vector<zmq_recorder::Packet> dataQueue_;
    /**
     * This condition variable waits until data has been received
     */
    boost::condition_variable queueEmptyCondition_;
    /**
     * Determines whether the storageThread is running or not
     */
    bool isRunning_;

};

#endif //_DATA_STORAGE_H_
