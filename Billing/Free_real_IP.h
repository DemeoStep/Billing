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

};

#endif