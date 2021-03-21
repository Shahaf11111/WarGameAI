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
	int securityMap[MSZ][MSZ] = { 0 };
	Room rooms[NUM_ROOMS];
	AStar aStar;

public:
	Maze();
	void InitMaze();
	void InitRooms();
	void runAStar(int c, int r, int tc, int tr);
	void DigTunnel(int index1, int index2);
	void DigTunnels();
	void DrawMe();
	bool isInsideTunnel(int col, int row);
	bool isSafe(int col, int row);
	void initSecurityMap();
	void GenerateSecurityMap();
	int get(int col, int row);
	void set(int col, int row, int color);
	stack<Node*> getPath();

};

