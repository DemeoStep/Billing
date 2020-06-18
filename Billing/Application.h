#ifndef APPLICATION_H
#define APPLICATION_H
#include "Console.h"

class Application;
class Abonent;
class Street;
class Tarif;
class CellOper;
class FreeIP;
class Console;
class MySQL;

typedef void(*FuncOnKey)(Application *Sender);

class Application {
public:
	int PressedKey;
	bool Running;
	bool AbonShow;
	time_t now;
	tm ltm;

	Application();
	~Application();
	virtual void Run();
	virtual void DoProcessKey(const int AKeyPressed);
	virtual void DrawMenu(short aY, char* str);
	char* HelpString;
	char* TableString;
	char* Curr;
	char* today;
	short CursorPos;

	FuncOnKey OnEscape;
	FuncOnKey OnSpace;
	FuncOnKey OnBackspace;
	FuncOnKey OnTab;
	FuncOnKey OnEnter;
	FuncOnKey OnInsert;
	FuncOnKey OnDelete;
	FuncOnKey OnUp;
	FuncOnKey OnDown;
	FuncOnKey OnLeft;
	FuncOnKey OnRight;
	FuncOnKey OnPageUp;
	FuncOnKey OnPageDown;
	FuncOnKey OnHome;
	FuncOnKey OnEnd;
	FuncOnKey OnCenter;
	FuncOnKey OnF1;
	FuncOnKey OnF2;
	FuncOnKey OnF3;
	FuncOnKey OnF4;
	FuncOnKey OnF5;
	FuncOnKey OnF6;
	FuncOnKey OnF7;
	FuncOnKey OnF8;
	FuncOnKey OnF9;
	FuncOnKey OnF10;
	FuncOnKey OnF11;
	FuncOnKey OnF12;

	Abonent* Abonents;
	Street* Streets;
	Tarif* Tarifs;
	CellOper* CellCodes;
	MySQL* Connection;

	FreeIP* FreeGreyIPs;
	FreeIP* FreeRealIPs;

	Abonent* TableFirst;
	Abonent* TableLast;
	Abonent* CursorOn;

	void TableDrawOnKeyDown();
	void TableDrawOnKeyUp();
	void TableDrawOnPageDown();
	void TableDrawOnPageUp();
	void TableDraw();

	int JumpTo(Abonent* toItem, bool New);

	void Search();

	void Check_login(char* str, const int length);

	void ListsNULL();

	void ListsReLoad();

	void IPReLoad();

	void GetTime();

	void Today();

	void LoadConfig();

	void ShowAbonentCard(Abonent* Item, bool New, bool ShowButtons = true);
	Abonent* ShowEditCard(bool New);
	void AbonDel(Abonent* Item);
	void AbonAdd(Abonent* List);
	void AbonEdit();
	void Balance_change(Abonent* Item);
	void OnExit();
	void Init();

protected:
	virtual void DoOnKeyEscape();
	virtual void DoOnKeySpace();
	virtual void DoOnKeyBackspace();
	virtual void DoOnKeyTab();
	virtual void DoOnKeyEnter();
	virtual void DoOnKeyInsert();
	virtual void DoOnKeyDelete();
	virtual void DoOnKeyUp();
	virtual void DoOnKeyDown();
	virtual void DoOnKeyLeft();
	virtual void DoOnKeyRight();
	virtual void DoOnKeyPageUp();
	virtual void DoOnKeyPageDown();
	virtual void DoOnKeyHome();
	virtual void DoOnKeyEnd();
	virtual void DoOnKeyCenter();
	virtual void DoOnKeyF1();
	virtual void DoOnKeyF2();
	virtual void DoOnKeyF3();
	virtual void DoOnKeyF4();
	virtual void DoOnKeyF5();
	virtual void DoOnKeyF6();
	virtual void DoOnKeyF7();
	virtual void DoOnKeyF8();
	virtual void DoOnKeyF9();
	virtual void DoOnKeyF10();
	virtual void DoOnKeyF11();
	virtual void DoOnKeyF12();

};

#endif