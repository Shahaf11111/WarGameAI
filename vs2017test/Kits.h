#pragma once
#include "Constants.h"
#include "Kit.h"
#include "Maze.h"
#include <vector>

using namespace std;

class Kits {

private:
	Maze* maze;
	vector<Kit*> kits;
	int amount;
	int color;

public:
	Kits(Maze* maze, int amount, int type);
	double* type2color(int type);
	int genRandAmount(int type);
	vector<Kit*> & getKits();
	void drawMe();

};

