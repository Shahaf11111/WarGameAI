
#include "Grenade.h"

Grenade::Grenade(int startCol, int startRow, 
	int targetCol, int targetRow, 
	int targetColor, int friendlyColor) {
	double* startCoor = this->cell2coor(startCol, startRow);
	double* endCoor = this->cell2coor(targetCol, targetRow);
	this->x = startCoor[0];
	this->y = startCoor[1];
	double size = sqrt(pow(endCoor[0] - startCoor[0], 2) + pow(endCoor[1] - startCoor[1], 2));
	this->dirx = (endCoor[0] - startCoor[0]) / size;
	this->diry = (endCoor[1] - startCoor[1]) / size;
	this->cx = endCoor[0];
	this->cy = endCoor[1];
	this->targetColor = targetColor;
	this->friendlyColor = friendlyColor;
	this->angle = (int)(atan2(diry, dirx) * 180 / 3.14);
	this->drawAngle = 0;
}

bool Grenade::go(Maze* maze) {
	// Return true if grenade finished moving and exploding
	if (this->isMoving()) {
		this->move(maze);
		return false;
	}
	if (!this->isExploded) {
		this->Exploding(maze);
		return this->isExploded;
	}
	return true;
}

bool Grenade::move(Maze* maze) {
	// return true if need to explode. otherwise return false
	int* cell = this->coor2cell(this->x, this->y);
	int myCellColor = maze->get(cell[0], cell[1]);
	if (myCellColor != SPACE && myCellColor != this->friendlyColor) {
		this->fly(false);
		this->Explode();
		return true;
	} else { // moving on
		x += SPEED * dirx;
		y += SPEED * diry;
	}
	return false;
}

void Grenade::Explode() {
	int i;
	double alpha, theta = 2 * 3.14 / NUM_BULLETS;
	this->bullets.clear();
	for (i = 0, alpha = 0; i < NUM_BULLETS; i++, alpha += theta) {
		this->bullets.push_back(new Bullet(this->x, this->y, 
			cos(alpha), sin(alpha), this->targetColor));
		this->bullets.at(i)->Fire(true);
	}
	this->isExploded = false;

}

void Grenade::Exploding(Maze* maze) {
	int notMovingCounter = 0;
	for (auto bullet : this->bullets) {
		if (bullet->IsMoving()) {
			bullet->Move(maze);
		} else {
			notMovingCounter++;
		}
	}
	this->isExploded = notMovingCounter == NUM_BULLETS;
}

void Grenade::DrawMe() {
	if (this->isFlying) {
		double size = 1.2 / MSZ;
		glColor3d(0.5, 1.0, 0.5);
		glPushMatrix();
		glTranslated(this->x, this->y, 0.0);
		glRotated(this->drawAngle, 0.0, 0.0, 1.0);
		glTranslated(-this->x, -this->y, 0.0);
		glBegin(GL_POLYGON);
		glVertex2d(this->x, this->y + size);
		glVertex2d(x + size, y);
		glVertex2d(x, y - size);
		glVertex2d(x - size, y);
		glEnd();
		glPopMatrix();
		this->drawAngle += 10;
		if (this->drawAngle >= 360) {
			this->drawAngle = 0;
		}
	} else {
		for (int i = 0; i < NUM_BULLETS; i++) {
			this->bullets.at(i)->DrawMe();
		}
	}
}

int* Grenade::coor2cell(double x, double y) {
	int col = int((MSZ * (x + 1) - 1) / 2);
	int row = int((MSZ * (y + 1) - 1) / 2);
	return new int[] { col, row };
}

double* Grenade::cell2coor(int col, int row) {
	double x = ((2.0 * col + 1.0) / MSZ) - 1.0;
	double y = ((2.0 * row + 1.0) / MSZ) - 1.0;
	return new double[] { x, y };
}

set<int*, CellPosComparator> Grenade::updateSecurityMap(int maze[MSZ][MSZ]) {
	int notMovingCounter;
	this->Explode(); // set bullets on fire
	set<int*, CellPosComparator> cellsToUpdate;
	while (!this->isExploded) {
		notMovingCounter = 0;
		for (auto bullet : this->bullets) {
			if (bullet->IsMoving()) {
				int* cell = bullet->updateSecurityMap(maze);
				if (cell != nullptr) {
					cellsToUpdate.insert(cell);
				}
			} else {
				notMovingCounter++;
			}
		}
		this->isExploded = notMovingCounter == NUM_BULLETS;
	}
	return cellsToUpdate;
}