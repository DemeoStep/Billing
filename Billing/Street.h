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
	int ListCount();
	void ListSwap(Street* Item_1, Street* Item_2);
	Street* ListSort(Street* Item);
	Street* Get_by_id(int id);
	Street* DrawChoiceList();
};

#endif