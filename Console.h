#pragma once
#include <stdio.h>
#include <direct.h>
#include <Windows.h>

typedef struct screensize
{
	int width, height;
} screensize_t;

enum class ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

void GotoXY(short x, short y);
void ClearChar(short x, short y);
void ClearScreen(void);
void HideCursor(void);
void DisableConsoleSelection(void);
void SetConsoleColor(ConsoleColor fg, ConsoleColor bg);
int ConsoleColorToInt(ConsoleColor color);
screensize_t GetScreenSize(void);