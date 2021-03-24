#include "Soldier.h"

using namespace std;
int thisisanindex = 0;
Soldier::Soldier(Maze* maze, int col, int row, int myColor, int enemyColor) {
	this->index = thisisanindex;
	thisisanindex++;
	this->maze = maze;
	this->col = col;
	this->row = row;
	this->myColor = myColor;
	this->enemyColor = enemyColor;
	this->ammo = MAX_AMMO;// rand() % MAX_AMMO + 1;
	this->grenadeAmount = MAX_GRENADES;// rand() % MAX_GRENADES + 1;
	this->hp = MAX_HP;// rand() % MAX_HP + 1;
	this->numTurnsToHide = 0;
	this->angle = rand() % 360;
	this->bullet = nullptr;
	this->grenade = nullptr;
	this->currentKit = nullptr;
	this->currentEnemy = nullptr;
	this->currentHideout = nullptr;
	this->isStabbing = false;
}

bool Soldier::refill(vector<Kit*>& kits) {
	// Return true if did refill, otherwise false
	if (this->hp <= LOW_HP_THRESHOLD) {
		if (this->refillKit(kits, HP_KIT)) {
			return true;
		}
	}
	if (this->ammo <= LOW_AMMO_THRESHOLD) {
		if (this->refillKit(kits, AMMO_KIT)) {
			return true;
		}
	}
	if (this->grenadeAmount <= LOW_GRENADE_THRESHOLD) {
		if (this->refillKit(kits, GRENADE_KIT)) {
			return true;
		}
	}
	return false;
}

bool Soldier::fight(vector<Soldier*>& enemySoldiers, vector<Kit*>& kits) {
	// Return true if did fight, otherwise false
	bool retVal = false;
	this->lookForEnemy(enemySoldiers);
	if (this->currentEnemy != nullptr) {
		if (this->bullet != nullptr) {
			this->shoot();
			retVal = true;
		}
		if (this->grenade != nullptr) {
			this->throwGrenade(enemySoldiers);
			retVal = true;
		}
		if (!retVal) {
			if (this->canStabCurrentEnemy()) {
				this->stabCurrentEnemy();
				retVal = true;
			}
			else if (this->moveTowardsEnemy(kits)) {
				retVal = true;
			}
		}
	}
	return retVal;
}

bool Soldier::canStabCurrentEnemy() {
	return abs(this->col - this->currentEnemy->getCol()) <= 1 &&
		abs(this->row - this->currentEnemy->getRow()) <= 1;
}

void Soldier::stabCurrentEnemy() {
	this->isStabbing = true;
	this->currentEnemy->setHp(-STAB_DAMAGE);
}

void Soldier::run(vector<Kit*>& kits) {
	this->hide();
	if (this->currentHideout != nullptr) {
		this->escape(kits);
	}
}

void Soldier::printMe(int i) {
	string enemyExists = this->currentEnemy != nullptr ? "true" : "false";
	string hideoutExists = this->currentHideout != nullptr ? "true" : "false";
	string kitExists = this->currentKit != nullptr ? "true" : "false";
	if (this->index == i && this->currentKit == nullptr && 
		(this->hp < LOW_HP_THRESHOLD || this->ammo < LOW_AMMO_THRESHOLD 
			|| this->grenadeAmount < LOW_GRENADE_THRESHOLD)) {
		cout<<"FUCK"<<endl;
	}
	if (this->index == i || this->index == i + NUM_SOLDIERS)
		if (this->myColor == TEAM_BLUE)
			cout << "BLUE " << this->index <<
			" || POS:" << this->col << "," << this->row <<
			" || HP:" << this->hp <<
			" || AMMO:" << this->ammo <<
			" || GRENADE:" << this->grenadeAmount <<
			" || ENEMY: " << enemyExists <<
			" || KIT: " << kitExists <<
			" || HIDE: " << hideoutExists << endl;
		/*else
			cout << "RED " << this->index <<
			" || POS:" << this->col << "," << this->row <<
			" || HP:" << this->hp <<
			" || AMMO:" << this->ammo <<
			" || GRENADE:" << this->grenadeAmount <<
			" || ENEMY EXISTS: " << enemyExists <<
			" || KIT EXISTS: " << kitExists <<
			" || HIDE EXISTS: " << hideoutExists << endl;*/
}

