#include <iostream>
#include <ctime>
#include "Kernel.h"
#include "MySQL.h"
#include "../Billing/Abonent.h"
#include "../Billing/Tarif.h"
#include "../Billing/StringHelper.h"

void LoadConfig() {
	printf("Загружаем конфиг...");
	char* filename = StringHelper::New();
	strcpy_s(filename, StringHelper::DefaultSize, "config.cfg");

	FILE* LFile;

	int FileOpenError = fopen_s(&LFile, filename, "r");
	if (!FileOpenError) {
		char** context = (char**)calloc(StringHelper::DefaultSize, sizeof(char*));
		char* buffer = StringHelper::New();

		fgets(buffer, StringHelper::DefaultSize * sizeof(char), LFile);
			
		strcpy_s(Connection->server, StringHelper::DefaultSize, "tcp://");
		strcat_s(Connection->server, StringHelper::DefaultSize, strtok_s(buffer, "|", context));
		strcat_s(Connection->server, StringHelper::DefaultSize, ":");
		strcat_s(Connection->server, StringHelper::DefaultSize, strtok_s(NULL, "|", context));

		strcpy_s(Connection->username, StringHelper::DefaultSize, strtok_s(NULL, "|", context));
		strcpy_s(Connection->password, StringHelper::DefaultSize, strtok_s(NULL, "\n", context));
		
		printf("");

		free(context);
		free(buffer);
		fclose(LFile);
		printf("OK\n");

	} else {
		printf("Файл %s отсутсвует! Нажмите любую клавишу для выхода", filename);
		getchar();
		exit (1);
	}

}

void LoadLists() {
	printf("Загружаем...\n");

	printf("...тарифы\n");
	Tarifs = Connection->LoadTarifs();
	printf("...данные абонентов\n");
	Abonents = Connection->LoadAbons(Tarifs);
	printf("OK\n");
}

void ListsNULL() {
	Abonents = Abonents->ListLast();
	while (Abonents) {
		Abonent* Temp = Abonents;
		Abonents = Abonents->ListPrev;
		delete Temp;
	}
	Abonents = NULL;

	Tarifs = Tarifs->ListLast();
	while (Tarifs) {
		Tarif* Temp = Tarifs;
		Tarifs = Tarifs->ListPrev;
		delete Temp;
	}
	Tarifs = NULL;
};

void GetTime() {
	now = time(0);
	localtime_s(&ltm, &now);
}

int GetDays_in_Mon() {
	int days = 0;
	switch (ltm.tm_mon) {
	case 0: days = 31; break;					//январь
	case 1: {
		if ((ltm.tm_year+1900) % 400 == 0) days = 29;
		else if ((ltm.tm_year + 1900) % 100 == 0) days = 28;
		else if ((ltm.tm_year + 1900) % 4 == 0) days = 29;
		else days = 28;
	} break;									//февраль
	case 2: days = 31; break;					//март
	case 3: days = 30; break;					//апрель
	case 4: days = 31; break;					//май
	case 5: days = 30; break;					//июнь
	case 6: days = 31; break;					//июль
	case 7: days = 31; break;					//август
	case 8: days = 30; break;					//сентябрь
	case 9: days = 31; break;					//октябрь
	case 10: days = 30; break;					//ноябрь
	case 11: days = 31; break;					//декабрь
	}
	return days;
}

//int DayPayCount() {
//
//}

int main() {
	setlocale(LC_ALL, "rus");
	SetConsoleCP(.1251);
	SetConsoleOutputCP(.1251);

	Connection = new MySQL;
	LoadConfig();
	LoadLists();
	char* last_our_time = StringHelper::New();
	strcpy_s(last_our_time, StringHelper::DefaultSize, Connection->lastupdate);

	/*while (true) {
		Connection->GetLastUpdatetime();
		if (strcmp(last_our_time, Connection->lastupdate)) {
			printf("Данные обновились - обновляем данные в памяти... \n");
			ListsNULL();
			LoadLists();
			strcpy_s(last_our_time, StringHelper::DefaultSize, Connection->lastupdate);
		}
		GetTime();
		if (ltm.tm_hour == 14 && ltm.tm_min == 10) {
			Abonent* List = Abonents->ListFirst();
			while (List) {
				if (!List->need_pay) {
					List->need_pay = true;
					Connection->SaveAbon(List);
				}
				List = List->ListNext;
			}

		}
		Sleep(1000);
	}*/
	GetTime();
	
	printf("%d %d %d", ltm.tm_year + 1900, ltm.tm_mon, GetDays_in_Mon());

}
