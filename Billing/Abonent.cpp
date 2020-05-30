#include <iostream>
#include "Abonent.h"
#include "StringHelper.h"
#include <string>
#include "Console.h"
#include "Application.h"
#include "Tarif.h"
#include "Free_grey_IP.h"
#include "Street.h"

class Application;

Abonent::Abonent() {
	id = 0;
	fio = StringHelper::New(StringHelper::DefaultSize);
	login = StringHelper::New(StringHelper::DefaultSize);
	pass = StringHelper::New(StringHelper::DefaultSize);
	Phone = StringHelper::New(StringHelper::DefaultSize);
	StreetPTR = NULL;
	House = 0;
	Apartment = 0;
	TarifPTR = NULL;
	IP = StringHelper::New(StringHelper::DefaultSize);
	balance = 0;
	state = 0;

	ListNext = NULL;
	ListPrev = NULL;
}

Abonent::~Abonent() {
	if (NULL != ListPrev) {
		ListPrev->ListNext = ListNext;
	}
	if (NULL != ListNext) {
		ListNext->ListPrev = ListPrev;
	}
	free (fio);
	free (Phone);
	free (login);
	free (pass);
	free (IP);
}

void Abonent::GenTest() {
	id = rand() % 2000;
	char* FirstName = StringHelper::New(StringHelper::DefaultSize);
	char* SecondName = StringHelper::New(StringHelper::DefaultSize);
	char* LastName = StringHelper::New(StringHelper::DefaultSize);

	const char* LC_FirstName[10] = {
		"Иванов\0",
		"Петров\0",
		"Сидоров\0",
		"Андреев\0",
		"Потапов\0",
		"Константинов\0",
		"Юрьев\0",
		"Антонов\0",
		"Сергеев\0",
		"Васильев\0"
	};
	strcpy_s(FirstName, StringHelper::DefaultSize, LC_FirstName[rand() % 10]);

	const char* LC_SecondName[10] = {
		"Иван\0",
		"Петр\0",
		"Сидор\0",
		"Андрей\0",
		"Потап\0",
		"Константин\0",
		"Юрий\0",
		"Антон\0",
		"Сергей\0",
		"Василий\0"
	};
	strcpy_s(SecondName, StringHelper::DefaultSize, LC_SecondName[rand() % 10]);

	const char* LC_LastName[10] = {
		"Иванович\0",
		"Петрович\0",
		"Сидорович\0",
		"Андреевич\0",
		"Потапович\0",
		"Константинович\0",
		"Юрьевич\0",
		"Антонович\0",
		"Сергеевич\0",
		"Васильевич\0"
	};
	strcpy_s(LastName, StringHelper::DefaultSize, LC_LastName[rand() % 10]);

	strcat_s(fio, StringHelper::DefaultSize, FirstName);
	strcat_s(fio, StringHelper::DefaultSize, " ");
	strcat_s(fio, StringHelper::DefaultSize, SecondName);
	strcat_s(fio, StringHelper::DefaultSize, " ");
	strcat_s(fio, StringHelper::DefaultSize, LastName);

	const char* LC_Operator[15] = {
		"039\0", "067\0", "068\0",
		"096\0", "097\0", "098\0",
		"050\0", "066\0", "095\0",
		"099\0", "063\0", "093\0",
		"091\0", "092\0", "094\0"
	};
	strcpy_s(Phone, StringHelper::DefaultSize, LC_Operator[rand() % 15]);

	int LPh = rand() % 1000;
	char* temp = StringHelper::New(StringHelper::DefaultSize);

	if (LPh < 100 && LPh >= 10) {
		strcat_s(Phone, StringHelper::DefaultSize, "0");
	} else if (LPh < 10) {
		strcat_s(Phone, StringHelper::DefaultSize, "00");
	};
	
	StringHelper::int_to_str(temp, LPh);
	strcat_s(Phone, StringHelper::DefaultSize, temp);

	for (int i = 0; i < 2; i++)	{
		LPh = rand() % 100;
		if (LPh < 10) {
			strcat_s(Phone, StringHelper::DefaultSize, "0");
		};
		StringHelper::Null(temp);
		StringHelper::int_to_str(temp, LPh);
		strcat_s(Phone, StringHelper::DefaultSize, temp);
	}

	House = rand() % 100;
	Apartment = rand() % 100;
	TarifPTR->id = rand() % 10;

	balance = ((rand() % 10000) - 1000);
	
	if (balance > 0) {
		state = 0;
	} else state = 1 + rand() % 5;



	free(FirstName);
	free(SecondName);
	free(LastName);
	free(temp);
}

