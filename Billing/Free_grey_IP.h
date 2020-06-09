#ifndef FREE_GREY_IP_H
#define FREE_GREY_IP_H

#include <iostream>
class Abonent;

class Free_grey_IP {
public:
	char* ip;

	Free_grey_IP();
	~Free_grey_IP();

	Free_grey_IP* ListNext;
	Free_grey_IP* ListPrev;

	Free_grey_IP* ListAdd(Free_grey_IP* ExistingItem);
	Free_grey_IP* ListDel();
	Free_grey_IP* ListFirst();
	Free_grey_IP* ListLast();
	int ListCount();

};

#endif