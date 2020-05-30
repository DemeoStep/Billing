#include <Windows.h>
#include <conio.h>
#include "Console.h"
#include "StringHelper.h"
#include <stdio.h>
#include <iostream>

HANDLE Console::GetHandle() {
	return GetStdHandle(STD_OUTPUT_HANDLE);
}

void Console::SetColor(const ConsoleColors Foreground, const ConsoleColors Background) {
	SetConsoleTextAttribute(GetHandle(), (WORD)(Background << 4) | Foreground);
}

void Console::FillRect(short LeftX, short LeftY, short RightX, short RightY, const ConsoleColors BgColor) {
	short LWidth = abs(LeftX - RightX) + 1;
	short LHeight = abs(LeftY - RightY) + 1;

	_CHAR_INFO* Buffer = (_CHAR_INFO*)calloc(LWidth * LHeight, sizeof(CHAR_INFO));

	WORD CharAttrib = BgColor << 4 | 0;
	for (int i = 0; i < (RightX - LeftX + 1) * (RightY - LeftY + 1); i++) {
		Buffer[i].Char.AsciiChar = ' ';
		Buffer[i].Attributes = CharAttrib;
	}
	COORD СharPos = { 0, 0 };
	SMALL_RECT writeArea;
	if (LeftY > RightY && LeftX > RightX)	writeArea = { RightX, RightY, LeftX, LeftY };
	else if (LeftX > RightX)				writeArea = { RightX, LeftY, LeftX, RightY };
	else if (LeftY > RightY)				writeArea = { LeftX, RightY, RightX, LeftY };
	else									writeArea = { LeftX, LeftY, RightX, RightY };
	COORD bufferSize = { LWidth, LHeight };
	WriteConsoleOutputA(Console::GetHandle(), Buffer, bufferSize, СharPos, &writeArea);

	free(Buffer);
}

void Console::GotoXY(const short x, const short y) {
	COORD LPos;
	LPos.X = x;
	LPos.Y = y;
	SetConsoleCursorPosition(GetHandle(), LPos);
}

void Console::GotoX(const short x) {
	GotoXY(x, Y());
}

void Console::GotoY(const short y) {
	GotoXY(X(), y);
}

short Console::X() {
	short LResult = 0;
	PCONSOLE_SCREEN_BUFFER_INFO ScrInfo = (PCONSOLE_SCREEN_BUFFER_INFO)malloc(1 * sizeof(struct _CONSOLE_SCREEN_BUFFER_INFO));
	if (TRUE == GetConsoleScreenBufferInfo(GetHandle(), ScrInfo) ){
		LResult = ScrInfo->dwCursorPosition.X;
	}
	free(ScrInfo);
	return LResult;
}

short Console::Y() {
	short LResult = 0;
	PCONSOLE_SCREEN_BUFFER_INFO ScrInfo = (PCONSOLE_SCREEN_BUFFER_INFO)malloc(sizeof(struct _CONSOLE_SCREEN_BUFFER_INFO));
	if (TRUE == GetConsoleScreenBufferInfo(GetHandle(), ScrInfo) ){
		LResult = ScrInfo->dwCursorPosition.Y;
	}
	free(ScrInfo);
	return LResult;
}

short Console::Height() {
	short LResult = 0;
	PCONSOLE_SCREEN_BUFFER_INFO ScrInfo = (PCONSOLE_SCREEN_BUFFER_INFO)malloc(sizeof(struct _CONSOLE_SCREEN_BUFFER_INFO));
	if (TRUE == GetConsoleScreenBufferInfo(GetHandle(), ScrInfo)) {
		LResult = ScrInfo->dwSize.Y;
	}
	free(ScrInfo);
	return LResult;
}

short Console::Width() {
	short LResult = 0;
	_CONSOLE_SCREEN_BUFFER_INFO *ScrInfo = (_CONSOLE_SCREEN_BUFFER_INFO *)malloc(sizeof(struct _CONSOLE_SCREEN_BUFFER_INFO));
	if (TRUE == GetConsoleScreenBufferInfo(GetHandle(), ScrInfo)) {
		LResult = ScrInfo->dwSize.X;
	}
	free(ScrInfo);
	return LResult;
}

void  Console::SetScreen(short AWidth, short AHeight) {
	PSMALL_RECT LConsRect = (PSMALL_RECT)malloc(sizeof(_SMALL_RECT));
	LConsRect->Left = 0;
	LConsRect->Top = 0;
	LConsRect->Right = AWidth-1;
	LConsRect->Bottom = AHeight-1;
	SetConsoleWindowInfo(GetHandle(), TRUE, LConsRect);
	free(LConsRect);

	COORD LConsoleSize;
	LConsoleSize.X = AWidth;
	LConsoleSize.Y = AHeight;
	SetConsoleScreenBufferSize(GetHandle(), LConsoleSize);

	// грязный хак
	DWORD LConsoleWindowStyle = 0;
	HWND LConsoleWindow = GetConsoleWindow();
	LConsoleWindowStyle = GetWindowLongA(LConsoleWindow, GWL_STYLE);
	LConsoleWindowStyle = LConsoleWindowStyle & 
		  ~(    WS_THICKFRAME   // изменять размер окна
			  //| WS_BORDER       // границы - рамки окна
		 	  | WS_MAXIMIZEBOX  // разеврнутьт во весь экран 
			  | WS_MINIMIZEBOX  // свернуть в панель задач
			  //| WS_SYSMENU      // кнопка "закрыть окно"
		  );


	SetWindowLongA(LConsoleWindow, GWL_STYLE, LConsoleWindowStyle);
}

void Console::ShowCursor(bool show) {
	CONSOLE_CURSOR_INFO CCI;
	CCI.bVisible = show;
	CCI.dwSize = 1;
	SetConsoleCursorInfo(GetHandle(), &CCI);
}

void Console::Print(char* str, const Console::ConsoleColors Color, const Console::ConsoleColors BgColor) {
	short LWidth = strlen(str);

	_CHAR_INFO* Buffer = (_CHAR_INFO*)calloc(LWidth, sizeof(CHAR_INFO));
	WORD CharAttrib = BgColor << 4 | Color;
	char* LineToPrint = StringHelper::New();

	strcpy_s(LineToPrint, StringHelper::DefaultSize, str);

	for (int i = 0; i < LWidth; i++) {
		Buffer[i].Char.AsciiChar = LineToPrint[i];
		Buffer[i].Attributes = CharAttrib;
	}
	COORD charPosition = { 0, 0 };

	SMALL_RECT writeArea = { Console::X(), Console::Y(), Console::X() + LWidth, Console::Y() };

	COORD bufferSize = { LWidth, 1 };
	WriteConsoleOutputA(Console::GetHandle(), Buffer, bufferSize, charPosition, &writeArea);

	free(LineToPrint);
	free(Buffer);
}

int Console::GetKey() {
	int keyPressed = _getch();

	if ((keyPressed == 0 && _kbhit()) || (keyPressed == 224 && _kbhit())) {
		keyPressed = _getch();
	}

	return keyPressed;
}