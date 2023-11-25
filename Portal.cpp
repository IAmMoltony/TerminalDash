#include "Portal.h"

Portal::Portal()
{
	x = 0;
	y = 0;
	mode = GameMode::Cube;
}

Portal::Portal(int _x, int _y, GameMode _mode)
{
	x = _x;
	y = _y;
	mode = _mode;
}

void Portal::Draw(void)
{
	SetConsoleColor(GameModeToPortalColor(mode), ConsoleColor::Black);

	GotoXY(x, y);
	printf(")");
	GotoXY(x, y + 1);
	printf(")");
	GotoXY(x, y + 2);
	printf(")");

	SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);
}

GameMode Portal::GetMode(void)
{
	return mode;
}

bool Portal::Intersects(int _x, int _y)
{
	return _x == x && (_y == y || _y == y + 1 || _y == y + 2);
}

int Portal::GetX(void)
{
	return x;
}

int Portal::GetY(void)
{
	return y;
}

int Portal::GetGridX(int xOrigin)
{
	return x - xOrigin;
}

int Portal::GetGridY(int yOrigin)
{
	return abs(y - yOrigin);
}

ConsoleColor GameModeToPortalColor(GameMode mode)
{
	switch (mode)
	{
	default:
	case GameMode::Cube:
		return ConsoleColor::LightGreen;
	case GameMode::Ship:
		return ConsoleColor::LightMagenta;
	}
	return ConsoleColor::LightGreen;
}

GameMode StringToGameMode(std::string s)
{
	std::string sl = s;
	std::transform(sl.begin(), sl.end(), sl.begin(), [](uint8_t ch) {
		return std::tolower(ch);
	});

	if (sl == "cube")
	{
		return GameMode::Cube;
	}
	else if (sl == "ship")
	{
		return GameMode::Ship;
	}

	return GameMode::Cube;
}
