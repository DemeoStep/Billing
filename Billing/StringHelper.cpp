#include <iostream>
#include "StringHelper.h"
#include <string>
#include "Abonent.h"
#include "CellOper.h"
#include <conio.h>

void StringHelper::Null(char* str, const int length) {
	for (int i = 0; i < length; i++) {
		str[i] = 0;
	}
}

void StringHelper::Input(char* str, const int length){
	char* s = str;
	*s = getchar();
	int i = 0;
	while (i < length - 2 && *s != '\n') {
		s++;
		i++;
		*s = getchar();
	}
	*s = 0;
}

void StringHelper::InputPass(char* str, const int length) {
	char* s = str;
	*s = _getch();
	printf("*");
	int i = 0;
	while (i < length - 2 && *s != '\r') {
		s++;
		i++;
		*s = _getch();
		printf("*");
	}
	*s = 0;
}

void StringHelper::InputDigit(char* str, const int length, bool can_be_null) {
	short X = Console::X();
	short Y = Console::Y();
	Console::SetColor(Console::clBlack, Console::clYellow);
	Console::FillRect(X - 1, Y, X + length, Y, Console::clYellow);
	Console::ShowCursor(true);
	int keyPressed = 0;
	
	char* s = str;
	int i = strlen(str);
	printf("%s", str);
	for (int j = 0; j < i; j++) {
		s++;
	}

	keyPressed = 0;
	do {
		keyPressed = Console::GetKey();
		if (!key_is_func(keyPressed)) {
			*s = keyPressed;
			if (*s >= '0' && *s <= '9') {
				printf("%c", *s);
				s++;
				i++;
			}
		} else if (keyPressed == Console::keyBackspace) {
			if (strlen(str) > 0) {
				s--;
				i--;
				*s = 0;
				Console::GotoXY(Console::X() - 1, Y);
				printf("%c", *s);
				Console::GotoXY(Console::X() - 1, Y);
			}
		} else if (keyPressed == Console::keyEnter || keyPressed == Console::keyEscape){
			if (!i && can_be_null) {
				*s = '0';
				break;
			} else if (i) break;
		}
		if (strlen(str) == length) break;
	} while (strlen(str) < length);

	*s = 0;

	Console::FillRect(X - 1, Y, X + length, Y, Console::clLightGrey);
	Console::GotoXY(X, Y);
	Console::Print(str, Console::clBlack, Console::clLightGrey);
	Console::ShowCursor(false);
}

