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

void Street::ListSwap(Street* Item_1, Street* Item_2) {
	if ((Item_1 != Item_2) && (Item_1 || Item_2)) {
		Street* L_Prev = NULL;
		Street* L_Next = NULL;
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

Street* Street::ListSort(Street* Item) {
	if (Item->ListCount() > 1) {
		while (Item->ListNext) {
			if (strcmp(Item->name, Item->ListNext->name) > 0) {
				ListSwap(Item, Item->ListNext);
				ListSort(Item);
			} else ListSort(Item->ListNext);
			return ListFirst();
		};
		return ListFirst();
	} else return Item;
}

Street* Street::LoadFromFile(const char* FileName) {
	Street* List = NULL;
	FILE* LFile;
	int LFileOpenError = fopen_s(&LFile, FileName, "r");
	if (!LFileOpenError) {
		char** context = (char**)calloc(StringHelper::DefaultSize, sizeof(char*));
		char* name = StringHelper::New(StringHelper::DefaultSize);

		while (!feof(LFile)) {
			fgets(name, StringHelper::DefaultSize * sizeof(char), LFile);
			if (feof(LFile)) break;
			if (List) {
				List = List->ListAdd(new Street);
			} else {
				List = new Street;
			}
			List->id = atoi(strtok_s(name, "|", context));
			strcpy_s(List->name, StringHelper::DefaultSize, strtok_s(NULL, "\n", context));
			StringHelper::Null(name, StringHelper::DefaultSize);
			printf("");
		}

		free(context);
		free(name);
		fclose(LFile);
		return List->ListFirst();
	} else {
		printf("Ошибка открытия файла\n");
		return NULL;
	}
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
	Choice = Choice->ListFirst();
	strcpy_s(temp, StringHelper::DefaultSize, Choice->name);
	StringHelper::StrToSize(temp, 20);
	int keyPressed = 0;

	while (keyPressed != Console::keyEnter) {
		if (keyPressed == Console::keyDown) {
			if (Choice->ListNext) Choice = Choice->ListNext;
		} else if (keyPressed == Console::keyUp) {
			if (Choice->ListPrev) Choice = Choice->ListPrev;
		} else if (*str >= 'А' && *str <= 'я') {
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