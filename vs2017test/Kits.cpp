#include "Kits.h"

Kits::Kits(Maze* maze, int amount, int type) {
	this->maze = maze;
	double* color = this->type2color(type);
	for (int i = 0; i < amount; i++) {
		int randX = rand() % MSZ;
		int randY = rand() % MSZ;
		while (maze->get(randX, randY) != SPACE) {
			randX = rand() % MSZ;
			randY = rand() % MSZ;
		}
		this->maze->set(randX, randY, type);
		this->kits.push_back(new Kit(randX, randY, this->genRandAmount(type), type, color));
	}
}

int Kits::genRandAmount(int type) {
	switch (type) {
	case HP_KIT:
		return rand() % 41 + 10; // from 10 to 50
	case AMMO_KIT:
		return rand() % 8 + 3; // from 3 to 10
	case GRENADE_KIT:
		return rand() % 3 + 1; // from 1 to 3
	default:
		return 0;
	}
}

double* Kits::type2color(int type) {
	switch (type) {
	case HP_KIT:
		return new double[] { 0.0, 1.0, 0.0 };
	case AMMO_KIT:
		return new double[] { 0.5, 0.5, 0.5 };
	case GRENADE_KIT:
		return new double[] { 0.8, 0.1, 0.5 };
	default:
		return new double[] { 0.0, 0.0, 0.0 };
	}
}

void Kits::drawMe() {
	for (auto kit : this->kits) {
		kit->drawMe();
	}
}

vector<Kit*> & Kits::getKits() {
	return this->kits;
}
