#include "FreeIP.h"
#include <iostream>
#include "StringHelper.h"
#include "Abonent.h"

FreeIP::FreeIP() {
	ip = StringHelper::New(StringHelper::DefaultSize);

	ListPrev = NULL;
	ListNext = NULL;
}

FreeIP::~FreeIP() {
	if (NULL != ListPrev) {
		ListPrev->ListNext = ListNext;
	}
	if (NULL != ListNext) {
		ListNext->ListPrev = ListPrev;
	}

	free(ip);
}

FreeIP* FreeIP::ListAdd(FreeIP* ExistingItem) {
	FreeIP* LResult = ExistingItem;
	LResult->ListPrev = this;
	LResult->ListNext = this->ListNext;
	if (this->ListNext) {
		this->ListNext->ListPrev = LResult;
	}
	this->ListNext = LResult;
	return LResult;
}

FreeIP* FreeIP::ListDel() {
	FreeIP* IPNext = this->ListNext;
	FreeIP* IPPrev = this->ListPrev;
	if (IPPrev) {
		IPPrev->ListNext = this->ListNext;
	}
	if (IPNext) {
		IPNext->ListPrev = this->ListPrev;
	}
	delete this;
	if (IPNext) {
		return IPNext;
	} else if (IPPrev) {
		return IPPrev;
	} else return NULL;
}

FreeIP* FreeIP::ListFirst() {
	FreeIP* LResult = this;
	while (LResult->ListPrev) {
		LResult = LResult->ListPrev;
	}
	return LResult;
}

FreeIP* FreeIP::ListLast() {
	FreeIP* LResult = this;
	while (LResult->ListNext) {
		LResult = LResult->ListNext;
	}
	return LResult;
}

int FreeIP::ListCount() {
	int LResult = 0;
	FreeIP* LItem = ListFirst();
	if (LItem) LResult++;
	while (NULL != LItem->ListNext) {
		LResult++;
		LItem = LItem->ListNext;
	}
	return LResult;
}