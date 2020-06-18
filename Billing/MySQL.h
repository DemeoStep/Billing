#ifndef MYSQL_H
#define MYSQL_H
#include <mysql/jdbc.h>

class Street;
class Tarif;
class CellOper;
class FreeIP;

class MySQL {
public:
	char* server;
	char* username;
	char* password;
	char* schema;
	char* lastupdate;

	MySQL();
	~MySQL();

	void InputLoginPass();

	void Connect();

	void GetLastUpdatetime();

	Street* LoadStreets();

	CellOper* LoadCellCodes();

	FreeIP* LoadGreyIPs();
	FreeIP* LoadRealIPs();

	Tarif* LoadTarifs();

	sql::Driver* driver = sql::mysql::get_driver_instance();
	sql::Connection* con = NULL;
	sql::PreparedStatement* pstmt = NULL;

	Abonent* LoadAbons(Street* StreetList, Tarif* TarifList);
	void SaveAbon(Abonent* Abon, bool New, Street* StreetList, Tarif* TarifList);
	void DelAbon(Abonent* Abon);
	void RestoreGreyIP(char* IP);
	void DelGreyIP(FreeIP* IP);
	void RestoreRealIP(char* IP);
	void DelRealIP(FreeIP* IP);
	void SavePay(Abonent* Abon, int payment);
};

#endif