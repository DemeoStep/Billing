#ifndef FREEIP_H
#define FREEIP_H

#include <iostream>
class Abonent;

class FreeIP {
public:
	char* ip;

	FreeIP();
	~FreeIP();

	FreeIP* ListNext;
	FreeIP* ListPrev;

	FreeIP* ListAdd(FreeIP* ExistingItem);
	FreeIP* ListDel();
	FreeIP* ListFirst();
	FreeIP* ListLast();
	int ListCount();

};

#endif