#include "Spike.h"

Spike::Spike()
{
	x = 0;
	y = 0;
}

Spike::Spike(int _x, int _y)
{
	x = _x;
	y = _y;
}

void Spike::Draw(int camera)
{
	if (x - camera < 0 || y > 19 || x - camera > GetScreenSize().width - 2)
	{
		return;
	}
	GotoXY(x - camera, y);
	printf("%c", 30); // draw the spike (triangle)
}

int Spike::GetX(void)
{
	return x;
}

int Spike::GetY(void)
{
	return y;
}

int Spike::GetGridX(int xOrigin)
{
	return x - xOrigin;
}

int Spike::GetGridY(int yOrigin)
{
	return abs(y - yOrigin);
}