bool Soldier::play(vector<Soldier*>& enemySoldiers, vector<Kit*>& kits) {
	// Return true if alive, false otherwise
	if (this->isDead()) {
		return false;
	}
	//this->printMe(0);
	if (!this->refill(kits)) {
		if (!this->fight(enemySoldiers, kits)) {
			this->run(kits);
		}
	}
	// Keep the grenage ongoing (even if the soldier is not currently fighting):
	if (this->grenade != nullptr) {
		this->throwGrenade(enemySoldiers);
	}
	return true;
}

/* Hide functions: */

void Soldier::hide() {
	if (this->currentHideout == nullptr || this->numTurnsToHide > MAX_TURNS_TO_HIDE) {
		this->findSafeHideout();
		this->numTurnsToHide = 0;
	}
	this->numTurnsToHide++;
	this->maze->runAStar(this->getCol(), this->getRow(),
		this->currentHideout->getCol(), this->currentHideout->getRow(),
		this->myColor, this->enemyColor);
	stack<Node*> path = this->maze->getPath();
	swap(this->currentPath, path);
}

void Soldier::findSafeHideout() {
	while (this->currentHideout == nullptr) {
		// Find a safe room to team up with:
		int* safeCell = this->maze->getSafeCellWith(this->col, this->row, this->myColor);
		if (safeCell != nullptr) { // Found a room with friends
			this->currentHideout = new Node(safeCell[0], safeCell[1]);
		} else {
			while (safeCell == nullptr) {
				safeCell = this->maze->getRoomAt(rand() % NUM_ROOMS).getSafeCell();
				if (safeCell != nullptr) {
					this->currentHideout = new Node(safeCell[0], safeCell[1]);
				}
			}
		}
	}
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
				this->setGrenadeAmount(kit->getAmount());
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

bool Soldier::refillKit(vector<Kit*>& kits, int kitType) {
	// Return true if goes to refill HP. False otherwise.
	double distanceToKit = DISTANCE_TO_SEARCH_KIT;
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
				currentKit->getCol(), currentKit->getRow(), 
				this->myColor, this->enemyColor);
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
	double distanceToKit = DISTANCE_TO_SEARCH_KIT;
	if (kits.size() == 0) {
		this->currentKit = nullptr;
	}
	return this->currentKit == nullptr ||
		this->distance(this->currentKit->getCol(), this->currentKit->getRow()) > distanceToKit;
}

/* Enemy Functions: */

bool Soldier::needToFindNewEnemy() {
	return this->ammo > 0 &&
		this->grenadeAmount > 0 &&
		(this->currentEnemy == nullptr ||
			this->currentEnemy->isDead() ||
			this->distance(this->currentEnemy->getCol(),
				this->currentEnemy->getRow()) > DISTANCE_TO_SEARCH_ENEMY);
}

