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

	CellOper* ListPrev;
	CellOper* ListNext;
};

#endif