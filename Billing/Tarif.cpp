#include <iostream>
#include "Tarif.h"
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

void Tarif::ListSwap(Tarif* Item_1, Tarif* Item_2) {
	if ((Item_1 != Item_2) && (Item_1 || Item_2)) {
		Tarif* L_Prev = NULL;
		Tarif* L_Next = NULL;
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

Tarif* Tarif::ListSort(Tarif* Item) {
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

void Tarif::SaveIP(FILE* FileHandle, Tarif* Item) {
	fprintf(FileHandle, "%d|%s|%d|%d\n",
		Item->id, Item->name, Item->isReal, Item->price);
}

void Tarif::SaveToFile(const char* FileName) {
	FILE* LFileHandle;
	int LFileOpenError = fopen_s(&LFileHandle, FileName, "w+");
	if (0 == LFileOpenError) {
		Tarif* LItem = this->ListFirst();
		while (LItem) {
			SaveIP(LFileHandle, LItem);
			LItem = LItem->ListNext;
		}
		fclose(LFileHandle);
	} else {
		printf("Не удалось сохранить данные в файл %s \n", FileName);
	}
}

Tarif* Tarif::LoadFromFile(const char* FileName) {
	Tarif* List = NULL;
	FILE* LFile;
	int LFileOpenError = fopen_s(&LFile, FileName, "r");
	if (!LFileOpenError) {
		char** context = (char**)calloc(StringHelper::DefaultSize, sizeof(char*));
		char* buffer = StringHelper::New(StringHelper::DefaultSize);

		while (!feof(LFile)) {
			fgets(buffer, StringHelper::DefaultSize * sizeof(char), LFile);
			if (feof(LFile)) break;
			if (List) {
				List = List->ListAdd(new Tarif);
			} else {
				List = new Tarif;
			}
			List->id = atoi(strtok_s(buffer, "|", context));
			strcpy_s(List->name, StringHelper::DefaultSize, strtok_s(NULL, "|", context));
			int is_real = atoi(strtok_s(NULL, "|", context));
			if (is_real == 0) List->isReal = false;
			else List->isReal = true;
			List->price = atoi(strtok_s(NULL, "|", context)); 
	
			StringHelper::Null(buffer, StringHelper::DefaultSize);
			printf("");
		}

		free(context);
		free(buffer);
		fclose(LFile);
		return List->ListFirst();
	} else {
		printf("Ошибка открытия файла\n");
		return NULL;
	}
}

Tarif* Tarif::Get_by_id(int id) {
	Tarif* LResult = this->ListFirst();
	while (id != LResult->id) {
		LResult = LResult->ListNext;
	}

	return LResult;
};