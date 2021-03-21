#include "Soldier.h"

using namespace std;

Soldier::Soldier(Maze* maze, int col, int row, int myColor, int enemyColor) {
	this->maze = maze;
	this->col = col;
	this->row = row;
	this->myColor = myColor;
	this->enemyColor = enemyColor;
	this->ammo = rand() % MAX_AMMO + 1;
	this->grenades = rand() % MAX_GRENADES + 1;
	this->hp = rand() % MAX_HP + 1;

	this->angle = 45;
	this->bullet = nullptr;
	this->grenade = nullptr;
	this->currentKit = nullptr;
	this->currentEnemy = nullptr;
	this->currentHpKit = nullptr;
	this->currentAmmoKit = nullptr;
}

bool Soldier::play(vector<Soldier*>& enemySoldiers, vector<Kit*>& kits) {
	if (this->isDead()) {
		return false;
	}
	if (this->myColor == TEAM_BLUE)
	//cout << "HP:" << this->hp << ", AMMO:" << this->ammo << ", GRENADES:" << this->grenades << endl;
	//int strategy = rand() % 100;
	if (this->hp <= LOW_HP_THRESHOLD) {
		if (this->refill(kits, HP_KIT)) {
			return true;
		}
	}
	if (this->ammo <= LOW_AMMO_THRESHOLD) {
		if (this->refill(kits, AMMO_KIT)) {
			return true;
		}
	}
	if (this->grenades <= LOW_GRENADE_THRESHOLD) {
		if (this->refill(kits, GRENADE_KIT)) {
			return true;
		}
	}
	this->lookForEnemy(enemySoldiers);
	if (this->currentEnemy != nullptr) {
		if (this->bullet != nullptr) {
			this->shoot();
			return true;
		}
		if (this->grenade != nullptr) {
			this->throwGrenade(enemySoldiers);
			return true;
		}
		if (this->moveTowardsEnemy(kits)) {
			return true;
		}
	}
	this->hide();
	if (this->currentHideout != nullptr) {
		this->escape(kits);
	}
	return true;
}

/* Hide functions: */

void Soldier::hide() {
	if (this->currentHideout == nullptr) {
		this->findSafestHideout();
	}
	this->maze->runAStar(this->getCol(), this->getRow(),
		this->currentHideout->getCol(), this->currentHideout->getRow());
	stack<Node*> path = this->maze->getPath();
	swap(this->currentPath, path);
}

void Soldier::findSafestHideout() {
	int hideoutCol, hideoutRow;
	while (true) {
		hideoutCol = rand() % MSZ;
		hideoutRow = rand() % MSZ;
		double distance = this->distance(hideoutCol, hideoutRow);
		if (distance > 20 && this->maze->get(hideoutCol, hideoutRow) == SPACE &&
			this->maze->isSafe(hideoutCol, hideoutRow)) {
			break;
		}
	}
	this->currentHideout = new Node(hideoutCol, hideoutRow);
}

void Soldier::escape(vector<Kit*>& kits) {
	if (!this->moveAndCollect(kits)) {
		this->currentHideout = nullptr;
	} else if (!this->isInsideTunnel()) {
		this->currentHideout = nullptr;
		stack<Node*> path;
		swap(this->currentPath, path);
	}
}

/* Kit Functions: */

void Soldier::collectKit(Node* nextNode, vector<Kit*>& kits, int kitType) {
	bool removeKit = false;
	for (auto kit : kits) {
		if (kit->getCol() == nextNode->getCol() && kit->getRow() == nextNode->getRow()) {
			if (kitType == HP_KIT) {
				this->setHp(kit->getAmount());
				removeKit = true;
			} else if (kitType == AMMO_KIT) {
				this->setAmmo(kit->getAmount());
				removeKit = true;
			} else if (kitType == GRENADE_KIT) {
				this->setGrenades(kit->getAmount());
				removeKit = true;
			}
			if (removeKit) {
				this->currentKit = nullptr;
				kits.erase(remove(kits.begin(), kits.end(), kit), kits.end());
				break;
			}
		}
	}
	
}

