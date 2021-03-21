#pragma once
#include <iostream>
#include "Constants.h"

class Room
{
private:
	int width, height, centerX, centerY;
public:
	Room();
	void DrawMe();
	void SetWidth(int w) { width = w; };
	void SetHeigth(int h) { height = h; };
	void SetCenterX(int x) { centerX = x; };
	void SetCenterY(int y) { centerY = y; };
	void FillMaze(int maze[MSZ][MSZ]);
	bool IsOverlap(int w, int h, int row, int col);
	int GetCenterRow() { return centerY; };
	int GetCenterCol() { return centerX; };
	int getHeight();
	int getWidth();

};

