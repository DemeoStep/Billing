#ifndef MYSQL_H
#define MYSQL_H

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>

class Abonent;
class Tarif;

class MySQL {
public:
	char* server;
	char* username;
	char* password;
	char* schema;
	char* lastupdate;

	MySQL();
	~MySQL();

	sql::Driver* driver;
	sql::Connection* con = NULL;
	sql::PreparedStatement* pstmt = NULL;

	void Connect();
	void GetLastUpdatetime();

	Tarif* LoadTarifs();
	Abonent* LoadAbons(Tarif* TarifList);

	void SaveAbon(Abonent* Abon);
	void SavePay(Abonent* Abon, int payment);
};

#endif