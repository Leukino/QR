#pragma once

#include <Windows.h>

bool getScreenCursorPosition(int& x, int& y)
{
	bool ret = true;
	POINT point{};
	ret = GetCursorPos(&point);
	x = point.x;
	y = point.y;
	return ret;
}

bool setScreenCursorPosition(int x, int y) //Don't use that in evil purposes ;)
{
	bool ret = true;
	SetCursorPos(x, y);
	return ret;

}