void Abonent::Print() {
	printf(" %3d | %-40s | %10s | ул. %-15s %2d-%-2d | %-5d | %15s | %-5d | %-9d |",
		id, fio, Phone, StreetPTR->name, House, Apartment, TarifPTR->id, IP, balance, state);
	while (Console::X() < Console::DefaultScreenWidth - 1) {
		printf(" ");
	}
	printf(" ");
}

void Abonent::FastPrint(bool AbonShow, const Console::ConsoleColors Color, const Console::ConsoleColors BgColor) {
	short LWidth;
	if (AbonShow) LWidth = Console::Width() - StringHelper::AbonCardWidth;
	else LWidth = Console::Width();
	
	_CHAR_INFO* Buffer = (_CHAR_INFO*)calloc(LWidth, sizeof(CHAR_INFO));
	WORD CharAttrib = BgColor << 4 | Color;
	char* LineToPrint = StringHelper::New();
	
	BuildStr(this, LineToPrint);

	for (int i = 0; i < LWidth; i++) {
		if (LineToPrint[i] == '\0') {
			for (int j = i; j < LWidth; j++) {
				Buffer[j].Char.AsciiChar = ' ';
				Buffer[j].Attributes = CharAttrib;
			};
			break;
		} else Buffer[i].Char.AsciiChar = LineToPrint[i];
		Buffer[i].Attributes = CharAttrib;
	}
	COORD charPosition = { 0, 0 };

	SMALL_RECT writeArea = { 0, Console::Y(), LWidth, Console::Y() };

	COORD bufferSize = { LWidth, 1 };
	WriteConsoleOutputA(Console::GetHandle(), Buffer, bufferSize, charPosition, &writeArea);
	
	free(LineToPrint);
	free(Buffer);
}

void Abonent::BuildStr(Abonent* Abon, char* LineToBuild) {
	char* temp = StringHelper::New();
	char* int_to_str_temp = StringHelper::New();
	StringHelper::int_to_str(int_to_str_temp, Abon->id);
	strcpy_s(temp, StringHelper::DefaultSize, int_to_str_temp);

	StringHelper::StrToSize(temp, 4, 1);
	strcat_s(LineToBuild, StringHelper::DefaultSize, temp);
	strcat_s(LineToBuild, StringHelper::DefaultSize, " | ");

	strcpy_s(temp, StringHelper::DefaultSize, Abon->fio);
	StringHelper::StrToSize(temp, 40);
	strcat_s(LineToBuild, StringHelper::DefaultSize, temp);
	strcat_s(LineToBuild, StringHelper::DefaultSize, " | ");
	strcat_s(LineToBuild, StringHelper::DefaultSize, Abon->Phone);
	strcat_s(LineToBuild, StringHelper::DefaultSize, " | ул.");

	strcpy_s(temp, StringHelper::DefaultSize, Abon->StreetPTR->name);
	strcat_s(temp, StringHelper::DefaultSize, ", ");

	StringHelper::int_to_str(int_to_str_temp, Abon->House);
	strcat_s(temp, StringHelper::DefaultSize, int_to_str_temp);
	strcat_s(temp, StringHelper::DefaultSize, "-");
	StringHelper::int_to_str(int_to_str_temp, Abon->Apartment);
	strcat_s(temp, StringHelper::DefaultSize, int_to_str_temp);
	StringHelper::StrToSize(temp, 22);
	strcat_s(LineToBuild, StringHelper::DefaultSize, temp);
	strcat_s(LineToBuild, StringHelper::DefaultSize, " | ");

	StringHelper::int_to_str(int_to_str_temp, Abon->TarifPTR->id);
	strcpy_s(temp, StringHelper::DefaultSize, int_to_str_temp);
	StringHelper::StrToSize(temp, 5);
	strcat_s(LineToBuild, StringHelper::DefaultSize, temp);

	strcat_s(LineToBuild, StringHelper::DefaultSize, " | ");
	strcpy_s(temp, StringHelper::DefaultSize, Abon->IP);
	StringHelper::StrToSize(temp, 15);
	strcat_s(LineToBuild, StringHelper::DefaultSize, temp);

	strcat_s(LineToBuild, StringHelper::DefaultSize, " | ");
	
	char* bal = StringHelper::New();
	StringHelper::int_to_currency(bal, Abon->balance);
	if (bal[0] != '-') {
		strcpy_s(temp, StringHelper::DefaultSize, " ");
		strcat_s(temp, StringHelper::DefaultSize, bal);
	} else strcpy_s(temp, StringHelper::DefaultSize, bal);

	StringHelper::StrToSize(temp, 9);
	strcat_s(LineToBuild, StringHelper::DefaultSize, temp);

	strcat_s(LineToBuild, StringHelper::DefaultSize, " | ");

	free(bal);
	free(temp);
	free(int_to_str_temp);
}

