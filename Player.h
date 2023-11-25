#pragma once
#include <vector>
#include <stdio.h>
#include "Console.h"
#include "Spike.h"
#include "Block.h"

enum class GameMode
{
	Cube,
	Ship
};

class Player
{
private:
	int x, y, startX, startY, attempt;
	float velY;
	bool falling, jumping, shipShouldFall, invincible, levelComplete;
	GameMode mode;
public:	
	Player();
	Player(int _x, int _y);

	void Draw(void);
	void Update(std::vector<Block> blocks, int *camera, int finish);
	void Jump(void);
	void Reset(void);
	void ToggleInvincibility(void);
	void GoToShip(void);
	void GoToCube(void);
	void Die(int *camera);
	void SetPosition(int _x, int _y);
	void SetInvincibility(bool _i);
	void SetLevelComplete(bool _c);

	int GetX(void);
	int GetY(void);
	int GetStartX(void);
	int GetStartY(void);
	int GetAttempt(void);

	bool IsGameMode(GameMode _mode);
	bool IsLevelComplete(void);
};
