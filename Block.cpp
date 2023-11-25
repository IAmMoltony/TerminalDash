#include "Block.h"

Block::Block()
{
	x = 0;
	y = 0;
	slab = false;
}

Block::Block(int _x, int _y, bool _slab)
{
	x = _x;
	y = _y;
	slab = _slab;
}

void Block::Draw(int camera)
{
	if (x - camera < 0 || y > 19 || x - camera > GetScreenSize().width - 2)
	{
		return;
	}
	GotoXY(x - camera, y);
	if (!slab)
	{
		printf("#");
	}
	else
	{
		printf("%c", 22);
	}
}

int Block::GetX(void)
{
	return x;
}

int Block::GetY(void)
{
	return y;
}

int Block::GetGridX(int xOrigin)
{
	return x - xOrigin;
}

int Block::GetGridY(int yOrigin)
{
	return abs(y - yOrigin);
}

bool Block::IsSlab(void)
{
	return slab;
}