bool Soldier::refill(vector<Kit*>& kits, int kitType) {
	// Return true if goes to refill HP. False otherwise.
	double distanceToKit = 100;
	if (this->isKitNeeded(kits)) {
		for (auto kit : kits) {
			if (kit->getType() == kitType) {
				double d = this->distance(kit->getCol(), kit->getRow());
				if (d < distanceToKit) {
					this->currentKit = kit;
					distanceToKit = d;
				}
			}
		}
	}
	if (currentKit != nullptr) {
		if (this->currentPath.empty()) {
			this->maze->runAStar(this->getCol(), this->getRow(),
				currentKit->getCol(), currentKit->getRow());
			stack<Node*> path = this->maze->getPath();
			swap(this->currentPath, path);
		}
		this->moveAndCollect(kits);
	}
	return currentKit != nullptr;
}

bool Soldier::moveAndCollect(vector<Kit*>& kits) {
	// Return true if can move, false otherwise.
	if (this->currentPath.empty()) {
		this->currentKit = nullptr;
		this->currentHideout = nullptr;
		return false;
	}
	Node* nextNode = this->currentPath.top();
	if (this->move(nextNode->getCol(), nextNode->getRow(), kits)) {
		this->currentPath.pop();
		return true;
	}
	this->currentKit = nullptr;
	return false;
}

bool Soldier::isKitNeeded(vector<Kit*> kits) {
	double distanceToKit = 100;
	if (kits.size() == 0) {
		this->currentKit = nullptr;
	}
	return this->currentKit == nullptr ||
		this->distance(this->currentKit->getCol(), this->currentKit->getRow()) > distanceToKit;
}

/* Enemy Functions: */

bool Soldier::needToFindNewEnemy() {
	double distanceToEnemy = 100;
	return this->ammo > 0 &&
		this->grenades > 0 &&
		(this->currentEnemy == nullptr ||
			this->currentEnemy->isDead() ||
			this->distance(this->currentEnemy->getCol(), this->currentEnemy->getRow()) > distanceToEnemy);
}

void Soldier::findNearestEnemy(vector<Soldier*>& enemySoldiers) {
	double distanceToEnemy = 100;
	for (auto soldier : enemySoldiers) {
		if (soldier->isDead()) {
			continue;
		}
		double d = this->distance(soldier->getCol(), soldier->getRow());
		if (d < distanceToEnemy) {
			this->currentEnemy = soldier;
			distanceToEnemy = d;
		}
	}
}

void Soldier::lookForEnemy(vector<Soldier*>& enemySoldiers) {
	if (!this->needToFindNewEnemy()) {
		return;
	}
	// Look for enemy:
	this->findNearestEnemy(enemySoldiers);
	// If enemy is found, move towards it or shoot it:
	if (currentEnemy != nullptr) {
		if (this->isEnemyInMyRoom()) {
			if (rand() % 5 == 0) { // Choose randomly between fire and throw grenade
				this->initGrenadeToss(this->currentEnemy->getCol(), this->currentEnemy->getRow());
			} else {
				this->initFire(this->currentEnemy->getCol(), this->currentEnemy->getRow());
			}
		}
		else { // Find path to the enemy:
			this->maze->runAStar(this->getCol(), this->getRow(),
				this->currentEnemy->getCol(), this->currentEnemy->getRow());
			stack<Node*> path = this->maze->getPath();
			swap(this->currentPath, path);
		}
	}
}

bool Soldier::moveTowardsEnemy(vector<Kit*>& kits) {
	// Return true if goes towards an enemy, false otherwise.
	if (this->isEnemyInMyRoom()) {
		stack<Node*> path;
		swap(this->currentPath, path);
	}
	if (this->moveAndCollect(kits)) {
		return true;
	}
	this->currentEnemy = nullptr;
	return false;
}

