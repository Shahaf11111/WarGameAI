#pragma once
#include "Bullet.h"
#include "Maze.h"
#include <set>
#include <math.h>

const int NUM_BULLETS = 20;

struct CellPosComparator {
	bool operator() (const int* cell1, const int* cell2) const {
		//cout << cell1[0] << "," << cell1[1] << "||"<< cell2[0] << "," << cell2[1] << endl;
		if (cell1[0] == cell2[0]) {
			return cell1[1] < cell2[1];
		}
		return cell1[0] < cell2[0];
		//return cell1[0] == cell2[0] ? true : cell1[1] < cell2[1];
	}
};

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
	void go(Maze* maze);
	bool isMoving() { return this->isFlying; }
	void move(Maze* maze);
	void Explode();
	bool exploded() { return isExploded; }
	void Exploding(Maze* maze);
	void DrawMe();
	int* coor2cell(double x, double y);
	double* cell2coor(int col, int row);
	int getAngle() { return this->angle; };
	vector<Bullet*> &getBullets() { return this->bullets;  }
	set<int*, CellPosComparator> updateSecurityMap(int maze[MSZ][MSZ]);
};