void StringHelper::InputEng(char* str, const int length) {
	short X = Console::X();
	short Y = Console::Y();
	Console::SetColor(Console::clBlack, Console::clYellow);
	Console::FillRect(X - 1, Y, X + length, Y, Console::clYellow);
	Console::ShowCursor(true);
	int keyPressed = 0;
	
	char* s = str;
	int i = strlen(str);
	printf("%s", str);
	for (int j = 0; j < i; j++) {
		s++;
	}

	keyPressed = Console::GetKey();
	while (strlen(str) < length) {
		if (!key_is_func(keyPressed)) {
			*s = keyPressed;
			*(s + 1) = 0;
			if ((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || (*s >= '0' && *s <= '9') || *s == '_') {
				printf("%c", *s);
				s++;
				i++;
			}
		} else if (keyPressed == Console::keySpace) {
			*s = ' ';
			printf("%c", *s);
			s++;
			i++;
		} else if (keyPressed == Console::keyBackspace) {
			if (strlen(str) > 0) {
				s--;
				i--;
				*s = 0;
				Console::GotoXY(Console::X() - 1, Y);
				printf("%c", *s);
				Console::GotoXY(Console::X() - 1, Y);
			}
		} else if (i && (keyPressed == Console::keyEnter || keyPressed == Console::keyEscape)) {
			break;
		}
		keyPressed = Console::GetKey();
	}

	*s = 0;

	Console::FillRect(X - 1, Y, X + length, Y, Console::clLightGrey);
	Console::GotoXY(X, Y);
	Console::Print(str, Console::clBlack, Console::clLightGrey);
	Console::ShowCursor(false);
}

void StringHelper::InputRus(char* str, const int length) {
	short X = Console::X();
	short Y = Console::Y();
	Console::SetColor(Console::clBlack, Console::clYellow);
	Console::FillRect(X - 1, Y, X + length, Y, Console::clYellow);
	Console::ShowCursor(true);
	int keyPressed = 0;
	
	char* s = str;
	int i = strlen(str);
	printf("%s", str);
	for (int j = 0; j < i; j++) {
		s++;
	}

	keyPressed = Console::GetKey();
	while (strlen(str) < length) {
		if (!key_is_func(keyPressed)) {
			*s = keyPressed;
			if ((*s >= 'À' && *s <= 'ÿ') || (*s >= '0' && *s <= '9') || *s == '_') {
				printf("%c", *s);
				s++;
				i++;
			}
		} else if (keyPressed == Console::keySpace) {
			*s = ' ';
			printf("%c", *s);
			s++;
			i++;
		} else if (keyPressed == Console::keyBackspace){
			if (strlen(str) > 0) {
				s--;
				i--;
				*s = 0;
				Console::GotoXY(Console::X() - 1, Y);
				printf("%c", *s);
				Console::GotoXY(Console::X() - 1, Y);
			}
		} else if (i && (keyPressed == Console::keyEnter || keyPressed == Console::keyEscape)) {
			break;
		}
		*s = 0;
		keyPressed = Console::GetKey();
	}
	
	*s = 0;

	Console::FillRect(X - 1, Y, X + length, Y, Console::clLightGrey);
	Console::GotoXY(X, Y);
	Console::Print(str, Console::clBlack, Console::clLightGrey);
	Console::ShowCursor(false);
}

void StringHelper::Input_currency(char* str, const int length) {
	int keyPressed = 0;
	bool end = false;
	short X = Console::X();
	keyPressed = _getch();
	*str = keyPressed;
	while (*str != '.' && *str != ',' && *str != '-' && !(*str >= '0' && *str <= '9') && keyPressed != Console::keyEscape && keyPressed != Console::keyEnter) {
		keyPressed = _getch();
		*str = keyPressed;
	}
	if (keyPressed == Console::keyEscape || keyPressed == Console::keyEnter) return;
	if (*str == '-') {
		printf("%c", *str);
		str++;
		X++;
		while (*str != '.' && *str != ',' && !(*str >= '0' && *str <= '9') && keyPressed != Console::keyEscape && keyPressed != Console::keyEnter) {
			keyPressed = _getch();
			*str = keyPressed;
		}
	}
	if (keyPressed == Console::keyEscape) {
		Null(str);
		return;
	}
	printf("%c", *str);
	int i = 0;
	while (i < length - 4) {
		if (*str == '.' || *str == ',' || (*str >= '0' && *str <= '9') || keyPressed == Console::keyEscape || keyPressed == Console::keyEnter) {
			if (*str == '0' && i == 0) {
				printf(".");
				i++;
				X++;
				end = false;
				break;
			} else i++;
			if (*str >= '0' && *str <= '9' && i > 0) {
				str++;
				i++;
				X++;
			} else if (*str == '.' || *str == ',') {
				end = false;
				break;
			} else if (keyPressed == Console::keyEnter) {
				*str = '0';
				str++;
				*str = '0';
				str++;
				end = true;
				break;
			}
			while (*str != '.' && *str != ',' && !(*str >= '0' && *str <= '9') && keyPressed != Console::keyEscape && keyPressed != Console::keyEnter) {
				keyPressed = _getch();
				*str = keyPressed;
			}
			if (keyPressed != Console::keyEscape) {
				printf("%c", *str);
			} else {
				Null(str);
				end = true;
				break;
			}
		} else {
			keyPressed = _getch();
			*str = keyPressed;
			Console::GotoX(X);
		}
	}

	if (!end) {
		for (int i = 0; i < 2; i++) {
			keyPressed = 0;
			while (!(keyPressed >= '0' && keyPressed <= '9') && keyPressed != Console::keyEscape && keyPressed != Console::keyEnter) {
				keyPressed = _getch();
			}
			if (keyPressed != Console::keyEscape) {
				if (keyPressed == Console::keyEnter && i == 0) {
					i--;
				} else if ((keyPressed >= '0' && keyPressed <= '9') || keyPressed == Console::keyEnter) {
					if (keyPressed != Console::keyEnter) *str = keyPressed;
					printf("%c", *str);
				} else i--;
			} else {
				Null(str);
				break;
			}
			if (*str == '0') {
				X++;
			} else if (*str >= '1' && *str <= '9') {
				str++;
				X++;
			} else if (keyPressed == Console::keyEnter) {
				*str = '0';
				str++;
			}
		}
	}
	*str = 0;
}

void StringHelper::InputCellPhone(char* str, CellOper* OperList) {
	int length = 13;
	short X = Console::X();
	short Y = Console::Y();
	CellOper* List = OperList;
	Console::SetColor(Console::clBlack, Console::clYellow);
	Console::FillRect(X - 1, Y, X + length + 2, Y, Console::clYellow);
	Console::ShowCursor(true);
	int keyPressed = 0;

	char* s = str;
	int i = strlen(str);
	printf("%s", str);
	for (int j = 0; j < i; j++) {
		s++;
	}

	keyPressed = 0;
	do {
		if (i == 3) {
			bool correct = false;
			List = List->ListFirst();

			while (List) {
				if (!strcmp(str, List->code)) {
					correct = true;
					break;
				} else List = List->ListNext;
			}
			if (!correct) {
				Console::GotoXY(X, Y);
				Console::Print((char*)"   ", Console::clBlack, Console::clYellow);
				StringHelper::Null(str);
				i = 0;
				s = str;
				List = OperList;
			}
		}
		if (i == 3 || i == 7 || i == 10) {

			i++;
			*s = '-';
			printf("%c", *s);
			s++;

		}
		keyPressed = Console::GetKey();
		if (!key_is_func(keyPressed)) {
			*s = keyPressed;
			if (*s >= '0' && *s <= '9') {
				printf("%c", *s);
				s++;
				i++;
			}
		} else if (keyPressed == Console::keyBackspace) {
			if (strlen(str) > 0) {
				s--;
				i--;
				if (i == 3 || i == 7 || i == 10) {
					s--;
					i--;
					*s = 0;
					Console::GotoXY(Console::X() - 1, Y);
					printf("%c", *s);
					Console::GotoXY(Console::X() - 1, Y);
				}
				*s = 0;
				Console::GotoXY(Console::X() - 1, Y);
				printf("%c", *s);
				Console::GotoXY(Console::X() - 1, Y);
			}
		} 
		if (strlen(str) == length) break;
	} while (strlen(str) < length);

	*s = 0;

	Console::FillRect(X - 1, Y, X + length + 2, Y, Console::clLightGrey);
	Console::GotoXY(X, Y);
	Console::Print(str, Console::clBlack, Console::clLightGrey);
	Console::ShowCursor(false);
}

char* StringHelper::New(const int length) {
	char* LStr = (char*)malloc(length * sizeof(char));
	Null(LStr);
	return LStr;
}

void StringHelper::int_to_str(char* str, int num) {
	Null(str);
	int digits = 0;
	bool is_negative = false;
	if (num < 0) {
		is_negative = true;
		num = abs(num);
	}
	int temp = num;
	if (num != 0) {
		while (temp > 0) {
			digits++;
			temp /= 10;
		}
		if (is_negative) digits++;
		for (int i = digits - 1; i >= 0; i--) {
			str[i] = (num % 10) + 48;
			num /= 10;
		}
		if (is_negative) str[0] = '-';
	} else if (num == 0){
		str[0] = '0';
	}
}

void StringHelper::StrToSize(char* str, unsigned int new_size, int dir) {
	char* temp = New();
	strcpy_s(temp, DefaultSize, str);
	if (new_size > strlen(str)) {
		if (!dir) {
			while (new_size > strlen(str)) {
				strcat_s(str, DefaultSize, " ");
			}
		} else {
			strcpy_s(str, DefaultSize, "");
			while (strlen(str) < new_size - strlen(temp)) {
				strcat_s(str, DefaultSize, " ");
			}
			strcat_s(str, DefaultSize, temp);
		}
	}
	free(temp);
}

void StringHelper::int_to_currency(char* str, int currency) { 
	Null(str);

	char* beforedot = New(DefaultSize);
	char* afterdot = New(DefaultSize);
	char* temp = New(DefaultSize);
	int_to_str(temp, abs(currency));

	strcpy_s(beforedot, DefaultSize, temp);
	if (abs(currency) >= 100) beforedot[strlen(beforedot) - 2] = '\0';
	else strcpy_s(beforedot, DefaultSize, "0");

	strcpy_s(afterdot, DefaultSize, temp);
	if (abs(currency) >= 100) {
		while (strlen(afterdot) > 2) {
			for (unsigned int i = 0; i < strlen(afterdot); i++) {
				afterdot[i] = afterdot[i + 1];
			}
		}
	} else if (abs(currency) >= 0 && abs(currency) < 10) {
		afterdot[2] = '\0';
		afterdot[1] = afterdot[0];
		afterdot[0] = '0';
		
	}

	if (currency < 0) {
		strcpy_s(str, DefaultSize, "-");
		strcat_s(str, DefaultSize, beforedot);
	} else strcpy_s(str, DefaultSize, beforedot);
	strcat_s(str, DefaultSize, ".");
	strcat_s(str, DefaultSize, afterdot);

	free(beforedot);
	free(afterdot);
	free(temp);
}

bool StringHelper::key_is_func(int keyPressed) {
	int key = Console::keyBackspace;
	bool isFunc = false;
	while (key <= Console::keyDelete) {
		if (keyPressed > Console::keySpace && keyPressed < Console::keyF1) {
			key++;
			continue;
		}
		if (keyPressed == key) {
			isFunc = true;
			break;
		}
		key++;
	}
	return isFunc;
}