Abonent* Abonent::ListFirst() {
	Abonent* LResult = this;
	while (LResult->ListPrev) {
		LResult = LResult->ListPrev;
	}
	return LResult;
}

Abonent* Abonent::ListLast() {
	Abonent* LResult = this;
	while (LResult->ListNext) {
		LResult = LResult->ListNext;
	}
	return LResult;
}

void Abonent::ListDraw(Abonent* TableFirst, short CursorPos) {
	Abonent* LItem = TableFirst;
	Console::GotoY(1);
	for (int i = 0; i < (Console::Height() - 2); i++) {
		if (Console::Y() % 2) {
			Console::SetColor(Console::clDarkGrey);
		}
		if (Console::Y() == CursorPos) {
			Console::SetColor(Console::clYellow, Console::clCyan);
		}
		LItem->Print();
		if (LItem->ListNext) {
			LItem = LItem->ListNext;
		}
		if (i < Console::Height() - 2) printf("\n");
		Console::SetColor(Console::clWhite, Console::clBlack);
	}
	Console::GotoY(CursorPos);
}

Abonent* Abonent::ListAdd(Abonent* ExistingItem) {
	Abonent* LResult = ExistingItem;
	LResult->ListPrev = this;
	LResult->ListNext = this->ListNext;
	if (this->ListNext) {
		this->ListNext->ListPrev = LResult;
	}
	this->ListNext = LResult;
	return LResult;
}

Abonent* Abonent::ListDel() {
	Abonent* AbonNext = this->ListNext;
	Abonent* AbonPrev = this->ListPrev;
	if (AbonPrev) {
		AbonPrev->ListNext = this->ListNext;
	} 
	if (AbonNext) {
		AbonNext->ListPrev = this->ListPrev;
	}
	delete this;
	if (AbonNext) {
		AbonNext->SaveToFile("bin\\abonents.db");
		return AbonNext;
	}
	else if (AbonPrev) {
		AbonPrev->SaveToFile("bin\\abonents.db");
		return AbonPrev;
	}
	else return NULL;
}

int Abonent::ListCount() {
	int LResult = 0;
	Abonent* LItem = ListFirst();
	if (LItem) LResult++;
	if (!LItem->ListNext && !LItem->ListNext) return LResult;
	while (LItem->ListNext) {
		LResult++;
		LItem = LItem->ListNext;
	}
	return LResult;
}

void Abonent::ListSwap(Abonent* Item_1, Abonent* Item_2) {
	if ((Item_1 != Item_2) && (Item_1 || Item_2)) {
		Abonent* L_Prev = NULL;
		Abonent* L_Next = NULL;
		if (Item_1->ListNext == Item_2 && Item_2->ListPrev == Item_1) {
			L_Prev = Item_1->ListPrev;
			L_Next = Item_2->ListNext;
			Item_1->ListNext = L_Next;
			Item_1->ListPrev = Item_2;
			Item_2->ListNext = Item_1;
			Item_2->ListPrev = L_Prev;
			if (L_Next) {
				L_Next->ListPrev = Item_1;
			}
			if (L_Prev) {
				L_Prev->ListNext = Item_2;
			}
		} else if (Item_2->ListNext == Item_1 && Item_1->ListPrev == Item_2) {
			L_Prev = Item_2->ListPrev;
			L_Next = Item_1->ListNext;
			Item_2->ListNext = L_Next;
			Item_2->ListPrev = Item_1;
			Item_1->ListNext = Item_2;
			Item_1->ListPrev = L_Prev;
			if (L_Next) {
				L_Next->ListPrev = Item_2;
			}
			if (L_Prev) {
				L_Prev->ListNext = Item_1;
			}
		} else {
			L_Prev = Item_1->ListPrev;
			L_Next = Item_1->ListNext;

			Item_1->ListPrev = Item_2->ListPrev;
			Item_2->ListPrev = L_Prev;
			Item_1->ListNext = Item_2->ListNext;
			Item_2->ListNext = L_Next;
			if (Item_1->ListNext) {
				Item_1->ListNext->ListPrev = Item_1;
			}
			if (Item_1->ListPrev) {
				Item_1->ListPrev->ListNext = Item_1;
			}
			if (Item_2->ListNext) {
				Item_2->ListNext->ListPrev = Item_2;
			}
			if (Item_2->ListPrev) {
				Item_2->ListPrev->ListNext = Item_2;
			}
		}
	}
}

