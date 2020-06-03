#ifndef CONSOLE_H
#define CONSOLE_H

#include <Windows.h>

class Console {
public:
	static const short DefaultScreenWidth  = 170;
	static const short DefaultScreenHeight = 40;
	static enum ConsoleColors {        // LRGB LRGB    Light Red Blue Green
		  clBlack = 0			// 0000 0000 
		, clBlue = 1			// 0000 0001 
		, clGreen = 2			// 0000 0010 
		, clCyan = 3			// 0000 0011
		, clRed = 4				// 0000 0100 
		, clMagenta = 5			// 0000 0101 
		, clBrown = 6			// 0000 0110 
		, clLightGrey = 7		// 0000 0111 
		, clDarkGrey = 8		// 0000 1000 
		, clLightBlue = 9		// 0000 1001 
		, clLightGreen = 10		// 0000 1010 
		, clLightCyan = 11		// 0000 1011 
		, clLightRed = 12		// 0000 1100 
		, clLightMagenta = 13   // 0000 1101 
		, clYellow = 14			// 0000 1110 
		, clWhite = 15			// 0000 1111 
	};
	static const ConsoleColors DefaultColor = clWhite;
	static const ConsoleColors DefaultBgColor = clBlack;

	static enum FuncKeys {
		keyBackspace = 8
		, keyTab = 9
		, keyEnter = 13
		, keyEscape = 27
		, keySpace = 32
		, keyF1 = 59
		, keyF2 = 60
		, keyF3 = 61
		, keyF4 = 62
		, keyF5 = 63
		, keyF6 = 64
		, keyF7 = 65
		, keyF8 = 66
		, keyF9 = 67
		, keyF10 = 68
		, keyF11 = 133
		, keyF12 = 134
		, keyHome = 71
		, keyUp = 72
		, keyPageUp = 73
		, keyLeft = 75
		, keyCenter = 76
		, keyRight = 77
		, keyEnd = 79
		, keyDown = 80
		, keyPageDown = 81
		, keyInsert = 82
		, keyDelete = 83
	};

	static HANDLE GetHandle();
	static void SetColor(const ConsoleColors Foreground, const ConsoleColors Background = clBlack);
	static void FillRect(short LeftX, short LeftY, short RightX, short RightY, const ConsoleColors BgColor);
	static void GotoXY(const short x, const short y);
	static void GotoX(const short x);
	static void GotoY(const short y);
	static short X();
	static short Y();
	static short Width();
	static short Height();
	static void SetScreen(short AWidth = DefaultScreenWidth, short AHeight = DefaultScreenHeight);
	static void ShowCursor(bool show);
	static void Print(char* str, const Console::ConsoleColors Color, const Console::ConsoleColors BgColor);
	static int GetKey();
	static bool ShowWarning(char* warning, short CursorPos);
};

#endif