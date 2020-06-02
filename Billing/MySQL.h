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
	char* schema;

	MySQL();
	~MySQL();

	void Connect();

	Street* LoadStreets();

	Tarif* LoadTarifs();

	sql::Driver* driver;
	sql::Connection* con;
	//sql::Statement* stmt;
	sql::PreparedStatement* pstmt;

	Abonent* LoadAbons(Street* StreetList, Tarif* TarifList);
	void SaveAbon(Abonent* Abon, bool New, Street* StreetList, Tarif* TarifList);
	void DelAbon(Abonent* Abon);
};

#endif