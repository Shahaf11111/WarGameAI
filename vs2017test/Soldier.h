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
	int index;
	Maze* maze;
	int col;
	int row;
	int myColor;
	int enemyColor;
	int hp;
	int ammo;
	int grenadeAmount;
	int angle;
	int numTurnsToHide;
	Bullet* bullet;
	Grenade* grenade;
	Soldier* currentEnemy;
	Kit* currentKit;
	Node* currentHideout;
	stack<Node*> currentPath;
	bool isStabbing;

public:
	Soldier(Maze* maze, int col, int row, int team, int enemyTeam);

	// Main function of soldier:
	bool play(vector<Soldier*>& enemySoldiers, vector<Kit*>& kits);

	// Helpers of the main function:
	bool refill(vector<Kit*>& kits);
	bool fight(vector<Soldier*>& enemySoldiers, vector<Kit*>& kits);
	void run(vector<Kit*>& kits);

	// Debugging for the i'th soldier:
	void printMe(int i);

	// Movement - coloring and readjusting the maze cells:
	bool move(int col, int row);
	bool move(int col, int row, vector<Kit*>& kits);

	// Hiding and collaborating with team-mates:
	void hide();
	void findSafeHideout();
	void escape(vector<Kit*>& kits);

	// Enemy searching and fighting methods:
	bool needToFindNewEnemy();
	void findNearestEnemy(vector<Soldier*>& enemySoldiers);
	void lookForEnemy(vector<Soldier*>& enemySoldiers);
	bool moveTowardsEnemy(vector<Kit*>& kits);	
	// Stabbing enemy if he is a neighbor:
	bool canStabCurrentEnemy();
	void stabCurrentEnemy();
	// Shooting bullets:
	void shoot();
	void initFire(int destCol, int destRow);
	// Throwing grenades:
	void throwGrenade(vector<Soldier*>& enemySoldiers);
	void initGrenadeToss(int destCol, int destRow);

	// Refill of HP, ammo and grenades:
	bool refillKit(vector<Kit*>& kits, int kitType);
	bool moveAndCollect(vector<Kit*>& kits);
	bool isKitNeeded(vector<Kit*> kits);
	void collectKit(Node* kit, vector<Kit*>& kits, int kitType);

	// Helpers:
	void drawMe();
	bool isInsideTunnel();
	bool isDead();
	double distance(int col, int row);
	double* cell2coor(int col, int row);

	// Getters and Setters:
	void setAngle(int col, int row);
	void setAngle(int angle);
	int getHp();
	void setHp(int delta);
	int getAmmo();
	void setAmmo(int delta);
	int getGrenadeAmount();
	void setGrenadeAmount(int delta);
	int getCol();
	int getRow();

};

