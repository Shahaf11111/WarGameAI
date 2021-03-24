#pragma once
#include <iostream>
#include "Constants.h"
#include <vector>

using namespace std;


class Room {

private:
	int width, height, centerX, centerY;
	int* leftBottomCorner;
	int* rightUpperCorner;
	vector<int*> safeCells;

public:
	Room();
	void DrawMe();
	void SetWidth(int w) { width = w; };
	void SetHeigth(int h) { height = h; };
	void SetCenterX(int x) { centerX = x; };
	void SetCenterY(int y) { centerY = y; };
	void FillMaze(int maze[MSZ][MSZ]);
	void initSafeCells(int securityMap[MSZ][MSZ]);
	int* getSafeCell();
	int getAmountOf(int maze[MSZ][MSZ], int colToExclude, int rowToExclude, int color);
	bool IsOverlap(int w, int h, int row, int col);
	bool contains(int col, int row);
	int GetCenterRow() { return centerY; };
	int GetCenterCol() { return centerX; };
	int getHeight();
	int getWidth();
	int* getLeftBottomCorner() { return this->leftBottomCorner; };
	int* getRightUpperCorner() { return this->rightUpperCorner; };

};

