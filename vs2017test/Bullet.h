#pragma once
#include "room.h"
#include "Maze.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "glut.h"

const double SPEED = 0.01;

class Bullet
{
private:
	double initialX, initialY;
	double x, y;
	double dirx, diry;
	bool isMoving;
	int targetColor;
	int damage;
	int angle;
	bool hit;

public:
	Bullet(int startCol, int startRow, int targetCol, int targetRow, int targetColor);
	Bullet(double startX, double startY, double directionX, double directionY, int targetColor);
	void DrawMe();
	void Fire(bool f) { isMoving = f; }
	bool IsMoving() { return isMoving; }
	bool Move(Maze* maze);
	double GetX() { return x; }
	double GetY() { return y; }
	int* updateSecurityMap(int maze[MSZ][MSZ]);
	int* coor2cell(double x, double y);
	double* cell2coor(int col, int row);
	int calcDamage();
	void setDamage(double destX, double destY);
	int getDamage() { return this->damage == -1 ? this->calcDamage() : this->damage; };
	int getAngle() { return this->angle; };
	bool didHit();
};

