#pragma once
#include "Soldier.h"
#include "Maze.h"
#include "Kits.h"

class Team {

private:
	int teamColor;
	int enemyColor;
	Maze* maze;
	vector<Soldier*> soldiers;

public:
	Team(Maze* maze, int numSoldiers, int teamColor, int enemyColor);
	void initSoldiers(int numSoldiers);
	void drawMe();
	bool isAlive();
	void play(Team* enemyTeam, vector<Kit*>& allKits);
	vector<Soldier*> & getSoldiers();

};

