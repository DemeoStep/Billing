#include "Street.h"
#include "Console.h"
#include "StringHelper.h"
#include "Application.h"

Street::Street() {
	id = 0;
	name = StringHelper::New();

	ListPrev = NULL;
	ListNext = NULL;
}

Street::~Street() {
	if (NULL != ListPrev) {
		ListPrev->ListNext = ListNext;
	}
	if (NULL != ListNext) {
		ListNext->ListPrev = ListPrev;
	}

	free(name);
}

Street* Street::ListAdd(Street* ExistingItem) {
	Street* LResult = ExistingItem;
	LResult->ListPrev = this;
	LResult->ListNext = this->ListNext;
	if (this->ListNext) {
		this->ListNext->ListPrev = LResult;
	}
	this->ListNext = LResult;
	return LResult;
}

Street* Street::ListFirst() {
	Street* LResult = this;
	while (LResult->ListPrev) {
		LResult = LResult->ListPrev;
	}
	return LResult;
}

Street* Street::ListLast() {
	Street* LResult = this;
	while (LResult->ListNext) {
		LResult = LResult->ListNext;
	}
	return LResult;
}

int Street::ListCount() {
	int LResult = 0;
	Street* LItem = ListFirst();
	if (LItem) LResult++;
	while (NULL != LItem->ListNext) {
		LResult++;
		LItem = LItem->ListNext;
	}
	return LResult;
}

Street* Street::Get_by_id(int id) {
	Street* LResult = this->ListFirst();
	while (id != LResult->id) {
		LResult = LResult->ListNext;
	}

	return LResult;
};

Street* Street::DrawChoiceList() {
	short X = Console::X();
	short Y = Console::Y();
	Street* Choice = this;
	char* temp = StringHelper::New();
	char* str = StringHelper::New();

	Console::FillRect(X - 1, Y, X + 21, Y, Console::clYellow);
	strcpy_s(temp, StringHelper::DefaultSize, Choice->name);
	StringHelper::StrToSize(temp, 20);
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
		StringHelper::StrToSize(temp, 20);
		Console::Print(temp, Console::clBlack, Console::clYellow);
		keyPressed = Console::GetKey();
		*str = keyPressed;
		*(str + 1) = '\0';
	}

	Console::GotoXY(X, Y);
	Console::FillRect(X - 1, Y, X + 21, Y, Console::clLightGrey);
	Console::Print(Choice->name, Console::clBlack, Console::clLightGrey);

	free(str);
	free(temp);

	return Choice;
}