#pragma once
#include <stdio.h>
#include "Console.h"

class Block
{
private:
	int x, y;
	bool slab;
public:
	Block();
	Block(int _x, int _y, bool _slab = false);

	void Draw(int camera);

	int GetX(void);
	int GetY(void);
	int GetGridX(int xOrigin);
	int GetGridY(int yOrigin);
	bool IsSlab(void);
};
