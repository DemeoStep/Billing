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
	index = 0;
	fio = StringHelper::New();
	login = StringHelper::New();
	pass = StringHelper::New();
	Phone = StringHelper::New();
	StreetPTR = NULL;
	House = 0;
	Apartment = 0;
	TarifPTR = NULL;
	IP = StringHelper::New();
	balance = 0;
	state = 0;
	last_pay = StringHelper::New();

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
	StringHelper::int_to_str(int_to_str_temp, Abon->index);
	strcpy_s(temp, StringHelper::DefaultSize, int_to_str_temp);

	StringHelper::StrToSize(temp, 6, 1);
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
	StringHelper::StrToSize(temp, 25);
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
		return AbonNext;
	}
	else if (AbonPrev) {
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
	Item = Item->ListLast();
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

Abonent* Abonent::Get_by_id(int id) {
	Abonent* LResult = this->ListFirst();
	while (id != LResult->id) {
		LResult = LResult->ListNext;
	}

	return LResult;
};

void Abonent::IndexChange() {
	Abonent* List = this->ListFirst();
	int index = 0;
	while (List) {
		index++;
		List->index = index;
		List = List->ListNext;
	}
}
