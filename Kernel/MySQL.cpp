#include <iostream>
#include "MySQL.h"
#include "../Billing/Abonent.h"
#include "../Billing/StringHelper.h"
#include "../Billing/Tarif.h"

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>


MySQL::MySQL() {
	lastupdate = StringHelper::New();
	driver = get_driver_instance();
	server = StringHelper::New();
	schema = StringHelper::New();
	username = StringHelper::New();
	password = StringHelper::New();
}

MySQL::~MySQL() {
	free(server);
	free(schema);
	free(username);
	free(password);
	free(lastupdate);
}


void MySQL::Connect() {
	try {
		con = driver->connect(server, username, password);
		con->setSchema(schema);
	} catch (sql::SQLException e) {
		printf("Could not connect to server. Error message: %s", e.what());
		system("pause");
		exit(1);
	}
}

void MySQL::GetLastUpdatetime() {
	sql::ResultSet* result = NULL;
	try {
		Connect();

		pstmt = con->prepareStatement("SELECT UPDATE_TIME FROM information_schema.tables WHERE TABLE_SCHEMA = ? ORDER BY UPDATE_TIME DESC LIMIT 1");
		pstmt->setString(1, schema);
		result = pstmt->executeQuery();

		while (result->next()) {
			strcpy_s(lastupdate, StringHelper::DefaultSize, result->getString(1).c_str());

		}
		con->close();
		delete con;
		delete result;
		delete pstmt;
	} catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
};

Tarif* MySQL::LoadTarifs() {
	sql::ResultSet* result = NULL;
	Tarif* LResult = NULL;
	try {
		Connect();

		pstmt = con->prepareStatement("SELECT * FROM tarifs ORDER BY id");
		result = pstmt->executeQuery();

		while (result->next()) {

			if (LResult) {
				LResult = LResult->ListAdd(new Tarif);
			} else {
				LResult = new Tarif;
			}

			LResult->id = result->getInt(1);
			strcpy_s(LResult->name, StringHelper::DefaultSize, result->getString(2).c_str());
			LResult->isReal = result->getBoolean(3);
			LResult->price = result->getInt(4);
		}
		con->close();
		delete result;
		delete pstmt;
		delete con;
	} catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return LResult->ListFirst();
}

Abonent* MySQL::LoadAbons(Tarif* TarifList) {
	sql::ResultSet* result = NULL;
	sql::ResultSet* bal_result = NULL;
	Abonent* LResult = NULL;
	int index = 0;
	try {
		Connect();

		pstmt = con->prepareStatement("SELECT id,fio,tarif,state,last_pay FROM users ORDER BY id;");
		result = pstmt->executeQuery();

		while (result->next()) {

			if (LResult) {
				LResult = LResult->ListAdd(new Abonent);
			} else {
				LResult = new Abonent;
			}
			index++;
			LResult->id = result->getInt(1);
			strcpy_s(LResult->fio, StringHelper::DefaultSize, result->getString(2).c_str());

			LResult->TarifPTR = TarifList->Get_by_id(result->getInt(3));
		
			LResult->state = result->getInt(4);
			strcpy_s(LResult->last_pay, StringHelper::DefaultSize, result->getString(5).c_str());

			pstmt = con->prepareStatement("SELECT ROUND(SUM(money)) FROM Kurs_billing.pays WHERE user_id = ?;");
			pstmt->setInt(1, LResult->id);
			bal_result = pstmt->executeQuery();
			while (bal_result->next()) {
				LResult->balance = bal_result->getInt(1);
			}
		}
		con->close();
		delete con;
		delete result;
		delete bal_result;
		delete pstmt;
	} catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}

	return LResult->ListFirst();
}

void MySQL::SaveAbon(Abonent* Abon) {
	sql::ResultSet* result = NULL;
	try {
		Connect();

		pstmt = con->prepareStatement("UPDATE `users` SET `state` = ?, `last_pay` = ? WHERE `users`.`id` = ?");

		pstmt->setInt(1, Abon->state);
		pstmt->setString(2, Abon->last_pay);

		pstmt->setInt(3, Abon->id);
		
		pstmt->executeQuery();

		con->close();
		delete result;
		delete pstmt;
		delete con;
	} catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}

void MySQL::SavePay(Abonent* Abon, int payment) {
	sql::ResultSet* result = NULL;
	char* temp = StringHelper::New();
	strcpy_s(temp, StringHelper::DefaultSize, "Ежедневная абонплата ");
	strcat_s(temp, StringHelper::DefaultSize, username);
	try {
		Connect();
		
		pstmt = con->prepareStatement("INSERT INTO `pays` (`user_id`, `money`, `reason`) VALUES (?, ?, ?)");

		pstmt->setInt(1, Abon->id);
		pstmt->setInt(2, payment);
		pstmt->setString(3, temp);
		pstmt->executeQuery();

		con->close();
		delete result;
		delete pstmt;
		delete con;
		free(temp);

	} catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	printf("");
}