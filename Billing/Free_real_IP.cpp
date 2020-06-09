#include "Free_real_IP.h"
#include <iostream>
#include "StringHelper.h"
#include "Abonent.h"

Free_real_IP::Free_real_IP() {
	ip = StringHelper::New(StringHelper::DefaultSize);

	ListPrev = NULL;
	ListNext = NULL;
}

Free_real_IP::~Free_real_IP() {
	if (NULL != ListPrev) {
		ListPrev->ListNext = ListNext;
	}
	if (NULL != ListNext) {
		ListNext->ListPrev = ListPrev;
	}

	free(ip);
}

Free_real_IP* Free_real_IP::ListAdd(Free_real_IP* ExistingItem) {
	Free_real_IP* LResult = ExistingItem;
	LResult->ListPrev = this;
	LResult->ListNext = this->ListNext;
	if (this->ListNext) {
		this->ListNext->ListPrev = LResult;
	}
	this->ListNext = LResult;
	return LResult;
}

Free_real_IP* Free_real_IP::ListDel() {
	Free_real_IP* IPNext = this->ListNext;
	Free_real_IP* IPPrev = this->ListPrev;
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

Free_real_IP* Free_real_IP::ListFirst() {
	Free_real_IP* LResult = this;
	while (LResult->ListPrev) {
		LResult = LResult->ListPrev;
	}
	return LResult;
}

Free_real_IP* Free_real_IP::ListLast() {
	Free_real_IP* LResult = this;
	while (LResult->ListNext) {
		LResult = LResult->ListNext;
	}
	return LResult;
}

int Free_real_IP::ListCount() {
	int LResult = 0;
	Free_real_IP* LItem = ListFirst();
	if (LItem) LResult++;
	while (NULL != LItem->ListNext) {
		LResult++;
		LItem = LItem->ListNext;
	}
	return LResult;
}