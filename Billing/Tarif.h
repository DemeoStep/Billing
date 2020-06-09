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
	Tarif* ListLast();
	int ListCount();

	Tarif* Get_by_id(int id);

	Tarif* DrawChoiceList();



};

#endif
