#include <iostream>
#include <Windows.h>
#include "Application.h"
#include "Console.h"
#include "Abonent.h"

void App_OnKeyEscape(Application* Sender) {
	if (Sender->AbonShow) {
		int CursorPos = Console::Y();
		Sender->AbonShow = false;
		Sender->TableDraw(CursorPos);
		Sender->DrawMenu(Console::Height() - 1, Sender->HelpString);
		Sender->DrawMenu(0, Sender->TableString);
		Console::GotoXY(0, CursorPos);
	} else Sender->Running = false;
}

void App_OnKeyDown(Application* Sender) {
	if (Sender->CursorOn) {
		Sender->TableDrawOnKeyDown(Console::Y());
	}
}

void App_OnKeyUp(Application* Sender) {
	if (Sender->CursorOn) {
		Sender->TableDrawOnKeyUp(Console::Y());
	}
}

void App_OnPageDown(Application* Sender) {
	if (Sender->CursorOn) {
		Sender->TableDrawOnPageDown(Console::Y());
	}
}

void App_OnPageUp(Application* Sender) {
	if (Sender->CursorOn) {
		Sender->TableDrawOnPageUp(Console::Y());
	}
}

void App_OnEnter(Application* Sender) {
	if (Sender->CursorOn) {
		if (!Sender->AbonShow) {
			Sender->ShowAbonentCard(Sender->CursorOn, false);
		} else {
			int CursorPos = Console::Y();
			Sender->AbonShow = false;
			Sender->TableDraw(CursorPos);
			Sender->DrawMenu(Console::Height() - 1, Sender->HelpString);
			Sender->DrawMenu(0, Sender->TableString);
			Console::GotoXY(0, CursorPos);
		}
	}
}

void App_OnF5(Application* Sender) {
	if (!Sender->AbonShow) {
		Sender->AbonAdd(Sender->Abonents, Console::Y());
	} else Sender->ShowEditCard(Console::Y(), false);
}

void App_OnF6(Application* Sender) {
	Sender->Balance_change(Sender->CursorOn, Console::Y());
}

void App_OnF7(Application* Sender) {
	Sender->Search(Console::Y());
}

void App_OnF8(Application* Sender) {
	if (Sender->AbonShow) {
		if (Sender->CursorOn && Sender->CursorOn->balance <= 0) { // Запрет удаления абонентов с положительным балансом
		//if (Sender->CursorOn) {
			if (Sender->CursorOn->ListNext || Sender->CursorOn->ListPrev) {
				Sender->AbonDel(Sender->CursorOn, Console::Y());
			} else {
				Sender->AbonDel(Sender->CursorOn, Console::Y());
				Sender->DrawMenu(Console::Height() - 1, Sender->HelpString);
				Sender->DrawMenu(0, Sender->TableString);
			}
		}
	}
}

int main() {
	Application* App = new Application;
	App->OnEscape = &App_OnKeyEscape;
	App->OnDown = &App_OnKeyDown;
	App->OnUp = &App_OnKeyUp;
	App->OnPageDown = &App_OnPageDown;
	App->OnPageUp = &App_OnPageUp;
	App->OnEnter = &App_OnEnter;
	App->OnF5 = &App_OnF5;
	App->OnF6 = &App_OnF6;
	App->OnF7 = &App_OnF7;
	App->OnF8 = &App_OnF8;
	App->Run();
	delete App;
}