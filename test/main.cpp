#include <iostream>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/driver.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/metadata.h"
#include "cppconn/exception.h"

int main() {
	const char* user = "basta";
	const char* passwd = "yd0927HK#";
	const char* host = "tcp://123.56.8.254:3306";
	const char* database = "BusDB";
	try {
		sql::mysql::MySQL_Driver* driver =sql::mysql::get_mysql_driver_instance();
		sql::Connection* conn = driver->connect(host, user, passwd);
		conn->setSchema(database);
		sql::Statement *stmt = conn->createStatement();

		sql::ResultSet * res = stmt->executeQuery("CALL selectLastHourInfo()");
		while (res->next()){
			printf("phone:%s,pwd:%s,timestamp:%s\r\n", res->getString("phone").c_str(),\
				res->getString("pwd").c_str(), res->getString("timeseconds").c_str());
		}
		delete res;
		delete stmt;
		delete conn;
	}
	catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	getchar();
	return 0;
}