#ifndef STRINGHELPER_H
#define STRINGHELPER_H
#include <string>

class Abonent;

class StringHelper {
public:
	static const int DefaultSize = 255;
	static const int DefaultSizeSmall = 20;
	static const short AbonCardWidth = 57;

	static void Null(char *str, const int length = DefaultSize);
	static void Input(char* str, const int length = DefaultSize);
	static void InputDigit(char* str, const int length);
	static void InputEng(char* str, const int length);
	static void InputRus(char* str, const int length);
	static void Input_currency(char* str, const int length = DefaultSize);
	static char* New(const int length = DefaultSize);
	static void int_to_str(char* str, int num);
	static void StrToSize(char* str, unsigned int new_size, int dir = 0);
	static void int_to_currency(char* str, int currency);
};

#endif