Abonent* Abonent::ListSort(Abonent* Item) {
	if (Item->ListCount() > 1) {
		while (Item->ListPrev && strcmp(Item->fio, Item->ListPrev->fio) < 0) {
			ListSwap(Item, Item->ListPrev);
		}
		return ListFirst();
	} else return Item;
}

int Abonent::GetMaxID() {
	int max = 0;
	Abonent* Start = this->ListFirst();
	while (Start->ListNext) {
		if (Start->id > max) max = Start->id;
		if (Start->ListNext) Start = Start->ListNext;
	}
	return max;
}

void Abonent::SaveAbon(FILE* FileHandle, Abonent* Item) {
	fprintf(FileHandle, "%d|%s|%s|%s|%s|%d|%d|%d|%d|%s|%d|%d\n",
		Item->id, Item->fio, Item->login, Item->pass, Item->Phone, Item->StreetPTR->id, Item->House,
		Item->Apartment, Item->TarifPTR->id, Item->IP, Item->balance, Item->state);
}

void Abonent::SaveToFile(const char* FileName) {
	FILE* LFileHandle;
	int LFileOpenError = fopen_s(&LFileHandle, FileName, "w+");
	if (0 == LFileOpenError) {
		if (this) {
			Abonent* LItem = this->ListFirst();
			while (LItem) {
				SaveAbon(LFileHandle, LItem);
				LItem = LItem->ListNext;
			}
		} else fprintf(LFileHandle, "");
		fclose(LFileHandle);
	} else {
		printf("Не удалось сохранить данные в файл %s \n", FileName);
	}
}

Abonent* Abonent::LoadFromFile(const char* FileName, Street* StreetList, Tarif* TarifList) {
	Abonent* List = NULL;
	FILE* LFile;
	int LFileOpenError = fopen_s(&LFile, FileName, "r");
	if (!LFileOpenError) {
		char** context = (char**)calloc(StringHelper::DefaultSize, sizeof(char*));
		char* buffer = StringHelper::New(StringHelper::DefaultSize);

		while (!feof(LFile)) {
			fgets(buffer, StringHelper::DefaultSize * sizeof(char), LFile);
			if (feof(LFile)) break;
			if (List) {
				List = List->ListAdd(new Abonent);
			} else {
				List = new Abonent;
			}
			List->id = atoi(strtok_s(buffer, "|", context));
			strcpy_s(List->fio, StringHelper::DefaultSize, strtok_s(NULL, "|", context));
			strcpy_s(List->login, StringHelper::DefaultSize, strtok_s(NULL, "|", context));
			strcpy_s(List->pass, StringHelper::DefaultSize, strtok_s(NULL, "|", context));
			strcpy_s(List->Phone, StringHelper::DefaultSize, strtok_s(NULL, "|", context));

			List->StreetPTR = StreetList->Get_by_id(atoi(strtok_s(NULL, "|", context)));
			List->House = atoi(strtok_s(NULL, "|", context));
			List->Apartment = atoi(strtok_s(NULL, "|", context));
			List->TarifPTR = TarifList->Get_by_id(atoi(strtok_s(NULL, "|", context)));
			strcpy_s(List->IP, StringHelper::DefaultSize, strtok_s(NULL, "|", context));
			List->balance = atoi(strtok_s(NULL, "|", context));
			List->state = atoi(strtok_s(NULL, "\n", context));
			StringHelper::Null(buffer, StringHelper::DefaultSize);
			printf("");
		}

		free(context);
		free(buffer);
		fclose(LFile);
		if (List) return List->ListFirst();
	} else {
		printf("Ошибка открытия файла\n");
		return NULL;
	}
}