void Soldier::findNearestEnemy(vector<Soldier*>& enemySoldiers) {
	double distanceToEnemy = DISTANCE_TO_SEARCH_ENEMY;
	for (auto soldier : enemySoldiers) {
		if (soldier->isDead()) {
			continue;
		}
		if (!soldier->isInsideTunnel()) {
			double d = this->distance(soldier->getCol(), soldier->getRow());
			if (d < distanceToEnemy) {
				this->currentEnemy = soldier;
				distanceToEnemy = d;
			}
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
	if (currentEnemy == nullptr) { // Enemy is not found -> return!
		return;
	}
	// If reached here -> there's a current enemy
	int numEnemiesInMyRoom =
		this->maze->getAmountOfColorInRoom(this->col, this->row, this->enemyColor);
	if (numEnemiesInMyRoom == 0) { // No enemies in my room -> search for path to enemy
		this->maze->runAStar(this->getCol(), this->getRow(),
			this->currentEnemy->getCol(), this->currentEnemy->getRow(),
			this->myColor, this->enemyColor);
		stack<Node*> path = this->maze->getPath();
		swap(this->currentPath, path);
		return;
	}
	// If reached here, there is an enemy in my room -> fight or escape:
	int numFriendsInMyRoom =
		this->maze->getAmountOfColorInRoom(this->col, this->row, this->myColor);
	if (numEnemiesInMyRoom >= numFriendsInMyRoom) { // then go hide!
		this->currentEnemy = nullptr; // reset current enemy
		this->currentHideout = nullptr; // reset current hideout 
		return;
	}
	// Choose randomly between fire and throw grenade: 
	if (rand() % 3 == 0 && this->grenade == nullptr) {
		this->initGrenadeToss(this->currentEnemy->getCol(), this->currentEnemy->getRow());
	} else {
		this->initFire(this->currentEnemy->getCol(), this->currentEnemy->getRow());
	}
}

bool Soldier::moveTowardsEnemy(vector<Kit*>& kits) {
	// Return true if goes towards an enemy, false otherwise.
	if (this->maze->getAmountOfColorInRoom(this->col, this->row, this->enemyColor) > 0) {
		stack<Node*> path;
		swap(this->currentPath, path);
	} else if (this->moveAndCollect(kits)) {
		return true;
	}
	this->currentEnemy = nullptr;
	return false;
}

bool Soldier::isInsideTunnel() {
	return this->maze->isInsideTunnel(this->col, this->row);
}

/* Bullet Functions: */

void Soldier::shoot() {
	if (this->bullet && this->bullet->Move(this->maze)) { // Enemy is hit
		this->currentEnemy->setHp((int)(-this->bullet->getDamage()));
		this->bullet = nullptr;
	}
	if (!(this->bullet && this->bullet->IsMoving())) { // Bullet reached EOL
		this->currentEnemy = nullptr;
		this->bullet = nullptr;
	}
}

void Soldier::initFire(int destCol, int destRow) {
	this->bullet = new Bullet(this->col, this->row, destCol, destRow, this->enemyColor);
	this->setAngle(this->bullet->getAngle());
	this->bullet->Fire(true);
	this->setAmmo(-1);
}

/* Grenade Functions: */

void Soldier::initGrenadeToss(int destCol, int destRow) {
	this->grenade = new Grenade(this->col, this->row, destCol, destRow, 
		this->enemyColor, this->myColor);
	this->setAngle(this->grenade->getAngle());
	this->setGrenadeAmount(-1);
}

void Soldier::throwGrenade(vector<Soldier*>& enemySoldiers) {
	if (this->grenade) { // Then grenade exists!
		this->grenade->go(this->maze); // grenade is on the go
		if (!this->grenade->isMoving()) {
			if (this->grenade->exploded()) {
				this->grenade = nullptr;
			} else {
				// Then now grenade stopped moving and is exploding
				// Check for hits if the grenade is not moving but indeed exploding:
				for (auto bullet : this->grenade->getBullets()) {
					if (bullet->didHit()) {
						for (auto soldier : enemySoldiers) {
							int* bulletCell = bullet->coor2cell(bullet->GetX(), bullet->GetY());
							if (soldier->getCol() == bulletCell[0] &&
								soldier->getRow() == bulletCell[1]) {
								soldier->setHp(-bullet->getDamage());
								bullet->Fire(false);
							}
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
	if (nextNodeColor == WALL || nextNodeColor == OBSTACLE) {
		return false;
	}
	this->setAngle(col, row);
	this->bullet = nullptr;
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
	double size = 1.2 / MSZ;
	double* myCoors = this->cell2coor(this->col, this->row);
	double x = myCoors[0];
	double y = myCoors[1];
	// Draw the soldier:
	double drawColor[3] = { 0 };
	if (this->myColor == TEAM_RED) {
		drawColor[0] = 1;
	}
	else if (this->myColor == TEAM_BLUE) {
		drawColor[2] = 1;
	}
	// Rifle:
	drawRectangle(drawColor, x + size, y, size, size / 2, x, y, this->angle);
	// Outer body:
	drawRectangle(drawColor, x, y, size, size, NULL, NULL, NULL);
	// Inner body:
	drawRectangle(new double[] {0, 0, 0}, x, y, size / 2, size / 2, x, y, this->angle);
	// Draw the dagger or the bullet or the grenade (if exists):
	if (this->isStabbing) {
		drawTriangle(new double[] {0, 0, 0}, x + size, y,
			size * 2, size / 2, x, y, this->angle);
	}
	this->isStabbing = false;
	if (this->bullet) {
		this->bullet->DrawMe();
	} else if (bullet && (!bullet->IsMoving())) {
		this->bullet = nullptr;
	}
	if (this->grenade) {
		this->grenade->DrawMe();
	} else if (grenade && grenade->exploded()) {
		this->grenade = nullptr;
	}
}

double Soldier::distance(int col, int row) {
	return sqrt(pow(this->col - col, 2) + pow(this->row - row, 2));
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

int Soldier::getGrenadeAmount() {
	return this->grenadeAmount;
}

void Soldier::setGrenadeAmount(int delta) {
	this->grenadeAmount += delta;
	if (this->grenadeAmount < 0) {
		this->grenadeAmount = 0;
	}
	if (this->grenadeAmount > MAX_GRENADES) {
		this->grenadeAmount = MAX_GRENADES;
	}
}

int Soldier::getCol() {
	return this->col;
}

int Soldier::getRow() {
	return this->row;
}
