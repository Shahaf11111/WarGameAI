#pragma once
#include "Bullet.h"
#include "Maze.h"

const int NUM_BULLETS = 20;

class Grenade {

private:
	vector<Bullet*> bullets;
	double x, y;
	double dirx, diry;
	double cx, cy;
	bool isFlying;
	bool isExploded;
	int targetColor;
	int friendlyColor;
	int angle;
	int drawAngle;

public:
	Grenade(int startCol, int startRow, int targetCol, int targetRow, int targetColor, int friendlyColor);
	bool go(Maze* maze);
	void fly(bool f) { this->isFlying = f; }
	bool isMoving() { return this->isFlying; }
	bool move(Maze* maze);
	void Explode();
	bool exploded() { return isExploded; }
	void Exploding(Maze* maze);
	void DrawMe();
	int* coor2cell(double x, double y);
	double* cell2coor(int col, int row);
	int getAngle() { return this->angle; };
	vector<Bullet*> &getBullets() { return this->bullets;  }
	vector<int*> updateSecurityMap(int maze[MSZ][MSZ]);
};
