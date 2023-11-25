#pragma once
#include <stdio.h>
#include "Console.h"

class Spike
{
private:
	int x, y;
public:
	Spike();
	Spike(int _x, int _y);

	void Draw(int camera);

	int GetX(void);
	int GetY(void);
	int GetGridX(int xOrigin);
	int GetGridY(int yOrigin);
};
