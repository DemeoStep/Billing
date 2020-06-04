#include <iostream>
#include <ctime>
#include "StringHelper.h"
#include "MySQL.h"
#include "Abonent.h"
#include "Street.h"
#include "Tarif.h"
#include "CellOper.h"
#include "Free_grey_IP.h"
#include "Free_real_IP.h"

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>


MySQL::MySQL() {
	lastupdate = StringHelper::New();
	driver = get_driver_instance();
	server = StringHelper::New();
	strcpy_s(server, StringHelper::DefaultSize, "tcp://192.168.12.2:3306");
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

Street* MySQL::LoadStreets() {
	sql::ResultSet* result = NULL;
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
	return LResult->ListFirst();
}

CellOper* MySQL::LoadCellCodes() {
	sql::ResultSet* result = NULL;
	CellOper* LResult = NULL;
	try {
		Connect();

		pstmt = con->prepareStatement("SELECT * FROM cell_oper");
		result = pstmt->executeQuery();

		while (result->next()) {

			if (LResult) {
				LResult = LResult->ListAdd(new CellOper);
			} else {
				LResult = new CellOper;
			}

			strcpy_s(LResult->code, StringHelper::DefaultSize, result->getString(1).c_str());

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

Free_grey_IP* MySQL::LoadGreyIPs() {
	sql::ResultSet* result = NULL;
	Free_grey_IP* LResult = NULL;
	try {
		Connect();

		pstmt = con->prepareStatement("SELECT * FROM free_grey_ip ORDER BY INET_ATON(ip)");
		result = pstmt->executeQuery();

		while (result->next()) {

			if (LResult) {
				LResult = LResult->ListAdd(new Free_grey_IP);
			} else {
				LResult = new Free_grey_IP;
			}

			strcpy_s(LResult->ip, StringHelper::DefaultSize, result->getString(1).c_str());

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

Free_real_IP* MySQL::LoadRealIPs() {
	sql::ResultSet* result = NULL;
	Free_real_IP* LResult = NULL;
	try {
		Connect();

		pstmt = con->prepareStatement("SELECT * FROM free_real_ip ORDER BY INET_ATON(ip)");
		result = pstmt->executeQuery();

		while (result->next()) {

			if (LResult) {
				LResult = LResult->ListAdd(new Free_real_IP);
			} else {
				LResult = new Free_real_IP;
			}

			strcpy_s(LResult->ip, StringHelper::DefaultSize, result->getString(1).c_str());

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

Abonent* MySQL::LoadAbons(Street* StreetList, Tarif* TarifList) {
	sql::ResultSet* result = NULL;
	sql::ResultSet* bal_result = NULL;
	Abonent* LResult = NULL;
	int index = 0;
	try {
		Connect();

		pstmt = con->prepareStatement("SELECT id,fio,login,AES_DECRYPT(pass,'hardpassword'),phone,street,house,appart,tarif,ip,state FROM users ORDER BY fio;");
		result = pstmt->executeQuery();

		while (result->next()) {

			if (LResult) {
				LResult = LResult->ListAdd(new Abonent);
			} else {
				LResult = new Abonent;
			}
			index++;
			LResult->id = result->getInt(1);
			LResult->index = index;
			strcpy_s(LResult->fio, StringHelper::DefaultSize, result->getString(2).c_str());
			strcpy_s(LResult->login, StringHelper::DefaultSize, result->getString(3).c_str());
			strcpy_s(LResult->pass, StringHelper::DefaultSize, result->getString(4).c_str());
			strcpy_s(LResult->Phone, StringHelper::DefaultSize, result->getString(5).c_str());
			LResult->StreetPTR = StreetList->Get_by_id(result->getInt(6));
			LResult->House = result->getInt(7);
			LResult->Apartment = result->getInt(8);
			LResult->TarifPTR = TarifList->Get_by_id(result->getInt(9));
			strcpy_s(LResult->IP, StringHelper::DefaultSize, result->getString(10).c_str());
			LResult->state = result->getInt(11);

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

void MySQL::SaveAbon(Abonent* Abon, bool New, Street* StreetList, Tarif* TarifList) {
	sql::ResultSet* result = NULL;
	try {
		Connect();

		if (!New) pstmt = con->prepareStatement("UPDATE `users` SET `fio` = ?, `login` = ?, `pass` = AES_ENCRYPT(?, 'hardpassword'), `phone` = ?, `street` = ?, `house` = ?, `appart` = ?, `tarif` = ?, `ip` = ?, `state` = ? WHERE `users`.`id` = ?");
		else pstmt = con->prepareStatement("INSERT INTO `users` (`fio`, `login`, `pass`, `phone`, `street`, `house`, `appart`, `tarif`, `ip`, `state`) VALUES (?, ?, AES_ENCRYPT(?, 'hardpassword'), ?, ?, ?, ?, ?, ?, ?)");

		pstmt->setString(1, Abon->fio);
		pstmt->setString(2, Abon->login);
		pstmt->setString(3, Abon->pass);
		pstmt->setString(4, Abon->Phone);
		pstmt->setInt(5, Abon->StreetPTR->id);
		pstmt->setInt(6, Abon->House);
		pstmt->setInt(7, Abon->Apartment);
		pstmt->setInt(8, Abon->TarifPTR->id);
		pstmt->setString(9, Abon->IP);
		pstmt->setInt(10, Abon->state);

		if (!New)pstmt->setInt(11, Abon->id);
		
		pstmt->executeQuery();

		if (New) {
			pstmt = con->prepareStatement("SELECT * FROM users WHERE login = ?");
			pstmt->setString(1, Abon->login);
			result = pstmt->executeQuery();
			while (result->next()) {
				Abon->id = result->getInt(1);
			}
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
}

void MySQL::DelAbon(Abonent* Abon) {

	try {
		Connect();

		pstmt = con->prepareStatement("DELETE FROM `users` WHERE `users`.`id` = ?");
		pstmt->setInt(1, Abon->id);
		pstmt->executeQuery();

		con->close();
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

void MySQL::RestoreGreyIP(char* IP) {

	try {
		Connect();

		pstmt = con->prepareStatement("INSERT INTO free_grey_ip VALUES (?)");
		pstmt->setString(1, IP);
		pstmt->executeQuery();

		con->close();
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

void MySQL::DelGreyIP(Free_grey_IP* IP) {

	try {
		Connect();

		pstmt = con->prepareStatement("DELETE FROM free_grey_ip WHERE `ip` = ?");
		pstmt->setString(1, IP->ip);
		pstmt->executeQuery();

		con->close();
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

void MySQL::RestoreRealIP(char* IP) {

	try {
		Connect();

		pstmt = con->prepareStatement("INSERT INTO free_real_ip VALUES (?)");
		pstmt->setString(1, IP);
		pstmt->executeQuery();

		con->close();
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

void MySQL::DelRealIP(Free_real_IP* IP) {

	try {
		Connect();

		pstmt = con->prepareStatement("DELETE FROM free_real_ip WHERE `ip` = ?");
		pstmt->setString(1, IP->ip);
		pstmt->executeQuery();

		con->close();
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
	strcpy_s(temp, StringHelper::DefaultSize, "Изменение баланса оператором ");
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