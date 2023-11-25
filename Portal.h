#pragma once
#include "Player.h"
#include "Console.h"
#include <string>
#include <algorithm>
#include <ctype.h>

ConsoleColor GameModeToPortalColor(GameMode mode);
GameMode StringToGameMode(std::string s);

class Portal
{
private:
	int x, y;
	GameMode mode;
public:
	Portal();
	Portal(int _x, int _y, GameMode _mode);

	void Draw(void);

	GameMode GetMode(void);
	bool Intersects(int _x, int _y);
	int GetX(void);
	int GetY(void);
	int GetGridX(int xOrigin);
	int GetGridY(int yOrigin);
};
