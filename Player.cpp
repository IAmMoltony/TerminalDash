#include "Player.h"

Player::Player()
{
	x = 0;
	y = 0;
	velY = 0;
	startX = x;
	startY = y;
	attempt = 1;
	falling = jumping = shipShouldFall = invincible = levelComplete = false;
	mode = GameMode::Cube;
}

Player::Player(int _x, int _y)
{
	x = _x;
	y = _y;
	velY = 0;
	startX = x;
	startY = y;
	attempt = 1;
	falling = jumping = shipShouldFall = invincible = levelComplete = false;
	mode = GameMode::Cube;
}

void Player::Draw(void)
{
	GotoXY(startX, y);
	SetConsoleColor(ConsoleColor::Cyan, ConsoleColor::Black);
	printf("%c", 2);
	if (mode == GameMode::Ship)
	{
		GotoXY(startX, y + 1);
		printf("%c", 22);
		GotoXY(startX - 1, y + 1);
		printf("%c", 17);
	}
	SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);
}

void Player::GoToCube(void)
{
	mode = GameMode::Cube;
}

void Player::Die(int *camera)
{
	if (invincible)
		return;
	GotoXY(startX, y);
	SetConsoleColor(ConsoleColor::LightRed, ConsoleColor::Black);
	printf("%c", 1);
	*camera = 0;
	x = startX;
	y = startY;
	velY = 0;
	attempt++;
	SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);
	PlaySound(L"sounds/explode.wav", NULL, SND_FILENAME | SND_ASYNC);
	Sleep(800);
}

void Player::SetPosition(int _x, int _y)
{
	x = _x;
	y = _y;
}

void Player::Update(std::vector<Block> blocks, int *camera, int finish)
{
	if (!levelComplete)
	{
		int oldX = x;
		int oldY = y;

		x++;
		y += (int)floor(velY);

		if ((falling || jumping) && mode != GameMode::Ship)
		{
			velY += 0.5;
		}

		if (mode == GameMode::Ship)
		{
			if (shipShouldFall)
			{
				velY = 1;
			}
		}

		if (y > 19)
		{
			y = 19;
			velY = 0;
			falling = jumping = false;
		}

		for (auto block : blocks)
		{
			if (block.GetX() == oldX + 1 && block.GetY() == y)
			{
				if (block.GetY() == oldY + 1)
				{
					falling = jumping = false;
					velY = 0;
					y = block.GetY() - 1;
				}
				else
				{
					Die(camera);
				}
			}

			if (block.GetY() == y + 1 && block.GetX() == x)
			{
				falling = jumping = false;
				velY = 0;
				y = block.GetY() - 1;
			}
			else
			{
				falling = true;
			}

			if (block.GetX() == x && block.GetY() == oldY - 1)
			{
				if (mode == GameMode::Cube)
				{
					Die(camera);
					return;
				}
				velY = 0;
				y = block.GetY() + 1;
			}
		}

		if (x == finish)
		{
			levelComplete = true;
			PlaySound(L"sounds/levelComplete.wav", NULL, SND_FILENAME | SND_ASYNC);
		}

		shipShouldFall = true;
	}
}

void Player::Jump(void)
{
	if (mode == GameMode::Ship)
	{
		velY = -1;
		shipShouldFall = false;
		return;
	}
	if (jumping)
	{
		return;
	}
	jumping = true;
	velY = -1;
}

void Player::Reset(void)
{
	x = 0;
	y = 0;
	velY = 0;
	attempt++;
	levelComplete = false;
}

void Player::ToggleInvincibility(void)
{
	invincible = !invincible;
}

void Player::GoToShip(void)
{
	mode = GameMode::Ship;
}

void Player::SetInvincibility(bool _i)
{
	invincible = _i;
}

void Player::SetLevelComplete(bool _c)
{
	levelComplete = _c;
}

int Player::GetX(void)
{
	return x;
}

int Player::GetY(void)
{
	return y;
}

int Player::GetStartX(void)
{
	return startX;
}

int Player::GetStartY(void)
{
	return startY;
}

int Player::GetAttempt(void)
{
	return attempt;
}

bool Player::IsGameMode(GameMode _mode)
{
	return mode == _mode;
}

bool Player::IsLevelComplete(void)
{
	return levelComplete;
}
