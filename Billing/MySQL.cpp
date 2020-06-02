#include <iostream>
#include "StringHelper.h"
#include "MySQL.h"
#include "Abonent.h"
#include "Street.h"
#include "Tarif.h"

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>


MySQL::MySQL() {
	server = StringHelper::New();
	strcpy_s(server, StringHelper::DefaultSize, "tcp://euronet.dn.ua:3306");
	schema = StringHelper::New();
	strcpy_s(schema, StringHelper::DefaultSize, "Kurs_billing");

	int X = Console::Width() / 2;
	int Y = Console::Height() / 2;
	char* temp = StringHelper::New();
	int Width = 35;
	int Height = 4;

	X -= 20;
	Y -= 2;

	Console::FillRect(X, Y, X + Width, Y + Height, Console::clCyan);
	X += 2;
	Y++;
	Console::GotoXY(X + 1, Y);
	Console::Print((char*)"логин: ", Console::clYellow, Console::clCyan);
	Console::GotoXY(X, Y + 2);
	Console::Print((char*)"пароль: ", Console::clYellow, Console::clCyan);

	Console::ShowCursor(true);
	X += 9;

	Console::FillRect(X - 1, Y, X + 15, Y, Console::clLightGrey);
	Console::FillRect(X - 1, Y + 2, X + 15, Y + 2, Console::clLightGrey);
	Console::GotoXY(X, Y);
	Console::SetColor(Console::clBlack, Console::clLightGrey);

	username = StringHelper::New();
	password = StringHelper::New();

	StringHelper::Input(username);

	Console::GotoXY(X, Y + 2);
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
		con->setSchema(schema);
	} catch (sql::SQLException e) {
		printf("Could not connect to server. Error message: %s", e.what());
		system("pause");
		exit(1);
	}
}

Street* MySQL::LoadStreets() {
	sql::ResultSet* result;
	Street* LResult = NULL;
	try {
		Connect();

		pstmt = con->prepareStatement("SELECT * FROM p_street ORDER BY street_name");
		result = pstmt->executeQuery();

		while (result->next()) {

			if (LResult) {
				LResult = LResult->ListAdd(new Street);
			} else {
				LResult = new Street;
			}

			LResult->id = result->getInt(1);
			strcpy_s(LResult->name, StringHelper::DefaultSize, result->getString(2).c_str());
	
		}
	} catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return LResult->ListFirst();
}

Tarif* MySQL::LoadTarifs() {
	sql::ResultSet* result;
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
	} catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return LResult->ListFirst();
}

Abonent* MySQL::LoadAbons(Street* StreetList, Tarif* TarifList) {
	sql::ResultSet* result;
	Abonent* LResult = NULL;
	try {
		Connect();

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
	} catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return LResult->ListFirst();
}

void MySQL::SaveAbon(Abonent* Abon, bool New, Street* StreetList, Tarif* TarifList) {

	try {
		Connect();

		if (!New) pstmt = con->prepareStatement("UPDATE `users` SET `fio` = ?, `login` = ?, `pass` = ?, `phone` = ?, `street` = ?, `house` = ?, `appart` = ?, `tarif` = ?, `ip` = ?, `balance` = ?, `state` = ? WHERE `users`.`id` = ?");
		else pstmt = con->prepareStatement("INSERT INTO `users` (`id`, `fio`, `login`, `pass`, `phone`, `street`, `house`, `appart`, `tarif`, `ip`, `balance`, `state`) VALUES (NULL, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

		pstmt->setString(1, Abon->fio);
		pstmt->setString(2, Abon->login);
		pstmt->setString(3, Abon->pass);
		pstmt->setString(4, Abon->Phone);
		pstmt->setInt(5, Abon->StreetPTR->id);
		pstmt->setInt(6, Abon->House);
		pstmt->setInt(7, Abon->Apartment);
		pstmt->setInt(8, Abon->TarifPTR->id);
		pstmt->setString(9, Abon->IP);
		pstmt->setInt(10, Abon->balance);
		pstmt->setInt(11, Abon->state);

		if (!New)pstmt->setInt(12, Abon->id);
		
		pstmt->executeQuery();

		
	} catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}

void MySQL::DelAbon(Abonent* Abon) {

	try {
		Connect();

		pstmt = con->prepareStatement("DELETE FROM `users` WHERE `users`.`id` = ?");
		pstmt->setInt(1, Abon->id);
		pstmt->executeQuery();


	} catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}