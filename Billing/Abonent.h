#ifndef ABONENT_H
#define ABONENT_H
#include "Console.h"
#include "Application.h"

class Console;
class Application;

class Abonent {
public:
	Abonent();
	~Abonent();
	int id;
	int index;
	char* fio;
	char* login;
	char* pass;
	char* Phone;
	Street* StreetPTR;
	int House;
	int Apartment;
	Tarif* TarifPTR;
	char* IP;
	int balance; //в копейках
	int state;
	bool need_pay;

	void GenTest();
	void Print();
	void FastPrint(bool AbonShow, const Console::ConsoleColors Color = Console::DefaultColor, const Console::ConsoleColors BgColor = Console::DefaultBgColor);
	void BuildStr(Abonent* Abon, char* LineToBuild);
	Abonent* ListNext;
	Abonent* ListPrev;
	virtual Abonent* ListFirst();
	virtual Abonent* ListLast();
	static void ListDraw(Abonent* aTableFirst, short CursorPos);
	virtual Abonent* ListAdd(Abonent* ExistingItem);
	virtual Abonent* ListDel();
	static void ListSwap(Abonent* Item_1, Abonent* Item_2);
	virtual Abonent* ListSort(Abonent* Item);
	int GetMaxID();

	Abonent* Get_by_id(int id);

	void IndexChange();

	int ListCount();
};

#endif

