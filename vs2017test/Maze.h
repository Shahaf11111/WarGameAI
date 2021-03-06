#pragma once
#include "Constants.h"
#include "Room.h"
#include "glut.h"
#include "AStar.h"
#include <queue>
#include <vector>

using namespace std;

class Maze {

private:
	int maze[MSZ][MSZ];
	Room rooms[NUM_ROOMS];
	AStar aStar;

public:
	Maze();
	void initMaze();
	void initRooms();
	void runAStar(int c, int r, int tc, int tr, int friendColor, int enemyColor);
	void digTunnel(int index1, int index2);
	void digTunnels();
	void drawMe();
	bool isInsideTunnel(int col, int row);
	bool isSafe(int col, int row);
	void initSecurityMap();
	void initSafeCells(int securityMap[MSZ][MSZ]);
	int* getSafeCellWith(int colToExclude, int rowToExclude, int color);
	int get(int col, int row);
	void set(int col, int row, int color);
	stack<Node*> getPath();
	int getAmountOfColorInRoom(int col, int row, int color);
	Room getRoomAt(int index);
};

