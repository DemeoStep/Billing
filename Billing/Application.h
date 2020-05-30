#ifndef APPLICATION_H
#define APPLICATION_H
#include "Console.h"

class Application;
class Abonent;
class Street;
class Tarif;
class CellOper;
class Free_grey_IP;
class Free_real_IP;
class Console;

typedef void(*FuncOnKey)(Application *Sender);

class Application {
public:
	int PressedKey;
	bool Running;
	bool AbonShow;

	Application();
	~Application();
	virtual void Run();
	virtual void DoProcessKey(const int AKeyPressed);
	virtual void DrawMenu(short aY, char* str);
	char* HelpString;
	char* TableString;
	char* Curr;

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

	Free_grey_IP* FreeGreyIPs;
	Free_real_IP* FreeRealIPs;

	Abonent* TableFirst;
	Abonent* TableLast;
	Abonent* CursorOn;

	void TableDrawOnKeyDown(short CursorPos);
	void TableDrawOnKeyUp(short CursorPos);
	void TableDrawOnPageDown(short CursorPos);
	void TableDrawOnPageUp(short CursorPos);
	void TableDraw(short CursorPos);
	void Print(char* str, const Console::ConsoleColors Color = Console::DefaultColor, const Console::ConsoleColors BgColor = Console::DefaultBgColor);

	int JumpTo(int CursorPos, Abonent* toItem);

	void ShowAbonentCard(Abonent* Item, bool New);
	Abonent* ShowAddCard(short CursorPos);
	void AbonDel(Abonent* Item, short CursorPos);
	int GetKey();
	void AbonAdd(Abonent* List, short CursorPos);
	void Balance_change(Abonent* Item, short CursorPos);
	bool ShowWarning(short CursorPos, char* warning);
	void ShowProgress(short CursorPos, char* mess, int step);
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