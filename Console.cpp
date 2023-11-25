#include "Console.h"

void GotoXY(short x, short y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });
}

void ClearChar(short x, short y)
{
    GotoXY(x, y);
    printf(" ");
}

void ClearScreen(void)
{
	system("cls");
}

void HideCursor(void)
{
    // https://superuser.com/questions/1496322/how-do-i-remove-hide-the-cursor-the-blinking-underscore-character-in-cmd-exe#:~:text=Use%200%20to%20hide%20the,in%20CMD%20on%20Windows%2010.
    HANDLE hStdOut = NULL;
    CONSOLE_CURSOR_INFO curInfo;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &curInfo);
}

void DisableConsoleSelection(void)
{
    // https://stackoverflow.com/questions/46567248/how-to-disable-user-selection-in-windows-console
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prev_mode;
    GetConsoleMode(hInput, &prev_mode); 
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));
}

void SetConsoleColor(ConsoleColor fg, ConsoleColor bg)
{
    int bgi = ConsoleColorToInt(bg);
    int fgi = ConsoleColorToInt(fg);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((bgi << 4) | fgi));
}

int ConsoleColorToInt(ConsoleColor color)
{
    switch (color)
    {
    case ConsoleColor::Black:
        return 0;
    case ConsoleColor::Blue:
        return 1;
    case ConsoleColor::Green:
        return 2;
    case ConsoleColor::Cyan:
        return 3;
    case ConsoleColor::Red:
        return 4;
    case ConsoleColor::Magenta:
        return 5;
    case ConsoleColor::Brown:
        return 6;
    case ConsoleColor::LightGray:
        return 7;
    case ConsoleColor::DarkGray:
        return 8;
    case ConsoleColor::LightBlue:
        return 9;
    case ConsoleColor::LightGreen:
        return 10;
    case ConsoleColor::LightCyan:
        return 11;
    case ConsoleColor::LightRed:
        return 12;
    case ConsoleColor::LightMagenta:
        return 13;
    case ConsoleColor::Yellow:
        return 14;
    case ConsoleColor::White:
        return 15;
    default:
        return 0;
    }
}

screensize_t GetScreenSize(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int w, h;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return {w, h};
}
