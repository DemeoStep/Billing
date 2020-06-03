#ifndef TARIF_H
#define TARIF_H

class Tarif {
public:
	int id;
	char* name;
	bool isReal;
	int price;
	Tarif();
	~Tarif();

	Tarif* ListPrev;
	Tarif* ListNext;

	Tarif* ListAdd(Tarif* ExistingItem);
	Tarif* ListFirst();
	int ListCount();
	void ListSwap(Tarif* Item_1, Tarif* Item_2);
	Tarif* ListSort(Tarif* Item);

	Tarif* Get_by_id(int id);

	Tarif* DrawChoiceList();



};

#endif