bool Soldier::isEnemyInMyRoom() {
	if (this->currentEnemy == nullptr) {
		return false;
	}
	if (this->isInsideTunnel()) {
		return false;
	}
	int* startEdge = this->getRoomEdge('-');
	int* endEdge = this->getRoomEdge('+');
	return this->currentEnemy->getCol() >= startEdge[0] &&
		this->currentEnemy->getCol() <= endEdge[0] &&
		this->currentEnemy->getRow() >= startEdge[1] &&
		this->currentEnemy->getRow() <= endEdge[1];
}

bool Soldier::isInsideTunnel() {
	return this->maze->isInsideTunnel(this->col, this->row);
}

int* Soldier::getRoomEdge(char direction) {
	int col = this->col;
	int row = this->row;
	while (col >= 0 && row >= 0 && col < MSZ && row < MSZ) {
		if (this->maze->get(col, row) == WALL) {
			break;
		} else {
			if (direction == '+') {
				col++;
			} else if (direction == '-') {
				col--;
			}
			if (this->maze->get(col, row) == WALL) {
				break;
			}
			if (direction == '+') {
				row++;
			} else if (direction == '-') {
				row--;
			}
			if (this->maze->get(col, row) == WALL) {
				break;
			}
		}
	}
	return new int[] {col, row};
}

/* Bullet Functions: */

void Soldier::shoot() {
	if (this->fire()) { // Enemy is hit
		this->currentEnemy->setHp((int)(-this->bullet->getDamage()));
	}
	if (!this->isFiring()) { // Bullet reached EOL
		this->currentEnemy = nullptr;
	}
}

void Soldier::initFire(int destCol, int destRow) {
	this->bullet = new Bullet(this->col, this->row, destCol, destRow, this->enemyColor);
	this->setAngle(this->bullet->getAngle());
	this->bullet->Fire(true);
	this->setAmmo(-1);
}

bool Soldier::fire() {
	return this->bullet && this->bullet->Move(this->maze);
}

bool Soldier::isFiring() {
	// Return true if the bullet stopped moving, otherwise false
	return this->bullet && this->bullet->IsMoving();
}

/* Grenade Functions: */

void Soldier::initGrenadeToss(int destCol, int destRow) {
	this->grenade = new Grenade(this->col, this->row, destCol, destRow, this->enemyColor, this->myColor);
	this->setAngle(this->grenade->getAngle());
	this->grenade->fly(true);
	this->setGrenades(-1);
}

void Soldier::throwGrenade(vector<Soldier*>& enemySoldiers) {
	if (this->grenade) { // Then grenade exists!
		if (this->grenade->go(this->maze)) { // then grenade stopped moving and exploding
			this->currentEnemy = nullptr;
		} else if (!this->grenade->isMoving() && !this->grenade->exploded()) {
			// Check for hits if the grenade is not moving but indeed exploding:
			for (auto bullet : this->grenade->getBullets()) {
				if (bullet->didHit()) {
					for (auto soldier : enemySoldiers) {
						int* bulletCell = bullet->coor2cell(bullet->GetX(), bullet->GetY());
						if (soldier->getCol() == bulletCell[0] && soldier->getRow() == bulletCell[1]) {
							soldier->setHp(-bullet->getDamage());
							bullet->Fire(false);
						}
					}
				}
			}
		}
	}
}

/* Utilities */

bool Soldier::move(int col, int row) {
	// Returns true if move succeeded, false otherwise.
	int nextNodeColor = this->maze->get(col, row);
	if (nextNodeColor == WALL || nextNodeColor == OBSTACLE ||
		nextNodeColor == TEAM_BLUE || nextNodeColor == TEAM_RED) {
		return false;
	}
	this->setAngle(col, row);
	this->bullet = nullptr;
	this->grenade = nullptr;
	this->maze->set(this->getCol(), this->getRow(), SPACE);
	this->col = col;
	this->row = row;
	this->maze->set(this->getCol(), this->getRow(), this->myColor);
	return true;
}

