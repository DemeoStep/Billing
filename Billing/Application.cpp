#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include "Console.h"
#include "Application.h"
#include "StringHelper.h"
#include "Abonent.h"
#include "Street.h"
#include "CellOper.h"
#include "Tarif.h"
#include "Free_grey_IP.h"
#include "Free_real_IP.h"
#include "MySQL.h"

Application::Application() {
	// установка локализации по-умолчанию
	setlocale(LC_ALL, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	now = time(0);
	localtime_s(&ltm, &now);

	// установка параметров экрана по-умолчанию
	Console::SetScreen();
	// очистим экран
	Console::FillRect(0, 0, Console::Width(), Console::Height(), Console::clBlack);
	// 
	OnEscape	= NULL;
	OnSpace		= NULL;
	OnBackspace = NULL;
	OnTab		= NULL;
	OnEnter		= NULL;
	OnInsert	= NULL;
	OnDelete	= NULL;
	OnUp		= NULL;
	OnDown		= NULL;
	OnLeft		= NULL;
	OnRight		= NULL;
	OnPageUp	= NULL;
	OnPageDown	= NULL;
	OnHome		= NULL;
	OnEnd		= NULL;
	OnCenter	= NULL;
	OnF1		= NULL;
	OnF2		= NULL;
	OnF3		= NULL;
	OnF4		= NULL;
	OnF5		= NULL;
	OnF6		= NULL;
	OnF7		= NULL;
	OnF8		= NULL;
	OnF9		= NULL;
	OnF10		= NULL;
	OnF11		= NULL;
	OnF12		= NULL;
	Running		= false;
	PressedKey	= 0;

	HelpString = StringHelper::New();
	TableString = StringHelper::New();
	Curr = StringHelper::New();
	today = StringHelper::New();
	CursorPos = 0;

	strcpy_s(HelpString, StringHelper::DefaultSize,  " Esc - Выход | F5 - Новый | F6 - Пополнить счет | F7 - Поиск по адресу");
	strcpy_s(TableString, StringHelper::DefaultSize, "   №   |                    ФИО                   |    Телефон    |             Адрес            | Тариф |        IP       |   Баланс  | "); // Состояние | ");
	strcpy_s(Curr, StringHelper::DefaultSize, "грн.");

	Abonents = NULL;
	Streets = NULL;
	Tarifs = NULL;
	FreeGreyIPs = NULL;
	FreeRealIPs = NULL;
	CellCodes = NULL;
	Connection = new MySQL;

	CursorOn = NULL;
	TableFirst = NULL;
	TableLast = NULL;
	AbonShow = false;
	
}

Application::~Application() {
	free(HelpString);
	free(TableString);
	free(Curr);
	free(today);
	Console::FillRect(0, 0, Console::Width(), Console::Height(), Console::clBlack);
	Console::GotoXY(0, 0);

	delete Connection;
}

void Application::Run() {
	Running = true;
	Connection->InputLoginPass();
	DrawMenu(Console::Height() - 1, HelpString);
	DrawMenu(0, TableString);
	Console::ShowCursor(false);

	Console::GotoXY(0, 1);

	Init();

	if (Abonents) {
		TableFirst = Abonents;

		TableLast = TableFirst;
		int end = 0;

		if (Abonents->ListCount() < (Console::Height() - 2)) {
			end = Abonents->ListCount();
		} else {
			end = (Console::Height() - 2);
		}

		if (TableLast->ListNext) {
			for (int i = 1; i < end; i++) {
				TableLast = TableLast->ListNext;
			}
		}

		CursorOn = TableFirst;
		CursorPos = 1;
		TableDraw();

	}

	Connection->GetLastUpdatetime();

	while (Running) {
		Today();
		if (_kbhit()) {
			PressedKey = _getch();
			if ((0xE0 != PressedKey) && (0 != PressedKey)) {
				DoProcessKey(PressedKey);
			}
		} else if (ltm.tm_sec == 0){
			ListsReLoad();
		}
	}

	OnExit();
}

void Application::DoProcessKey(const int AKeyPressed) {
	switch (AKeyPressed) {
		case Console::keyEscape:	{ DoOnKeyEscape();		break; }
		case Console::keySpace:		{ DoOnKeySpace();		break; }
		case Console::keyBackspace: { DoOnKeyBackspace();	break; }
		case Console::keyTab:		{ DoOnKeyTab();			break; }
		case Console::keyEnter:		{ DoOnKeyEnter();		break; }
		case Console::keyInsert:	{ DoOnKeyInsert();		break; }
		case Console::keyDelete:	{ DoOnKeyDelete();		break; }
		case Console::keyUp:		{ DoOnKeyUp();			break; }
		case Console::keyDown:		{ DoOnKeyDown();		break; }
		case Console::keyLeft:		{ DoOnKeyLeft();		break; }
		case Console::keyRight:		{ DoOnKeyRight();		break; }
		case Console::keyPageUp:	{ DoOnKeyPageUp();		break; }
		case Console::keyPageDown:	{ DoOnKeyPageDown();	break; }
		case Console::keyHome:		{ DoOnKeyHome();		break; }
		case Console::keyEnd:		{ DoOnKeyEnd();			break; }
		case Console::keyCenter:	{ DoOnKeyCenter();		break; }
		case Console::keyF1:		{ DoOnKeyF1();			break; }
		case Console::keyF2:		{ DoOnKeyF2();			break; }
		case Console::keyF3:		{ DoOnKeyF3();			break; }
		case Console::keyF4:		{ DoOnKeyF4();			break; }
		case Console::keyF5:		{ DoOnKeyF5();			break; }
		case Console::keyF6:		{ DoOnKeyF6();			break; }
		case Console::keyF7:		{ DoOnKeyF7();			break; }
		case Console::keyF8:		{ DoOnKeyF8();			break; }
		case Console::keyF9:		{ DoOnKeyF9();			break; }
		case Console::keyF10:		{ DoOnKeyF10();			break; }
		case Console::keyF11:		{ DoOnKeyF11();			break; }
		case Console::keyF12:		{ DoOnKeyF12();			break; }
	}
}

void Application::DrawMenu(short aY, char* str) {
	Console::FillRect(0, aY, Console::Width(), aY, Console::clLightGrey);
	Console::GotoXY(0, aY);
	Console::Print(str, Console::clBlack, Console::clLightGrey);
}

void Application::DoOnKeyEscape()		{ if (NULL != OnEscape)		OnEscape(this); }
void Application::DoOnKeySpace()		{ if (NULL != OnSpace)		OnSpace(this); }
void Application::DoOnKeyBackspace()	{ if (NULL != OnBackspace)	OnBackspace(this); }
void Application::DoOnKeyTab()			{ if (NULL != OnTab)		OnTab(this); }
void Application::DoOnKeyEnter()		{ if (NULL != OnEnter)		OnEnter(this); }
void Application::DoOnKeyInsert()		{ if (NULL != OnInsert)		OnInsert(this); }
void Application::DoOnKeyDelete()		{ if (NULL != OnDelete)		OnDelete(this); }
void Application::DoOnKeyUp()			{ if (NULL != OnUp)			OnUp(this); }
void Application::DoOnKeyDown()			{ if (NULL != OnDown)		OnDown(this); }
void Application::DoOnKeyLeft()			{ if (NULL != OnLeft)		OnLeft(this); }
void Application::DoOnKeyRight()		{ if (NULL != OnRight)		OnRight(this); }
void Application::DoOnKeyPageUp()		{ if (NULL != OnPageUp)		OnPageUp(this); }
void Application::DoOnKeyPageDown()		{ if (NULL != OnPageDown)	OnPageDown(this); }
void Application::DoOnKeyHome()			{ if (NULL != OnHome)		OnHome(this); }
void Application::DoOnKeyEnd()			{ if (NULL != OnEnd)		OnEnd(this); }
void Application::DoOnKeyCenter()		{ if (NULL != OnCenter)		OnCenter(this); }
void Application::DoOnKeyF1()			{ if (NULL != OnF1)			OnF1(this); }
void Application::DoOnKeyF2()			{ if (NULL != OnF2)			OnF2(this); }
void Application::DoOnKeyF3()			{ if (NULL != OnF3)			OnF3(this); }
void Application::DoOnKeyF4()			{ if (NULL != OnF4)			OnF4(this); }
void Application::DoOnKeyF5()			{ if (NULL != OnF5)			OnF5(this); }
void Application::DoOnKeyF6()			{ if (NULL != OnF6)			OnF6(this); }
void Application::DoOnKeyF7()			{ if (NULL != OnF7)			OnF7(this); }
void Application::DoOnKeyF8()			{ if (NULL != OnF8)			OnF8(this); }
void Application::DoOnKeyF9()			{ if (NULL != OnF9)			OnF9(this); }
void Application::DoOnKeyF10()			{ if (NULL != OnF10)		OnF10(this); }
void Application::DoOnKeyF11()			{ if (NULL != OnF11)		OnF11(this); }
void Application::DoOnKeyF12()			{ if (NULL != OnF12)		OnF12(this); }

void Application::TableDrawOnKeyDown() {
	if (CursorOn) {
		Console::ConsoleColors TextColor = Console::clWhite;
		Console::ConsoleColors BgColor = Console::clBlack;
		if (CursorPos < Console::Height() - 2 && CursorOn != TableLast) {
			if (CursorOn->state) {
				TextColor = Console::clRed;
			}
			if (CursorPos % 2) {
				if (CursorOn->state) {
					TextColor = Console::clRed;
				} else {
					TextColor = Console::clDarkGrey;
				}
				BgColor = Console::clBlack;
			}
			CursorOn->FastPrint(AbonShow, TextColor, BgColor);
			CursorOn = CursorOn->ListNext;
			CursorPos++;
			Console::GotoXY(0, CursorPos);
			if (CursorOn->state) {
				CursorOn->FastPrint(AbonShow, Console::clYellow, Console::clRed);
			} else {
				CursorOn->FastPrint(AbonShow, Console::clYellow, Console::clCyan);
			}
			if (AbonShow) {
				ShowAbonentCard(CursorOn, false);
			}
		} else if (CursorOn->ListNext) {
			TableFirst = TableFirst->ListNext;
			TableLast = TableLast->ListNext;
			CursorOn = CursorOn->ListNext;
			Console::GotoXY(0, 1);
			TableDraw();
			if (AbonShow) {
				ShowAbonentCard(CursorOn, false);
			}
			Console::GotoXY(0, Console::Height() - 2);
		}
		Console::SetColor(Console::clWhite, Console::clBlack);
	}
}

void Application::TableDrawOnKeyUp() {
	if (CursorOn) {
		Console::ConsoleColors TextColor = Console::clWhite;
		Console::ConsoleColors BgColor = Console::clBlack;
		if (Console::Y() > 1 && CursorOn != TableFirst) {
			if (CursorOn->state) {
				TextColor = Console::clRed;
			}
			if (CursorPos % 2) {
				if (CursorOn->state) {
					TextColor = Console::clRed;
				} else {
					TextColor = Console::clDarkGrey;
				}
				BgColor = Console::clBlack;
			}
			CursorOn->FastPrint(AbonShow, TextColor, BgColor);
			CursorOn = CursorOn->ListPrev;
			CursorPos--;
			Console::GotoXY(0, CursorPos);
			if (CursorOn->state) {
				CursorOn->FastPrint(AbonShow, Console::clYellow, Console::clRed);
			} else {
				CursorOn->FastPrint(AbonShow, Console::clYellow, Console::clCyan);
			}
			if (AbonShow) {
				ShowAbonentCard(CursorOn, false);
			}
		} else if (CursorOn->ListPrev) {
			TableFirst = TableFirst->ListPrev;
			TableLast = TableLast->ListPrev;
			CursorOn = CursorOn->ListPrev;
			Console::GotoXY(0, 1);
			TableDraw();
			if (AbonShow) {
				ShowAbonentCard(CursorOn, false);
			}
			Console::GotoXY(0, 1);
		}
		Console::SetColor(Console::clWhite, Console::clBlack);
	}
}

void Application::TableDrawOnPageDown() {
	if (CursorOn) {
		Console::ConsoleColors TextColor = Console::clWhite;
		Console::ConsoleColors BgColor = Console::clBlack;
		if (CursorPos < (Console::Height() - 2) && CursorOn != TableLast) {
			if (CursorOn->state) {
				TextColor = Console::clRed;
			}
			if (CursorPos % 2) {
				if (CursorOn->state) {
					TextColor = Console::clRed;
				} else {
					TextColor = Console::clDarkGrey;
				}
				BgColor = Console::clBlack;
			}
			CursorOn->FastPrint(AbonShow, TextColor, BgColor);
			for (int i = 0; i < (Console::Height() - 2) / 2; i++) {
				if (CursorOn != TableLast) {
					CursorOn = CursorOn->ListNext;
					CursorPos++;
				}
			}
			Console::GotoXY(0, CursorPos);
			if (CursorOn->state) {
				CursorOn->FastPrint(AbonShow, Console::clYellow, Console::clRed);
			} else {
				CursorOn->FastPrint(AbonShow, Console::clYellow, Console::clCyan);
			}
			if (AbonShow) {
				ShowAbonentCard(CursorOn, false);
			}
			Console::GotoXY(0, CursorPos);
		} else if (TableLast->ListNext) {
			if (CursorPos % 2) {
				TextColor = Console::clDarkGrey;
				BgColor = Console::clBlack;
			}
			CursorOn->FastPrint(AbonShow, TextColor, BgColor);
			for (int i = 0; i < (Console::Height() - 2) / 2; i++) {
				if (TableLast->ListNext) {
					TableFirst = TableFirst->ListNext;
					TableLast = TableLast->ListNext;
				}
			}
			CursorOn = TableLast;
			CursorPos = Console::Height() - 2;
			Console::GotoXY(0, CursorPos);
			TableDraw();
			if (AbonShow) {
				ShowAbonentCard(CursorOn, false);
			}
			Console::GotoXY(0, CursorPos);
		}
		Console::SetColor(Console::clWhite, Console::clBlack);
	}
}

void Application::TableDrawOnPageUp() {
	if (CursorOn) {
		Console::ConsoleColors TextColor = Console::clWhite;
		Console::ConsoleColors BgColor = Console::clBlack;
		if (CursorPos > 1 && CursorOn != TableFirst) {
			if (CursorOn->state) {
				TextColor = Console::clRed;
			}
			if (CursorPos % 2) {
				if (CursorOn->state) {
					TextColor = Console::clRed;
				} else {
					TextColor = Console::clDarkGrey;
				}
				BgColor = Console::clBlack;
			}
			CursorOn->FastPrint(AbonShow, TextColor, BgColor);
			for (int i = 0; i < (Console::Height() - 2) / 2; i++) {
				if (CursorOn != TableFirst) {
					CursorOn = CursorOn->ListPrev;
					CursorPos--;
				}
			}
			Console::GotoXY(0, CursorPos);
			if (CursorOn->state) {
				CursorOn->FastPrint(AbonShow, Console::clYellow, Console::clRed);
			} else {
				CursorOn->FastPrint(AbonShow, Console::clYellow, Console::clCyan);
			}
			if (AbonShow) {
				ShowAbonentCard(CursorOn, false);
			}
			Console::GotoXY(0, CursorPos);
		} else if (TableFirst->ListPrev) {
			if (Console::Y() % 2) {
				Console::SetColor(Console::clDarkGrey);
			}
			CursorOn->FastPrint(AbonShow, TextColor, BgColor);
			for (int i = 0; i < (Console::Height() - 2) / 2; i++) {
				if (TableFirst->ListPrev) {
					TableFirst = TableFirst->ListPrev;
					TableLast = TableLast->ListPrev;
				}
			}
			CursorOn = TableFirst;
			CursorPos = 1;
			Console::GotoXY(0, CursorPos);
			if (AbonShow) {
				ShowAbonentCard(CursorOn, false);
			}
			TableDraw();
			Console::GotoXY(0, CursorPos);
		}
		Console::SetColor(Console::clWhite, Console::clBlack);
	}
}

void Application::TableDraw() {
	Console::GotoXY(0, 1);
	if (CursorOn) {
		if (CursorOn->ListNext || CursorOn->ListPrev) {
			for (Abonent* i = TableFirst; i != TableLast->ListNext; i = i->ListNext) {
				Console::ConsoleColors TextColor = Console::clWhite;
				Console::ConsoleColors BgColor = Console::clBlack;
				if (i->state) {
					TextColor = Console::clRed;
				}
				if (Console::Y() % 2) {
					if (i->state) {
						TextColor = Console::clRed;
					} else {
						TextColor = Console::clDarkGrey;
					}
					BgColor = Console::clBlack;
				}
				if (Console::Y() == CursorPos) {
					if (i->state) {
						TextColor = Console::clYellow;
						BgColor = Console::clRed;
					} else {
						TextColor = Console::clYellow;
						BgColor = Console::clCyan;
					}
				}
				i->FastPrint(AbonShow, TextColor, BgColor);
				if (CursorOn->ListCount() < Console::Height() - 2) {
					if (!AbonShow)Console::FillRect(0, CursorOn->ListCount() + 1, Console::Width(), Console::Height() - 2, Console::clBlack);
					else Console::FillRect(0, CursorOn->ListCount() + 1, Console::Width() - StringHelper::AbonCardWidth - 1, Console::Height() - 2, Console::clBlack);
				}
				Console::SetColor(Console::clWhite, Console::clBlack);
				Console::GotoXY(0, Console::Y() + 1);
			}

		} else {
			CursorOn->FastPrint(AbonShow, Console::clYellow, Console::clCyan);
			if (!AbonShow) Console::FillRect(0, 2, Console::Width(), Console::Height() - 2, Console::clBlack);
			else Console::FillRect(0, 2, Console::Width() - StringHelper::AbonCardWidth - 1, Console::Height() - 2, Console::clBlack);
		}
		Console::GotoXY(0, CursorPos);
	} else {
		Console::FillRect(0, 1, Console::Width(), Console::Height() - 2, Console::clBlack);
		Console::GotoXY(0, 1);
	}
	
}

void Application::ShowAbonentCard(Abonent* Item, bool New, bool ShowButtons) {
	if (Item) {
		short LeftX = Console::Width() - StringHelper::AbonCardWidth;
		short LeftY = 0;

		char* temp = StringHelper::New();
		char* int_to_str_temp = StringHelper::New();
		char* bal = StringHelper::New();
		Console::ConsoleColors BgColor = Console::clCyan;
		Console::ConsoleColors Color = Console::clYellow;

		if (!AbonShow) {
			Console::FillRect(LeftX + 2, LeftY + 1, Console::Width() - 3, Console::Height() - 2, BgColor);
		}

		if (Item->state) {
			BgColor = Console::clRed;
		} else {
			BgColor = Console::clCyan;
		}

		Console::FillRect(LeftX, LeftY, Console::Width(), LeftY, BgColor);
		Console::FillRect(LeftX, LeftY + 1, LeftX + 1, Console::Height() - 1, BgColor);
		Console::FillRect(Console::Width() - 2, LeftY + 1, Console::Width(), Console::Height() - 1, BgColor);
		Console::FillRect(LeftX, Console::Height() - 1, Console::Width(), Console::Height() - 1, BgColor);

		BgColor = Console::clCyan;

		LeftX += 4;
		LeftY += 2;

		Console::GotoXY(LeftX, LeftY);
		if (!AbonShow) Console::Print((char*)"Ф.И.О: ", Color, BgColor);
		Console::GotoX(LeftX + 7);
		strcpy_s(temp, StringHelper::DefaultSize, " ");
		if (!New) strcat_s(temp, StringHelper::DefaultSize, Item->fio);
		StringHelper::StrToSize(temp, 42);
		Console::Print(temp, Console::clBlack, Console::clLightGrey);

		LeftY += 2;
		Console::GotoXY(LeftX, LeftY);
		if (!AbonShow) Console::Print((char*)"Адрес: ул. ", Color, BgColor);

		Console::GotoX(LeftX + 7);
		strcpy_s(temp, StringHelper::DefaultSize, " ");
		if (!New) strcat_s(temp, StringHelper::DefaultSize, Item->StreetPTR->name);
		StringHelper::StrToSize(temp, 23);
		Console::Print(temp, Console::clBlack, Console::clLightGrey);

		Console::GotoX(LeftX + 31);
		if (!AbonShow) Console::Print((char*)"д: ", Color, BgColor);
		Console::GotoX(LeftX + 34);
		strcpy_s(temp, StringHelper::DefaultSize, " ");
		if (!New) StringHelper::int_to_str(int_to_str_temp, Item->House);
		strcat_s(temp, StringHelper::DefaultSize, int_to_str_temp);
		StringHelper::StrToSize(temp, 5);
		Console::Print(temp, Console::clBlack, Console::clLightGrey);
		Console::GotoX(LeftX + 40);
		if (!AbonShow) Console::Print((char*)"кв: ", Color, BgColor);
		Console::GotoX(LeftX + 44);
		strcpy_s(temp, StringHelper::DefaultSize, " ");
		if (!New) StringHelper::int_to_str(int_to_str_temp, Item->Apartment);
		strcat_s(temp, StringHelper::DefaultSize, int_to_str_temp);
		StringHelper::StrToSize(temp, 5);
		Console::Print(temp, Console::clBlack, Console::clLightGrey);

		LeftY += 2;
		Console::GotoXY(LeftX, LeftY);
		if (!AbonShow) Console::Print((char*)"Тел: ", Color, BgColor);
		Console::GotoX(LeftX + 7);
		strcpy_s(temp, StringHelper::DefaultSize, " ");
		strcat_s(temp, StringHelper::DefaultSize, Item->Phone);
		StringHelper::StrToSize(temp, 17);
		Console::Print(temp, Console::clBlack, Console::clLightGrey);

		Console::GotoX(LeftX + 26);
		if (!AbonShow) Console::Print((char*)"Id: ", Color, BgColor);
		Console::GotoX(LeftX + 30);
		StringHelper::int_to_str(int_to_str_temp, Item->id);
		StringHelper::StrToSize(int_to_str_temp, 5);
		Console::Print(int_to_str_temp, Color, BgColor);

		LeftY += 2;
		Console::GotoXY(LeftX, LeftY);
		if (!AbonShow) Console::Print((char*)"Логин: ", Color, BgColor);
		Console::GotoX(LeftX + 7);
		strcpy_s(temp, StringHelper::DefaultSize, " ");
		strcat_s(temp, StringHelper::DefaultSize, Item->login);
		StringHelper::StrToSize(temp, 17);
		Console::Print(temp, Console::clBlack, Console::clLightGrey);

		Console::GotoX(LeftX + 26);
		if (!AbonShow) Console::Print((char*)"Пароль: ", Color, BgColor);
		Console::GotoX(LeftX + 34);
		strcpy_s(temp, StringHelper::DefaultSize, " ");
		strcat_s(temp, StringHelper::DefaultSize, Item->pass);
		StringHelper::StrToSize(temp, 15);
		Console::Print(temp, Console::clBlack, Console::clLightGrey);

		LeftY += 2;
		Console::GotoXY(LeftX, LeftY);
		if (!AbonShow) Console::Print((char*)"Тариф: ", Color, BgColor);
		Console::GotoX(LeftX + 7);
		strcpy_s(temp, StringHelper::DefaultSize, " ");
		Tarifs = Tarifs->ListFirst();
		while (Tarifs->id != Item->TarifPTR->id) {
			Tarifs = Tarifs->ListNext;
		}
		if (!New) strcat_s(temp, StringHelper::DefaultSize, Tarifs->name);
		StringHelper::StrToSize(temp, 42);
		Console::Print(temp, Console::clBlack, Console::clLightGrey);

		LeftY += 2;
		Console::GotoXY(LeftX, LeftY);
		if (!AbonShow) Console::Print((char*)"IP: ", Color, BgColor);
		Console::GotoX(LeftX + 7);
		strcpy_s(temp, StringHelper::DefaultSize, " ");
		strcat_s(temp, StringHelper::DefaultSize, Item->IP);

		StringHelper::StrToSize(temp, 17);
		Console::Print(temp, Console::clBlack, Console::clLightGrey);
		if (!New) {
			LeftY += 2;
			Console::GotoXY(LeftX, LeftY);
			if (!AbonShow) Console::Print((char*)"Баланс: ", Color, BgColor);
			Console::GotoX(LeftX + 8);

			StringHelper::int_to_currency(bal, Item->balance);
			if (bal[0] != '-') {
				strcpy_s(temp, StringHelper::DefaultSize, " ");
				strcat_s(temp, StringHelper::DefaultSize, bal);
			} else strcpy_s(temp, StringHelper::DefaultSize, bal);
			strcat_s(temp, StringHelper::DefaultSize, " ");
			strcat_s(temp, StringHelper::DefaultSize, Curr);
			strcat_s(temp, StringHelper::DefaultSize, "         ");
			Console::Print(temp, Color, BgColor);

			LeftY += 2;
			Console::GotoXY(LeftX, LeftY);
			if (!AbonShow) Console::Print((char*)"Статус: ", Color, BgColor);
			Console::GotoX(LeftX + 8);
			if (!this->CursorOn->state) {
				Console::Print((char*)"Доступ разрешен", Color, BgColor);
			} else Console::Print((char*)"Заблокирован   ", Console::clRed, BgColor);

			if (ShowButtons) {

				Color = Console::clCyan;
				BgColor = Console::clYellow;

				Console::GotoXY(LeftX, LeftY);
				if (Console::Height() >= 33) {
					LeftX += 14;
					LeftY = Console::Height() - 14;
					Console::FillRect(LeftX - 2, LeftY - 1, LeftX + 21, LeftY + 1, BgColor);
				} else {
					LeftY = Console::Height() - 5;
					LeftX++;
				}
				Console::GotoXY(LeftX, LeftY);
				Console::Print((char*)" F5 - Редактировать  ", Color, BgColor);

				if (Console::Height() >= 33) {
					LeftY = Console::Height() - 9;
					Console::FillRect(LeftX - 2, LeftY - 1, LeftX + 21, LeftY + 1, BgColor);
				} else LeftX += 26;
				Console::GotoXY(LeftX, LeftY);
				Console::Print((char*)" F6 - Пополнить счет ", Color, BgColor);

				if (CursorOn->balance <= 0) {
					Color = Console::clYellow;
					BgColor = Console::clRed;
				} else {
					Color = Console::clWhite;
					BgColor = Console::clLightGrey;
				}

				if (Console::Height() >= 33) {
					LeftY = Console::Height() - 4;
					LeftX = Console::Width() - 39;
					Console::FillRect(LeftX - 2, LeftY - 1, LeftX + 21, LeftY + 1, BgColor);
				} else {
					LeftY = Console::Height() - 3;
					LeftX = Console::Width() - 39;
				}
				Console::GotoXY(LeftX, LeftY);
				Console::Print((char*)"   F8 - УДАЛИТЬ!!!   ", Color, BgColor);
			} else {
				Color = Console::clCyan;
				BgColor = Console::clCyan;

				if (Console::Height() >= 33) {
					LeftX += 14;
					LeftY = Console::Height() - 14;
					Console::FillRect(LeftX - 2, LeftY - 1, LeftX + 21, LeftY + 1, BgColor);
				} else {
					LeftY = Console::Height() - 5;
					LeftX++;
				}

				if (Console::Height() >= 33) {
					LeftY = Console::Height() - 9;
					Console::FillRect(LeftX - 2, LeftY - 1, LeftX + 21, LeftY + 1, BgColor);
				} else LeftX += 26;

				if (Console::Height() >= 33) {
					LeftY = Console::Height() - 4;
					LeftX = Console::Width() - 39;
					Console::FillRect(LeftX - 2, LeftY - 1, LeftX + 21, LeftY + 1, BgColor);
				} else {
					LeftY = Console::Height() - 3;
					LeftX = Console::Width() - 39;
				}
			}
		}
		Console::GotoXY(0, CursorPos);
		AbonShow = true;

		free(bal);
		free(int_to_str_temp);
		free(temp);
	}
}

Abonent* Application::ShowEditCard(bool New) {
	if (!New) {
		Abonents = CursorOn;
	}
	Abonent* LResult = Abonents;
	int keyPressed = 0;
	char* temp = StringHelper::New();
	char* str = StringHelper::New();

	if (Abonents->ListCount() > 0) TableDraw();
	else {
		int X = Console::Width() / 2;
		int Y = Console::Height() / 2;
		Console::FillRect(X - 20, Y - 2, X + 20, Y + 2, Console::clBlack);
	};

	ShowAbonentCard(Abonents, New, false);

	Console::ShowCursor(false);

	short X = Console::Width() - StringHelper::AbonCardWidth + 12;
	short Y = 2;

	Console::GotoXY(X, Y);

	StringHelper::InputRus(Abonents->fio, 40);

	Console::ShowCursor(false);
	Y += 2;

	Console::GotoXY(X, Y);

	Abonents->StreetPTR = Abonents->StreetPTR->DrawChoiceList();

	X += 27;
	Console::GotoXY(X, Y);

	if (!New) StringHelper::int_to_str(temp, Abonents->House);
	StringHelper::InputDigit(temp, 3, false);
	Abonents->House = atoi(temp);
	StringHelper::Null(temp);

	Console::GotoXY(X + 10, Y);
	if (!New) StringHelper::int_to_str(temp, Abonents->Apartment);
	StringHelper::InputDigit(temp, 3, true);
	Abonents->Apartment = atoi(temp);
	Console::Print(temp, Console::clBlack, Console::clLightGrey);
	StringHelper::Null(temp);

	Y += 2;
	X -= 27;
	Console::GotoXY(X, Y);
	Console::FillRect(X - 1, Y, X + 15, Y, Console::clYellow);
	if (!New) {
		Console::Print(Abonents->Phone, Console::clBlack, Console::clYellow);
		keyPressed = Console::GetKey();
	} else keyPressed = Console::keyBackspace;

	if (keyPressed == Console::keyBackspace) {
		
		Console::FillRect(X - 1, Y, X + 15, Y, Console::clLightGrey);
		StringHelper::InputCellPhone(Abonents->Phone, CellCodes);
	}

	if (!New) {
		Console::FillRect(X - 1, Y, X + 15, Y, Console::clLightGrey);
		Console::Print(Abonents->Phone, Console::clBlack, Console::clLightGrey);
	}

	Y += 2;
	Console::GotoXY(X, Y);
	Check_login(Abonents->login, 15);

	Console::GotoXY(X + 27, Y);
	StringHelper::InputEng(Abonents->pass, 13);

	Y += 2;
	Console::GotoXY(X, Y);
	Console::FillRect(X - 1, Y, X + 40, Y, Console::clYellow);
	
	bool tarif_is_real = Abonents->TarifPTR->isReal;
	Abonents->TarifPTR = Abonents->TarifPTR->DrawChoiceList();

	Y += 2;
	Console::GotoXY(X, Y);
	Console::FillRect(X - 1, Y, X + 15, Y, Console::clYellow);

	if (tarif_is_real != Abonents->TarifPTR->isReal) {
		if (!New && tarif_is_real) {
			Connection->RestoreRealIP(Abonents->IP);
		} else if (!New) {
			Connection->RestoreGreyIP(Abonents->IP);
		}
		New = true;
	} else Console::Print(Abonents->IP, Console::clBlack, Console::clYellow);

	IPReLoad();

	if (!New) {
		keyPressed = Console::GetKey();
		if (keyPressed == Console::keyBackspace) {
			if (Abonents->TarifPTR->isReal) Connection->RestoreRealIP(Abonents->IP);
			else Connection->RestoreGreyIP(Abonents->IP);
		}
	} else keyPressed = Console::keyBackspace;

	if (keyPressed == Console::keyBackspace) {
		FreeGreyIPs = FreeGreyIPs->ListFirst();
		FreeRealIPs = FreeRealIPs->ListFirst();

		if (!Abonents->TarifPTR->isReal) {
			strcpy_s(temp, StringHelper::DefaultSize, FreeGreyIPs->ip);
		} else {
			strcpy_s(temp, StringHelper::DefaultSize, FreeRealIPs->ip);
		}
		StringHelper::StrToSize(temp, 15);
		Console::Print(temp, Console::clBlack, Console::clYellow);
		keyPressed = 0;

		while (keyPressed != Console::keyEnter) {
			if (keyPressed == Console::keyDown) {
				if (!Abonents->TarifPTR->isReal) {
					if (FreeGreyIPs->ListNext) FreeGreyIPs = FreeGreyIPs->ListNext;
				} else {
					if (FreeRealIPs->ListNext) FreeRealIPs = FreeRealIPs->ListNext;
				}
			} else if (keyPressed == Console::keyUp) {
				if (!Abonents->TarifPTR->isReal) {
					if (FreeGreyIPs->ListPrev) FreeGreyIPs = FreeGreyIPs->ListPrev;
				} else {
					if (FreeRealIPs->ListPrev) FreeRealIPs = FreeRealIPs->ListPrev;
				}
			}

			if (!Abonents->TarifPTR->isReal) {
				strcpy_s(temp, StringHelper::DefaultSize, FreeGreyIPs->ip);
			} else {
				strcpy_s(temp, StringHelper::DefaultSize, FreeRealIPs->ip);
			}
			StringHelper::StrToSize(temp, 15);
			Console::Print(temp, Console::clBlack, Console::clYellow);

			keyPressed = Console::GetKey();
			*str = keyPressed;
			*(str + 1) = '\0';
		}
		if (!Abonents->TarifPTR->isReal) {
			strcpy_s(Abonents->IP, StringHelper::DefaultSize, FreeGreyIPs->ip);
			Connection->DelGreyIP(FreeGreyIPs);
		} else {
			strcpy_s(Abonents->IP, StringHelper::DefaultSize, FreeRealIPs->ip);
			Connection->DelRealIP(FreeRealIPs);
		}
		Console::GotoXY(X, Y);
		Console::FillRect(X - 1, Y, X + 15, Y, Console::clLightGrey);
		Console::Print(Abonents->IP, Console::clBlack, Console::clLightGrey);
	}

	free(temp);
	free(str);
		
	return LResult;
}

void Application::AbonDel(Abonent* Item) {
	ListsReLoad();
	if (CursorOn) { // Список НЕнулевой
		bool Desigion = Console::ShowWarning((char*)"Удалить? (Y/n)", CursorPos);
		if (Desigion) {
			FreeGreyIPs = FreeGreyIPs->ListLast();
			if (CursorOn == TableFirst && CursorOn != TableLast) { // Курсор на первой позиции таблицы
				if (CursorOn->TarifPTR->isReal) {
					Connection->RestoreRealIP(CursorOn->IP);
				} else {
					Connection->RestoreGreyIP(CursorOn->IP);
				}
				Connection->DelAbon(CursorOn);
				CursorOn = CursorOn->ListDel();
				TableFirst = CursorOn;
				if (TableLast->ListNext) {
					TableLast = TableLast->ListNext;
				}
			} else if (CursorOn == TableLast && CursorOn != TableFirst) { // Курсор на последней позиции таблицы
				if (CursorOn->ListNext) { // Курсор НЕ на последнем элементе списка
					if (CursorOn->TarifPTR->isReal) {
						Connection->RestoreRealIP(CursorOn->IP);
					} else Connection->RestoreGreyIP(CursorOn->IP);
					Connection->DelAbon(CursorOn);
					CursorOn = CursorOn->ListDel();
					TableLast = CursorOn;
				} else { // Курсор на последнем элементе списка
					if (CursorOn->TarifPTR->isReal) {
						Connection->RestoreRealIP(CursorOn->IP);
					} else Connection->RestoreGreyIP(CursorOn->IP);
					Connection->DelAbon(CursorOn);
					CursorOn = CursorOn->ListDel();
					TableLast = CursorOn;
					if (TableFirst->ListPrev) {
						TableFirst = TableFirst->ListPrev;
					} else {
						CursorPos--;
						Console::GotoXY(0, CursorPos);
					}
				}
			} else if (CursorOn == TableFirst && CursorOn == TableLast) { // Единственный элемент списка
				if (CursorOn->TarifPTR->isReal) {
					Connection->RestoreRealIP(CursorOn->IP);
				} else {
					Connection->RestoreGreyIP(CursorOn->IP);
				}
				Connection->DelAbon(CursorOn);
				CursorOn = CursorOn->ListDel();
				CursorOn = NULL;
				AbonShow = false;
			} else { // Не последний и не первый элемент таблицы и не единственный элемент списка
				if (CursorOn->TarifPTR->isReal) {
					Connection->RestoreRealIP(CursorOn->IP);
				} else Connection->RestoreGreyIP(CursorOn->IP);
				Connection->DelAbon(CursorOn);
				CursorOn = CursorOn->ListDel();
				if (!TableLast->ListNext && TableFirst->ListPrev) {
					TableFirst = TableFirst->ListPrev;
					CursorPos++;
				} else if (TableLast->ListNext && !TableFirst->ListPrev) {
					TableLast = TableLast->ListNext;
				} else if (TableLast->ListNext && TableFirst->ListPrev) {
					TableFirst = TableFirst->ListPrev;
					CursorOn = CursorOn->ListPrev;
				}
			}
		}
		CursorOn->IndexChange();
		TableDraw();
		AbonShow = false;
		ShowAbonentCard(CursorOn, false);
		Console::GotoXY(0, CursorPos);
		Abonents = CursorOn;

	} else { // Список пустой
		Console::FillRect(0, 0, Console::Width(), Console::Height() - 2, Console::clBlack);
		AbonShow = false;
	}
	ListsReLoad();
}

void Application::AbonAdd(Abonent* LAdded) {

	if (Console::ShowWarning((char*)"Создать абонента? (Y/n)", CursorPos)) {

		ListsReLoad();

		if (Abonents) {
			Abonents = Abonents->ListLast();
			Abonents = Abonents->ListAdd(new Abonent);
		} else {
			Abonents = new Abonent;
		}

		Abonents->id = 0;
		Abonents->state = 1;
		Abonents->TarifPTR = Tarifs->ListFirst();
		Abonents->StreetPTR = Streets->ListFirst();
		strcpy_s(Abonents->last_pay, StringHelper::DefaultSize, today);

		LAdded = ShowEditCard(true);

		if (Console::ShowWarning((char*)"Сохранить? (Y/n)", CursorPos)) {

			if (Abonents->ListCount() > 1) {
				TableFirst = Abonents->ListFirst();
			} else TableFirst = Abonents;

			TableLast = TableFirst;
			int end = 0;

			if (Abonents->ListCount() < (Console::Height() - 2)) {
				end = Abonents->ListCount();
			} else {
				end = (Console::Height() - 2);
			}

			if (TableLast->ListNext) {
				for (int i = 1; i < end; i++) {
					TableLast = TableLast->ListNext;
				}
			}

		} else {
			if (!Abonents->TarifPTR->isReal) Connection->RestoreRealIP(Abonents->IP);
			else Connection->RestoreGreyIP(Abonents->IP);
			Abonents = Abonents->ListDel();
			LAdded = NULL;
		}

		if (LAdded) {
			if (Abonents->ListCount() > 1) {
				Abonents = Abonents->ListSort(Abonents);
			} else {
				CursorOn = Abonents;
				CursorPos = 1;

			}

			CursorPos = JumpTo(LAdded, true);
			Connection->SaveAbon(LAdded, true, Streets, Tarifs);

		}
	}
	ListsReLoad();
	AbonShow = false;
	ShowAbonentCard(CursorOn, false);
	Console::GotoXY(0, CursorPos);
}

void Application::AbonEdit() {
	
	if (Console::ShowWarning((char*)"Редактировать? (Y/n)", CursorPos)) {
		ListsReLoad();
		ShowEditCard(false);

		if (Abonents->ListCount() > 1) {
			TableFirst = Abonents->ListFirst();
		} else TableFirst = Abonents;

		TableLast = TableFirst;
		int end = 0;

		if (Abonents->ListCount() < (Console::Height() - 2)) {
			end = Abonents->ListCount();
		} else {
			end = (Console::Height() - 2);
		}

		if (TableLast->ListNext) {
			for (int i = 1; i < end; i++) {
				TableLast = TableLast->ListNext;
			}
		}

		if (Abonents->ListCount() > 1) {
			Abonents = Abonents->ListSort(Abonents);
		} else {
			CursorOn = Abonents;
			CursorPos = 1;

		}

		Connection->SaveAbon(CursorOn, false, Streets, Tarifs);
		CursorPos = JumpTo(CursorOn, false);

	}
	CursorOn->IndexChange();
	AbonShow = false;
	ShowAbonentCard(CursorOn, false);
	Console::GotoXY(0, CursorPos);
	ListsReLoad();
}

void Application::Balance_change(Abonent* Item) {
	ListsReLoad();
	int X = Console::Width() / 2;
	int Y = Console::Height() / 2;
	char* temp = StringHelper::New();
	char* bal = StringHelper::New();
	char* input = StringHelper::New();
	Console::FillRect(X - 25, Y - 4, X + 25, Y + 4, Console::clDarkGrey);

	X -= 23;
	Y -= 3;

	Console::GotoXY(X, Y);
	strcpy_s(temp, StringHelper::DefaultSize, " Текущий баланс: ");
	
	StringHelper::int_to_currency(bal, CursorOn->balance);
	strcat_s(temp, StringHelper::DefaultSize, bal);
	strcat_s(temp, StringHelper::DefaultSize, " ");
	Console::Print(temp, Console::clWhite, Console::clDarkGrey);

	Console::GotoY(Y += 2);
	Console::Print((char*)" Приход/расход: ", Console::clWhite, Console::clDarkGrey);

	Console::FillRect(X + 16, Y, X + 25, Y, Console::clLightGrey);
	Console::GotoXY(X + 17, Y);
	Console::ShowCursor(true);

	Console::SetColor(Console::clBlack, Console::clLightGrey);
	StringHelper::Input_currency(input);
	strcpy_s(temp, StringHelper::DefaultSize, input);
	if (std::strtol(temp, NULL, 10)) {
		CursorOn->balance += std::strtol(temp, NULL, 10);
		Connection->SavePay(CursorOn, std::strtol(temp, NULL, 10));
		if (CursorOn->balance <= 0 && CursorOn->state == 0) {
			CursorOn->state = 1;
			Connection->SaveAbon(CursorOn, false, Streets, Tarifs);
		}
		else if (CursorOn->balance > 0 && CursorOn->state == 1) {
			CursorOn->state = 0;
			strcpy_s(Abonents->last_pay, StringHelper::DefaultSize, today);
			Connection->SaveAbon(CursorOn, false, Streets, Tarifs);
		}
	}
	Console::ShowCursor(false);
	
	ListsReLoad();
	if (AbonShow) {
		AbonShow = false;
		ShowAbonentCard(CursorOn, false);
	}
	Console::GotoXY(0, CursorPos);

	free(bal);
	free(temp);
	free(input);
}

void Application::OnExit() {
	Console::GotoXY(0, 0);
	Console::SetColor(Console::clWhite);
}

void Application::Init() {
	FreeGreyIPs = Connection->LoadGreyIPs();
	FreeRealIPs = Connection->LoadRealIPs();
	Streets = Connection->LoadStreets();
	Tarifs = Connection->LoadTarifs();
	CellCodes = Connection->LoadCellCodes();
	Abonents = Connection->LoadAbons(Streets, Tarifs);

}

int Application::JumpTo(Abonent* toItem, bool New) {

	if (New) {
		if (Abonents->ListCount() < Console::Height() - 1) {
			TableFirst = Abonents->ListFirst();
			TableLast = Abonents->ListLast();
		} else {
			TableFirst = TableFirst->ListFirst();
			TableLast = TableFirst;

			for (int i = 0; i < Console::Height() - 3; i++) {
				TableLast = TableLast->ListNext;
			}
		}

		CursorOn = TableFirst;
		CursorPos = 1;

		if (strcmp(CursorOn->fio, toItem->fio) != 0) {
			while (strcmp(CursorOn->fio, toItem->fio) != 0) {
				if (TableLast->ListNext) {
					TableLast = TableLast->ListNext;
					CursorOn = CursorOn->ListNext;
					TableFirst = TableFirst->ListNext;
				} else {
					CursorOn = CursorOn->ListNext;
					CursorPos++;
				}
			}
		} else {
			printf("");
		}
	} else {
		CursorOn = TableFirst;
		if (strcmp(CursorOn->fio, toItem->fio) != 0) {
			while (strcmp(CursorOn->fio, toItem->fio) != 0) {
				CursorOn = CursorOn->ListNext;
			}
		}
	}
	return CursorPos;
}

void Application::Search() {
	int X = Console::Width() / 2;
	int Y = Console::Height() / 2;
	char* temp = StringHelper::New();
	int Width = 35;
	int Height = 4;

	Street* Search_street = Streets;
	Abonent* Search_abon = Abonents->ListFirst();
	int search_house = 0;
	int search_appart = 0;
	X -= 20;
	Y -= 2;

	Console::FillRect(X, Y, X + Width, Y + Height, Console::clCyan);
	X += 2;
	Y++;
	Console::GotoXY(X, Y);
	Console::Print((char*)"ул. ", Console::clYellow, Console::clCyan);
	Console::GotoXY(X + 1, Y + 2);
	Console::Print((char*)"д. ", Console::clYellow, Console::clCyan);
	Console::GotoXY(X + 11, Y + 2);
	Console::Print((char*)"кв. ", Console::clYellow, Console::clCyan);
	Console::ShowCursor(true);
	X += 5;

	Console::FillRect(X - 1, Y + 2, X + 3, Y + 2, Console::clLightGrey);
	Console::FillRect(X + 10, Y + 2, X + 14, Y + 2, Console::clLightGrey);
	Console::GotoXY(X, Y);

	Search_street = Search_street->DrawChoiceList();

	Y += 2;
	Console::GotoXY(X, Y);
	StringHelper::InputDigit(temp, 3, false);
	search_house = atoi(temp);
	StringHelper::Null(temp);

	Console::GotoXY(X + 11, Y);
	StringHelper::InputDigit(temp, 3, true);
	search_appart = atoi(temp);

	while (Search_abon) {
		if (Search_abon->StreetPTR == Search_street && Search_abon->House == search_house && Search_abon->Apartment == search_appart) {

			break;
		} else Search_abon = Search_abon->ListNext;
	}

	if (Search_abon) {
		CursorPos = JumpTo(Search_abon, true);
	} else {
		Console::ShowWarning((char*)"Абонент не найден!", CursorPos);
	}

	AbonShow = false;
	ShowAbonentCard(Search_abon, false);
	Console::GotoXY(0, CursorPos);
	TableDraw();
	free(temp);
}

void Application::Check_login(char* str, const int length) {
	bool is_unique = false;
	char* old_pass = StringHelper::New();
	char* new_pass = StringHelper::New();
	strcpy_s(old_pass, StringHelper::DefaultSize, str);
	strcpy_s(new_pass, StringHelper::DefaultSize, str);

	Abonent* AbonList = Abonents->ListFirst();
	short X = Console::X();
	short Y = Console::Y();

	while (!is_unique) {
		StringHelper::InputEng(new_pass, length);
		if (strcmp(old_pass, new_pass)) {
			while (AbonList) {
				if (!strcmp(new_pass, AbonList->login)) {
					is_unique = false;
					break;
				} else {
					is_unique = true;
				}
				AbonList = AbonList->ListNext;
			}
			if (!is_unique) {
				Console::ShowWarning((char*)"Логин должен быть уникальным!", Y);
				TableDraw();
				Console::GotoXY(X, Y);
				AbonList = Abonents->ListFirst();
			}
		} else is_unique = true;
	}

	strcpy_s(str, StringHelper::DefaultSize, new_pass);
	free(new_pass);
	free(old_pass);
};

void Application::ListsNULL() {
	Abonents = Abonents->ListLast();
	while (Abonents) {
		Abonent* Temp = Abonents;
		Abonents = Abonents->ListPrev;
		delete Temp;
	}
	Abonents = NULL;

	Streets = Streets->ListLast();
	while (Streets) {
		Street* Temp = Streets;
		Streets = Streets->ListPrev;
		delete Temp;
	}
	Streets = NULL;

	Tarifs = Tarifs->ListLast();
	while (Tarifs) {
		Tarif* Temp = Tarifs;
		Tarifs = Tarifs->ListPrev;
		delete Temp;
	}
	Tarifs = NULL;

	CellCodes = CellCodes->ListLast();
	while (CellCodes) {
		CellOper* Temp = CellCodes;
		CellCodes = CellCodes->ListPrev;
		delete Temp;
	}
	CellCodes = NULL;
};

void Application::ListsReLoad() {
	int CursorOn_id = CursorOn->id;
	int TableFirst_id = TableFirst->id;
	int TableLast_id = TableLast->id;
	char* last_our_time = StringHelper::New();
	strcpy_s(last_our_time, StringHelper::DefaultSize, Connection->lastupdate);
	Connection->GetLastUpdatetime();

	if (strcmp(last_our_time, Connection->lastupdate)) {

		ListsNULL();
		Init();
		CursorOn = Abonents;
		TableFirst = Abonents->Get_by_id(TableFirst_id);
		TableLast = Abonents->Get_by_id(TableLast_id);
		CursorPos = JumpTo(Abonents->Get_by_id(CursorOn_id), false);

		TableDraw();
		if (AbonShow) {
			AbonShow = false;
			ShowAbonentCard(CursorOn, false);
		}
	}
	free(last_our_time);
}

void Application::IPReLoad() {
	FreeGreyIPs = FreeGreyIPs->ListLast();
	while (FreeGreyIPs) {
		Free_grey_IP* Temp = FreeGreyIPs;
		FreeGreyIPs = FreeGreyIPs->ListPrev;
		delete Temp;
	}
	FreeGreyIPs = NULL;
	FreeGreyIPs = Connection->LoadGreyIPs();

	FreeRealIPs = FreeRealIPs->ListLast();
	while (FreeRealIPs) {
		Free_real_IP* Temp = FreeRealIPs;
		FreeRealIPs = FreeRealIPs->ListPrev;
		delete Temp;
	}
	FreeRealIPs = NULL;
	FreeRealIPs = Connection->LoadRealIPs();
}

void Application::GetTime() {
	now = time(0);
	localtime_s(&ltm, &now);
}

void Application::Today() {
	GetTime();
	char* temp = StringHelper::New();
	StringHelper::int_to_str(temp, 1900 + ltm.tm_year);
	strcpy_s(today, StringHelper::DefaultSize, temp);
	strcat_s(today, StringHelper::DefaultSize, "-");
	if (ltm.tm_mon < 10) {
		strcat_s(today, StringHelper::DefaultSize, "0");
	}
	StringHelper::int_to_str(temp, ltm.tm_mon);
	strcat_s(today, StringHelper::DefaultSize, temp);
	strcat_s(today, StringHelper::DefaultSize, "-");
	if (ltm.tm_mon < 10) {
		strcat_s(today, StringHelper::DefaultSize, "0");
	}
	StringHelper::int_to_str(temp, ltm.tm_mday);
	strcat_s(today, StringHelper::DefaultSize, temp);
	free(temp);
}