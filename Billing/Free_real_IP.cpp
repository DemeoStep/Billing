#include "Free_real_IP.h"
#include <iostream>
#include "StringHelper.h"
#include "Abonent.h"

Free_real_IP::Free_real_IP() {
	ip = StringHelper::New(StringHelper::DefaultSize);

	ListPrev = NULL;
	ListNext = NULL;
}

Free_real_IP::~Free_real_IP() {
	if (NULL != ListPrev) {
		ListPrev->ListNext = ListNext;
	}
	if (NULL != ListNext) {
		ListNext->ListPrev = ListPrev;
	}

	free(ip);
}

Free_real_IP* Free_real_IP::ListAdd(Free_real_IP* ExistingItem) {
	Free_real_IP* LResult = ExistingItem;
	LResult->ListPrev = this;
	LResult->ListNext = this->ListNext;
	if (this->ListNext) {
		this->ListNext->ListPrev = LResult;
	}
	this->ListNext = LResult;
	return LResult;
}

Free_real_IP* Free_real_IP::ListDel() {
	Free_real_IP* IPNext = this->ListNext;
	Free_real_IP* IPPrev = this->ListPrev;
	if (IPPrev) {
		IPPrev->ListNext = this->ListNext;
	}
	if (IPNext) {
		IPNext->ListPrev = this->ListPrev;
	}
	delete this;
	if (IPNext) {
		IPNext->SaveToFile("bin\\config\\free_real_ip.cfg");
		return IPNext;
	} else if (IPPrev) {
		IPPrev->SaveToFile("bin\\config\\free_real_ip.cfg");
		return IPPrev;
	}
	else return NULL;
}

Free_real_IP* Free_real_IP::ListFirst() {
	Free_real_IP* LResult = this;
	while (LResult->ListPrev) {
		LResult = LResult->ListPrev;
	}
	return LResult;
}

Free_real_IP* Free_real_IP::ListLast() {
	Free_real_IP* LResult = this;
	while (LResult->ListNext) {
		LResult = LResult->ListNext;
	}
	return LResult;
}

int Free_real_IP::ListCount() {
	int LResult = 0;
	Free_real_IP* LItem = ListFirst();
	if (LItem) LResult++;
	while (NULL != LItem->ListNext) {
		LResult++;
		LItem = LItem->ListNext;
	}
	return LResult;
}

void Free_real_IP::ListSwap(Free_real_IP* Item_1, Free_real_IP* Item_2) {
	if ((Item_1 != Item_2) && (Item_1 || Item_2)) {
		Free_real_IP* L_Prev = NULL;
		Free_real_IP* L_Next = NULL;
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

Free_real_IP* Free_real_IP::ListSort(Free_real_IP* Item) {
	if (Item->ListCount() > 1) {
		while (Item->ListPrev) {
			int first = GetLastOctet(Item);
			int second = GetLastOctet(Item->ListPrev);
			if (strcmp(Item->ip, Item->ListPrev->ip) < 0) {
				ListSwap(Item, Item->ListPrev);
			} else if (first < second) {
				ListSwap(Item, Item->ListPrev);
			} else break;
		};
		return ListFirst();
	} else {
		return Item;
	}
}

int Free_real_IP::GetLastOctet(Free_real_IP* Item) {
	int LResult = 0;
	char* temp = StringHelper::New();
	char** context = (char**)calloc(StringHelper::DefaultSize, sizeof(char*));
	strcpy_s(temp, StringHelper::DefaultSize, Item->ip);
	strtok_s(temp, ".", context);
	strtok_s(NULL, ".", context);
	strtok_s(NULL, ".", context);
	LResult = atoi(*context);

	free(temp);
	free(context);
	return LResult;
};

Free_real_IP* Free_real_IP::ipRestore(Abonent* Abon) {
	Free_real_IP* IP = this->ListLast();
	if (Abon->IP) {
		IP = IP->ListAdd(new Free_real_IP);
		strcpy_s(IP->ip, StringHelper::DefaultSize, Abon->IP);
	}
	IP = ListSort(IP);
	IP->SaveToFile("bin\\config\\free_real_ip.cfg");
	return IP;
}

void Free_real_IP::SaveIP(FILE* FileHandle, Free_real_IP* Item) {
	fprintf(FileHandle, "%s\n", Item->ip);
}

void Free_real_IP::SaveToFile(const char* FileName) {
	FILE* LFileHandle;
	int LFileOpenError = fopen_s(&LFileHandle, FileName, "w+");
	if (0 == LFileOpenError) {
		Free_real_IP* LItem = this->ListFirst();
		while (LItem) {
			SaveIP(LFileHandle, LItem);
			LItem = LItem->ListNext;
		}
		fclose(LFileHandle);
	} else {
		printf("Не удалось сохранить данные в файл %s \n", FileName);
	}
}

Free_real_IP* Free_real_IP::LoadFromFile(const char* FileName) {
	Free_real_IP* List = NULL;
	FILE* LFile;
	int LFileOpenError = fopen_s(&LFile, FileName, "r");
	if (!LFileOpenError) {
		char** context = (char**)calloc(StringHelper::DefaultSize, sizeof(char*));
		char* network = StringHelper::New(StringHelper::DefaultSize);

		while (!feof(LFile)) {
			fgets(network, StringHelper::DefaultSize * sizeof(char), LFile);
			if (feof(LFile)) break;
			if (List) {
				List = List->ListAdd(new Free_real_IP);
			} else {
				List = new Free_real_IP;
			}
			strcpy_s(List->ip, StringHelper::DefaultSize, strtok_s(network, "\n", context));
			StringHelper::Null(network, StringHelper::DefaultSize);
			printf("");
		}

		free(context);
		free(network);
		fclose(LFile);
		return List->ListFirst();
	} else {
		printf("Ошибка открытия файла\n");
		return NULL;
	}
}