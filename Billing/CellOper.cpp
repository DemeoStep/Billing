#include "CellOper.h"
#include "StringHelper.h"

CellOper::CellOper() {
	code = StringHelper::New();

	ListNext = NULL;
	ListPrev = NULL;
}

CellOper::~CellOper() {
	if (NULL != ListPrev) {
		ListPrev->ListNext = ListNext;
	}
	if (NULL != ListNext) {
		ListNext->ListPrev = ListPrev;
	}

	free(code);
}

CellOper* CellOper::ListAdd(CellOper* ExistingItem) {
	CellOper* LResult = ExistingItem;
	LResult->ListPrev = this;
	LResult->ListNext = this->ListNext;
	if (this->ListNext) {
		this->ListNext->ListPrev = LResult;
	}
	this->ListNext = LResult;
	return LResult;
}

CellOper* CellOper::ListFirst() {
	CellOper* LResult = this;
	while (LResult->ListPrev) {
		LResult = LResult->ListPrev;
	}
	return LResult;
}

CellOper* CellOper::ListLast() {
	CellOper* LResult = this;
	while (LResult->ListNext) {
		LResult = LResult->ListNext;
	}
	return LResult;
}

int CellOper::ListCount() {
	int LResult = 0;
	CellOper* LItem = ListFirst();
	if (LItem) LResult++;
	while (NULL != LItem->ListNext) {
		LResult++;
		LItem = LItem->ListNext;
	}
	return LResult;
}

void CellOper::ListSwap(CellOper* Item_1, CellOper* Item_2) {
	if ((Item_1 != Item_2) && (Item_1 || Item_2)) {
		CellOper* L_Prev = NULL;
		CellOper* L_Next = NULL;
		if (Item_1->ListNext == Item_2 && Item_2->ListPrev == Item_1) {
			L_Prev = Item_1->ListPrev;
			L_Next = Item_2->ListNext;
			Item_1->ListNext = L_Next;
			Item_1->ListPrev = Item_2;
			Item_2->ListNext = Item_1;
			Item_2->ListPrev = L_Prev;
			if (L_Next) {
				L_Next->ListPrev = Item_1;
			}
			if (L_Prev) {
				L_Prev->ListNext = Item_2;
			}
		} else if (Item_2->ListNext == Item_1 && Item_1->ListPrev == Item_2) {
			L_Prev = Item_2->ListPrev;
			L_Next = Item_1->ListNext;
			Item_2->ListNext = L_Next;
			Item_2->ListPrev = Item_1;
			Item_1->ListNext = Item_2;
			Item_1->ListPrev = L_Prev;
			if (L_Next) {
				L_Next->ListPrev = Item_2;
			}
			if (L_Prev) {
				L_Prev->ListNext = Item_1;
			}
		} else {
			L_Prev = Item_1->ListPrev;
			L_Next = Item_1->ListNext;

			Item_1->ListPrev = Item_2->ListPrev;
			Item_2->ListPrev = L_Prev;
			Item_1->ListNext = Item_2->ListNext;
			Item_2->ListNext = L_Next;
			if (Item_1->ListNext) {
				Item_1->ListNext->ListPrev = Item_1;
			}
			if (Item_1->ListPrev) {
				Item_1->ListPrev->ListNext = Item_1;
			}
			if (Item_2->ListNext) {
				Item_2->ListNext->ListPrev = Item_2;
			}
			if (Item_2->ListPrev) {
				Item_2->ListPrev->ListNext = Item_2;
			}
		}
	}
}

CellOper* CellOper::ListSort(CellOper* Item) {
	if (Item->ListCount() > 1) {
		while (Item->ListNext) {
			if (strcmp(Item->code, Item->ListNext->code) > 0) {
				ListSwap(Item, Item->ListNext);
				ListSort(Item);
			} else ListSort(Item->ListNext);
			return ListFirst();
		};
		return ListFirst();
	} else return Item;
}