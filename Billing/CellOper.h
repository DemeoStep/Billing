#ifndef CELLOPER_H
#define CELLOPER_H
class CellOper {
public:
	char* code;

	CellOper();
	~CellOper();

	CellOper* ListAdd(CellOper* ExistingItem);

	CellOper* ListFirst();

	CellOper* ListLast();

	int ListCount();

	void ListSwap(CellOper* Item_1, CellOper* Item_2);

	CellOper* ListSort(CellOper* Item);

	CellOper* ListPrev;
	CellOper* ListNext;
};

#endif