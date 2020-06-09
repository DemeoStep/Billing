#include <iostream>
#include "Tarif.h"
#include "Console.h"
#include "StringHelper.h"

Tarif::Tarif() {
	id = -1;
	name = StringHelper::New();
	isReal = false;
	price = 0;

	ListPrev = NULL;
	ListNext = NULL;
}

Tarif::~Tarif() {
	if (NULL != ListPrev) {
		ListPrev->ListNext = ListNext;
	}
	if (NULL != ListNext) {
		ListNext->ListPrev = ListPrev;
	}

	free(name);
}

Tarif* Tarif::ListAdd(Tarif* ExistingItem) {
	Tarif* LResult = ExistingItem;
	LResult->ListPrev = this;
	LResult->ListNext = this->ListNext;
	if (this->ListNext) {
		this->ListNext->ListPrev = LResult;
	}
	this->ListNext = LResult;
	return LResult;
}

Tarif* Tarif::ListFirst() {
	Tarif* LResult = this;
	while (LResult->ListPrev) {
		LResult = LResult->ListPrev;
	}
	return LResult;
}

Tarif* Tarif::ListLast() {
	Tarif* LResult = this;
	while (LResult->ListNext) {
		LResult = LResult->ListNext;
	}
	return LResult;
}

int Tarif::ListCount() {
	int LResult = 0;
	Tarif* LItem = ListFirst();
	if (LItem) LResult++;
	while (NULL != LItem->ListNext) {
		LResult++;
		LItem = LItem->ListNext;
	}
	return LResult;
}

Tarif* Tarif::Get_by_id(int id) {
	Tarif* LResult = this->ListFirst();
	while (id != LResult->id) {
		LResult = LResult->ListNext;
	}

	return LResult;
};

Tarif* Tarif::DrawChoiceList() {
	short X = Console::X();
	short Y = Console::Y();
	Tarif* Choice = this;
	char* temp = StringHelper::New();
	char* str = StringHelper::New();

	Console::FillRect(X - 1, Y, X + 41, Y, Console::clYellow);
	strcpy_s(temp, StringHelper::DefaultSize, Choice->name);
	StringHelper::StrToSize(temp, 40);
	int keyPressed = 0;

	while (keyPressed != Console::keyEnter) {
		if (keyPressed == Console::keyDown) {
			if (Choice->ListNext) Choice = Choice->ListNext;
		} else if (keyPressed == Console::keyUp) {
			if (Choice->ListPrev) Choice = Choice->ListPrev;
		} else if (*str >= 'À' && *str <= 'ÿ') {
			*str = toupper(*str);
			Choice = Choice->ListFirst();
			while (!strstr(Choice->name, str)) {
				if (Choice->ListNext) {
					Choice = Choice->ListNext;
				} else break;
			}
		}

		strcpy_s(temp, StringHelper::DefaultSize, Choice->name);
		StringHelper::StrToSize(temp, 40);
		Console::Print(temp, Console::clBlack, Console::clYellow);
		keyPressed = Console::GetKey();
		*str = keyPressed;
		*(str + 1) = '\0';
	}

	Console::GotoXY(X, Y);
	Console::FillRect(X - 1, Y, X + 41, Y, Console::clLightGrey);
	Console::Print(Choice->name, Console::clBlack, Console::clLightGrey);

	free(str);
	free(temp);

	return Choice;
}