#include <iostream>
#include <ctime>
#include "Kernel.h"
#include "../Billing/MySQL.h"
#include "../Billing/Abonent.h"
#include "../Billing/Tarif.h"
#include "../Billing/Street.h"
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



	} else {
		printf("Файл %s отсутсвует! Нажмите любую клавишу для выхода", filename);
		getchar();
		exit (1);
	}

}

int main() {
	setlocale(LC_ALL, "rus");
	SetConsoleCP(.1251);
	SetConsoleOutputCP(.1251);

	Connection = new MySQL;
	LoadConfig();
	printf("OK\n");
	printf("Загружаем...\n");

	printf("...тарифы\n");
	Tarifs = Connection->LoadTarifs();
	Streets = Connection->LoadStreets();
	printf("...данные абонентов\n");
	Abonents = Connection->LoadAbons(Streets, Tarifs);

	printf("ОК!\n");


}
