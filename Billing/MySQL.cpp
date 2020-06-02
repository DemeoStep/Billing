#include <iostream>
#include "StringHelper.h"
#include "MySQL.h"
#include "Abonent.h"
#include "Street.h"
#include "Tarif.h"

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>


MySQL::MySQL() {
	server = StringHelper::New();
	strcpy_s(server, StringHelper::DefaultSize, "tcp://euronet.dn.ua:3306");
	username = StringHelper::New();
	password = StringHelper::New();

	StringHelper::Input(username);
	StringHelper::InputPass(password);

}

MySQL::~MySQL() {
	free(server);
	free(username);
	free(password);
}

void MySQL::Connect() {
	try {
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
		printf("Connected");
	} catch (sql::SQLException e) {
		printf("Could not connect to server. Error message: %s", e.what());
		system("pause");
		exit(1);
	}
}

Abonent* MySQL::Load(Street* StreetList, Tarif* TarifList) {
	sql::ResultSet* result;
	Abonent* LResult = NULL;
	try {
		Connect();

		con->setSchema("Kurs_billing");

		pstmt = con->prepareStatement("SELECT * FROM users ORDER BY fio;");
		result = pstmt->executeQuery();

		while (result->next()) {

			if (LResult) {
				LResult = LResult->ListAdd(new Abonent);
			} else {
				LResult = new Abonent;
			}

			LResult->id = result->getInt(1);
			strcpy_s(LResult->fio, StringHelper::DefaultSize, result->getString(2).c_str());
			strcpy_s(LResult->login, StringHelper::DefaultSize, result->getString(3).c_str());
			strcpy_s(LResult->pass, StringHelper::DefaultSize, result->getString(4).c_str());
			strcpy_s(LResult->Phone, StringHelper::DefaultSize, result->getString(5).c_str());
			LResult->StreetPTR = StreetList->Get_by_id(result->getInt(6));
			LResult->House = result->getInt(7);
			LResult->Apartment = result->getInt(8);
			LResult->TarifPTR = TarifList->Get_by_id(result->getInt(9));
			strcpy_s(LResult->IP, StringHelper::DefaultSize, result->getString(10).c_str());
			LResult->balance = result->getInt(11);
			LResult->state = result->getInt(12);
		}
		return LResult->ListFirst();
	} catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}

}