bool Soldier::move(int col, int row, vector<Kit*> &kits) {
	// Returns true if move succeeded, false otherwise.
	int nextNodeColor = this->maze->get(col, row);
	if (nextNodeColor == HP_KIT || nextNodeColor == AMMO_KIT || nextNodeColor == GRENADE_KIT) {
		this->collectKit(new Node(col, row), kits, nextNodeColor);
	}
	return this->move(col, row);
}

void Soldier::setAngle(int angle) {
	if (angle < 0) {
		angle += 360;
	}
	this->angle = angle;
}

void Soldier::setAngle(int col, int row) {
	if (this->col < col) {
		this->angle = 0;
	} else if (this->col > col) {
		this->angle = 180;
	} else if (this->row < row) {
		this->angle = 90;
	} else if (this->row > row) {
		this->angle = 270;
	}
}

void Soldier::drawMe() {
	if (this->isDead()) {
		return;
	}
	double size = 0.01;
	double* myCoors = this->cell2coor(this->col, this->row);
	double x = myCoors[0];
	double y = myCoors[1];
	if (this->myColor == TEAM_RED) {
		glColor3d(1, 0, 0);
	} else if (this->myColor == TEAM_BLUE) {
		glColor3d(0, 0, 1);
	}
	// Draw the soldier:
	glBegin(GL_POLYGON);
	glVertex2d(x - size, y - size);
	glVertex2d(x - size, y + size);
	glVertex2d(x + size, y + size);
	glVertex2d(x + size, y - size);
	glEnd();
	glPushMatrix();
	glTranslated(x, y, 0.0);
	glRotated(angle, 0.0, 0.0, 1.0);
	glTranslated(-x, -y, 0.0);
	glBegin(GL_POLYGON);
	glVertex2d(x, y + 0.5 * size);
	glVertex2d(x + 2 * size, y + 0.5 * size);
	glVertex2d(x + 2 * size, y - 0.5 * size);
	glVertex2d(x , y - 0.5 * size);
	glEnd();
	glPopMatrix();
	// Draw the bullet:
	if (bullet) {
		this->bullet->DrawMe();
	} else if (bullet && (!bullet->IsMoving())) {
		this->bullet = nullptr;
	} else if (grenade) {
		this->grenade->DrawMe();
	} else if (grenade && (!grenade->exploded())) {
		this->grenade = nullptr;
	}
}

double Soldier::distance(int col, int row) {
	return sqrt(pow((this->col - col), 2) + pow((this->row - row), 2));
}

double* Soldier::cell2coor(int col, int row) {
	double x = ((2.0 * col + 1.0) / MSZ) - 1.0;
	double y = ((2.0 * row + 1.0) / MSZ) - 1.0;
	return new double[] { x, y };
}

bool Soldier::isDead() {
	return this->hp <= 0;
}

/* Getters & Setters: */

int Soldier::getHp() {
	return this->hp;
}

void Soldier::setHp(int delta) {
	this->hp += delta;
	if (this->hp < 0) {
		this->hp = 0;
	}
	if (this->hp > MAX_HP) {
		this->hp = MAX_HP;
	}
}

int Soldier::getAmmo() {
	return this->ammo;
}

void Soldier::setAmmo(int delta) {
	this->ammo += delta;
	if (this->ammo < 0) {
		this->ammo = 0;
	}
	if (this->ammo > MAX_AMMO) {
		this->ammo = MAX_AMMO;
	}
}

int Soldier::getGrenades() {
	return this->grenades;
}

void Soldier::setGrenades(int delta) {
	this->grenades += delta;
	if (this->grenades < 0) {
		this->grenades = 0;
	}
	if (this->grenades > MAX_GRENADES) {
		this->grenades = MAX_GRENADES;
	}
}

int Soldier::getCol() {
	return this->col;
}

int Soldier::getRow() {
	return this->row;
}
