#pragma once
#include "Constants.h"
#include "HpKit.h"
#include "Maze.h"
#include <vector>

using namespace std;

class HpKits {

private:
	Maze* maze;
	vector<HpKit*> kits;
	int hp;

public:
	HpKits(Maze* maze, int amount);
	vector<HpKit*> & getKits();
	void drawMe();

};

