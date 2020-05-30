#ifndef FREE_REAL_IP_H
#define FREE_REAL_IP_H

#include <iostream>
class Abonent;

class Free_real_IP {
public:
	char* ip;
	Free_real_IP();
	~Free_real_IP();

	Free_real_IP* ListNext;
	Free_real_IP* ListPrev;

	Free_real_IP* ListAdd(Free_real_IP* ExistingItem);
	Free_real_IP* ListDel();
	Free_real_IP* ListFirst();
	Free_real_IP* ListLast();
	int ListCount();
	void ListSwap(Free_real_IP* Item_1, Free_real_IP* Item_2);
	Free_real_IP* ListSort(Free_real_IP* Item);
	int GetLastOctet(Free_real_IP* Item);
	Free_real_IP* ipRestore(Abonent* Abon);
	void SaveIP(FILE* FileHandle, Free_real_IP* Item);
	void SaveToFile(const char* FileName);
	Free_real_IP* LoadFromFile(const char* FileName);
};

#endif