#ifndef MYSQL_H
#define MYSQL_H
#include <mysql/jdbc.h>

class Street;
class Tarif;
class CellOper;
class Free_grey_IP;
class Free_real_IP;

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

	Free_grey_IP* LoadGreyIPs();
	Free_real_IP* LoadRealIPs();

	Tarif* LoadTarifs();

	sql::Driver* driver = sql::mysql::get_driver_instance();
	sql::Connection* con = NULL;
	sql::PreparedStatement* pstmt = NULL;

	Abonent* LoadAbons(Street* StreetList, Tarif* TarifList);
	void SaveAbon(Abonent* Abon, bool New, Street* StreetList, Tarif* TarifList);
	void DelAbon(Abonent* Abon);
	void RestoreGreyIP(char* IP);
	void DelGreyIP(Free_grey_IP* IP);
	void RestoreRealIP(char* IP);
	void DelRealIP(Free_real_IP* IP);
	void SavePay(Abonent* Abon, int payment);
};

#endif