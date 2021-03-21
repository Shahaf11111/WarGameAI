#include "HpKits.h"

HpKits::HpKits(Maze* maze, int amount) {
	this->maze = maze;
	for (int i = 0; i < amount; i++) {
		int randX = rand() % MSZ;
		int randY = rand() % MSZ;
		while (maze->get(randX, randY) != SPACE) {
			randX = rand() % MSZ;
			randY = rand() % MSZ;
		}
		int randHp = rand() % 40 + 10;
		this->maze->set(randX, randY, HP_KIT);
		this->kits.push_back(new HpKit(randX, randY, randHp));
	}
}

void HpKits::drawMe() {
	for (auto kit : this->kits) {
		kit->drawMe();
	}
}

vector<HpKit*> & HpKits::getKits() {
	return this->kits;
}
