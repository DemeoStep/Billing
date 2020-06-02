#ifndef MYSQL_H
#define MYSQL_H

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>

class Street;
class Tarif;

class MySQL {
public:
	char* server;
	char* username;
	char* password;

	MySQL();
	~MySQL();

	void Connect();

	sql::Driver* driver;
	sql::Connection* con;
	//sql::Statement* stmt;
	sql::PreparedStatement* pstmt;

	Abonent* Load(Street* StreetList, Tarif* TarifList);
};

#endif