#ifndef STREET_H
#define STREET_H

class Street {
public:
	int id;
	char* name;
	Street();
	~Street();

	Street* ListPrev;
	Street* ListNext;

	Street* ListAdd(Street* ExistingItem);
	Street* ListFirst();
	Street* ListLast();
	int ListCount();
	Street* Get_by_id(int id);
	Street* DrawChoiceList();
};

#endif