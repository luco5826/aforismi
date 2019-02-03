#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <algorithm>
#include <random>

#include "Aforisma.hpp"

using std::cout;
using std::endl;

using afo::Aforisma;

const string username = "";
const string password = "";
const string dbName = "";


int main()
{

    std::vector<Aforisma *> aforismi = afo::popolateVectorFromFile("Aforismi-tot.txt");

    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(aforismi), std::end(aforismi), rng);

    std::stringstream stream;
    stream << "INSERT INTO AFORISMI VALUES";
    for (Aforisma *a : aforismi)
    {
        stream << "(@ID, \'";
        stream << a->getFrase();
        stream << "\',\'";
        stream << a->getAutore();
        stream << "\'),";
    }
    std::string query = stream.str();
    boost::replace_last(query, ",", ";");

    try
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("tcp://sql7.freemysqlhosting.net:3306", username, password);
        /* Connect to the MySQL test database */
        con->setSchema(dbName);

        stmt = con->createStatement();
        stmt->execute(query);

        delete stmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

    cout << endl;

    return EXIT_SUCCESS;
}