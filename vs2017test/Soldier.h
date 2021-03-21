#pragma once
#include "glut.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Constants.h"
#include "Maze.h"
#include "Kit.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

class Soldier : public Node {

private:
	Maze* maze;
	int col;
	int row;
	int myColor;
	int enemyColor;
	int hp;
	int ammo;
	int grenades;
	int angle;
	int numTurnsToHide;
	Bullet* bullet;
	Grenade* grenade;
	Soldier* currentEnemy;
	Kit* currentKit;
	Node* currentHideout;
	stack<Node*> currentPath;

public:
	Soldier(Maze* maze, int col, int row, int team, int enemyTeam);

	bool play(vector<Soldier*>& enemySoldiers, vector<Kit*>& kits);

	bool move(int col, int row);
	bool move(int col, int row, vector<Kit*>& kits);

	void hide();
	void findSafeHideout();
	void escape(vector<Kit*>& kits);

	bool needToFindNewEnemy();
	void findNearestEnemy(vector<Soldier*>& enemySoldiers);
	void lookForEnemy(vector<Soldier*>& enemySoldiers);
	bool moveTowardsEnemy(vector<Kit*>& kits);
	bool isEnemyInMyRoom();
	bool isInsideTunnel();
	int* getRoomEdge(char direction);
	
	void shoot();
	void initFire(int destCol, int destRow);

	void throwGrenade(vector<Soldier*>& enemySoldiers);
	void initGrenadeToss(int destCol, int destRow);

	bool refill(vector<Kit*>& kits, int kitType);
	bool moveAndCollect(vector<Kit*>& kits);
	bool isKitNeeded(vector<Kit*> kits);
	void collectKit(Node* kit, vector<Kit*>& kits, int kitType);

	void setAngle(int col, int row);
	void setAngle(int angle);
	void drawMe();
	void drawRectangle(double* color, double x, double y, 
		double width, double height, double rotateX, double rotateY);
	double distance(int col, int row);
	bool isDead();

	int getHp();
	void setHp(int delta);
	int getAmmo();
	void setAmmo(int delta);
	int getGrenades();
	void setGrenades(int delta);
	int getCol();
	int getRow();

	double* cell2coor(int col, int row);

};

