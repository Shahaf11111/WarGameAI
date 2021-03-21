#include "Team.h"

Team::Team(Maze* maze, int numSoldiers, int teamColor, int enemyColor) {
	this->maze = maze;
	this->teamColor = teamColor;
	this->enemyColor = enemyColor;
	this->initSoldiers(numSoldiers);
}

void Team::initSoldiers(int amount) {
	for (int i = 0; i < amount; i++) {
		int randX = rand() % MSZ;
		int randY = rand() % MSZ;
		while (maze->get(randX, randY) != SPACE) {
			randX = rand() % MSZ;
			randY = rand() % MSZ;
		}
		this->maze->set(randX, randY, this->teamColor);
		this->soldiers.push_back(new Soldier(this->maze, randX, randY, this->teamColor, this->enemyColor));
	}
}

void Team::drawMe() {
	for (auto soldier : this->soldiers) {
		soldier->drawMe();
	}
}

bool Team::isAlive() {
	return this->soldiers.size();
}


vector<Soldier*> & Team::getSoldiers() {
	return this->soldiers;
}

void Team::play(Team* enemyTeam, vector<Kit*>& allKits) {
	vector<Soldier*> deadSoldiers;
	for (auto soldier : this->soldiers) {
		if (!soldier->play(enemyTeam->getSoldiers(), allKits)) {
			deadSoldiers.push_back(soldier);
		}
	}
	for (auto soldier : deadSoldiers) {
		this->soldiers.erase(
			remove(this->soldiers.begin(), this->soldiers.end(), soldier), 
			this->soldiers.end());
	}
}
