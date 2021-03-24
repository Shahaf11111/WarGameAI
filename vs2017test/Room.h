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
	void drawMe();
	void fillMaze(int maze[MSZ][MSZ]);
	
	// Security map handlers and room utilities:
	void initSafeCells(int securityMap[MSZ][MSZ]);
	int* getSafeCell();
	int getAmountOf(int maze[MSZ][MSZ], int colToExclude, int rowToExclude, int color);
	bool isOverlapping(int w, int h, int row, int col);
	bool contains(int col, int row);
	
	
	//Getters & Setters:
	int getCenterX() { return centerY; };
	int getCenterY() { return centerX; };
	int getHeight();
	int getWidth();
	void setWidth(int w) { width = w; };
	void setHeight(int h) { height = h; };
	void setCenterX(int x) { centerX = x; };
	void setCenterY(int y) { centerY = y; };
	int* getLeftBottomCorner() { return this->leftBottomCorner; };
	int* getRightUpperCorner() { return this->rightUpperCorner; };

};

