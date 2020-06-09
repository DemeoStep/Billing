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