
#ifndef _DATA_READER_H_
#define _DATA_READER_H_

#include <boost/shared_ptr.hpp>
#include <sqlite3cc.h>
#include <vector>

class DataReader
{
public:
    /**
     * Creates a new datareader with the given Sqlite db as source
     */
    explicit DataReader(const std::string& dbName);
    virtual ~DataReader();

    /**
     * Reads all the data at once and returns it in a vector
     * TODO: This ignores the timestamps...
     * TODO: This reads all the data at once, it should rather stream it...
     */
    std::vector<std::string> readData() const;

private:
    /**
     * This is the connection to the database
     */
    boost::shared_ptr<sqlite::connection> connection_;

};

#endif //_DATA_READER_H_
