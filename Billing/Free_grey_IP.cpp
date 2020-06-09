#include "Free_grey_IP.h"
#include <iostream>
#include "StringHelper.h"
#include "Abonent.h"

Free_grey_IP::Free_grey_IP() {
	ip = StringHelper::New(StringHelper::DefaultSize);

	ListPrev = NULL;
	ListNext = NULL;
}

Free_grey_IP::~Free_grey_IP() {
	if (NULL != ListPrev) {
		ListPrev->ListNext = ListNext;
	}
	if (NULL != ListNext) {
		ListNext->ListPrev = ListPrev;
	}

	free(ip);
}

Free_grey_IP* Free_grey_IP::ListAdd(Free_grey_IP* ExistingItem) {
	Free_grey_IP* LResult = ExistingItem;
	LResult->ListPrev = this;
	LResult->ListNext = this->ListNext;
	if (this->ListNext) {
		this->ListNext->ListPrev = LResult;
	}
	this->ListNext = LResult;
	return LResult;
}

Free_grey_IP* Free_grey_IP::ListDel() {
	Free_grey_IP* IPNext = this->ListNext;
	Free_grey_IP* IPPrev = this->ListPrev;
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
	}
	else return NULL;
}

Free_grey_IP* Free_grey_IP::ListFirst() {
	Free_grey_IP* LResult = this;
	while (LResult->ListPrev) {
		LResult = LResult->ListPrev;
	}
	return LResult;
}

Free_grey_IP* Free_grey_IP::ListLast() {
	Free_grey_IP* LResult = this;
	while (LResult->ListNext) {
		LResult = LResult->ListNext;
	}
	return LResult;
}

int Free_grey_IP::ListCount() {
	int LResult = 0;
	Free_grey_IP* LItem = ListFirst();
	if (LItem) LResult++;
	while (NULL != LItem->ListNext) {
		LResult++;
		LItem = LItem->ListNext;
	}
	return LResult;
}