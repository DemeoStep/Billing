#include <iostream>
#include "StringHelper.h"
#include <string>
#include "Abonent.h"
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

void StringHelper::InputDigit(char* str, const int length) {
	short X = Console::X();
	short Y = Console::Y();
	Console::SetColor(Console::clBlack, Console::clYellow);
	Console::FillRect(X - 1, Y, X + length, Y, Console::clYellow);
	Null(str);
	char* s = str;
	while (!(*s >= '0' && *s <= '9') && *s != '\r') {
		*s = _getch();
		if (*s == '\r') {
			*s = '0';
			*(s++) = '\r';
			break;
		}
		if ((*s >= '0' && *s <= '9')) {
			printf("%c", *s);
		}
	}
	int i = 0;
	while (i < length && *s != '\r' && (*s >= '0' && *s <= '9')) {
		s++;
		i++;
		if (i < length) {
			*s = _getch();
			if ((*s >= '0' && *s <= '9')) {
				printf("%c", *s);
			} else if (*s != '\r') {
				s--;
			}
		} else break;
	}
	*s = 0;

	Console::FillRect(X - 1, Y, X + length, Y, Console::clLightGrey);
	Console::GotoXY(X, Y);
	Console::Print(str, Console::clBlack, Console::clLightGrey);
}

void StringHelper::InputEng(char* str, const int length) {
	short X = Console::X();
	short Y = Console::Y();
	Console::SetColor(Console::clBlack, Console::clYellow);
	Console::FillRect(X - 1, Y, X + length, Y, Console::clYellow);
	Null(str);
	char* s = str;
	while (!(*s >= 'A' && *s <= 'Z') && !(*s >= 'a' && *s <= 'z') && !(*s >= '0' && *s <= '9') && *s != '\r') {
		*s = _getch();
		if (*s == '\r') {
			*s = '0';
			*(s++) = '\r';
			break;
		}
		if ((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || (*s >= '0' && *s <= '9')) {
			printf("%c", *s);
		}
	}
	int i = 0;
	while (i < length && *s != '\r' && ((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || (*s >= '0' && *s <= '9'))) {
		s++;
		i++;
		if (i < length) {
			*s = _getch();
			if ((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || (*s >= '0' && *s <= '9')) {
				printf("%c", *s);
			} else if (*s != '\r') {
				s--;
			}
		} else break;
	}
	*s = 0;

	Console::FillRect(X - 1, Y, X + length, Y, Console::clLightGrey);
	Console::GotoXY(X, Y);
}

void StringHelper::InputRus(char* str, const int length) {
	short X = Console::X();
	short Y = Console::Y();
	Console::SetColor(Console::clBlack, Console::clYellow);
	Console::FillRect(X - 1, Y, X + length, Y, Console::clYellow);

	Null(str);
	char* s = str;
	while (!(*s >= '�' && *s <= '�') && !(*s >= '0' && *s <= '9') && *s != '\r') {
		*s = _getch();
		if (*s == '\r') {
			*s = '0';
			*(s++) = '\r';
			break;
		}
		if ((*s >= '�' && *s <= '�') || (*s >= '0' && *s <= '9')) {
			printf("%c", *s);
		}
	}
	int i = 0;
	while (i < length && *s != '\r' && ((*s >= '�' && *s <= '�') || (*s >= '0' && *s <= '9') || *s == ' ')) {
		s++;
		i++;
		if (i < length) {
			*s = _getch();
			if ((*s >= '�' && *s <= '�') || (*s >= '0' && *s <= '9') || *s == ' ') {
				printf("%c", *s);
			} else if (*s != '\r') {
				s--;
			}
		} else break;
	}
	*s = 0;

	Console::FillRect(X - 1, Y, X + length, Y, Console::clLightGrey);
	Console::GotoXY(X, Y);
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

char* StringHelper::New(const int length) {
	char* LStr = (char*)calloc(length, sizeof(char));
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