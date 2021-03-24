#include "Bullet.h"

using namespace std;

Bullet::Bullet(int startCol, int startRow, int targetCol, int targetRow, int targetColor) {
	double* startCoor = this->cell2coor(startCol, startRow);
	double* endCoor = this->cell2coor(targetCol, targetRow);
	this->initialX = startCoor[0];
	this->initialY = startCoor[1];
	this->x = startCoor[0];
	this->y = startCoor[1];
	double size = sqrt(pow(endCoor[0] - startCoor[0], 2) + pow(endCoor[1] - startCoor[1], 2));
	this->dirx = (endCoor[0] - startCoor[0]) / size;
	this->diry = (endCoor[1] - startCoor[1]) / size;
	this->isMoving = false;
	this->angle = int(atan2(diry, dirx) * 180 / 3.14);
	this->targetColor = targetColor;
	this->setDamage(endCoor[0], endCoor[1]);
	this->hit = false;
}

Bullet::Bullet(double startX, double startY, double directionX, double directionY, int targetColor) {
	this->initialX = startX;
	this->initialY = startY;
	this->x = startX;
	this->y = startY;
	this->dirx = directionX;
	this->diry = directionY;
	this->isMoving = false;
	this->angle = int(atan2(diry, dirx) * 180 / 3.14);
	this->targetColor = targetColor;
	this->damage = -1;
	this->hit = false;
}

void Bullet::setDamage(double destX, double destY) {
	double distanceFactor = (sqrt(pow((this->x - destX), 2) + pow((this->y - destY), 2)));
	this->damage = int(BULLET_DAMAGE / distanceFactor + 1);
}

void Bullet::DrawMe() {
	if (!this->isMoving) {
		return;
	}
	double size = 1.2 / MSZ;
	glColor3d(0.0, 0.8, 0.4);
	glBegin(GL_POLYGON);
	glVertex2d(this->x, this->y + size);
	glVertex2d(x + size, y);
	glVertex2d(x, y - size);
	glVertex2d(x - size, y);
	glEnd();
}

bool Bullet::Move(Maze* maze) {
	// return true if hit a soldier. otherwise return false
	int* cell = this->coor2cell(this->x, this->y);
	int myCellColor = maze->get(cell[0], cell[1]);
	if (myCellColor == this->targetColor) {
		Fire(false);
		this->hit = true;
		return true;
	}
	if (myCellColor == WALL) {
		Fire(false);
	} else { // moving on
		this->x += SPEED * dirx;
		this->y += SPEED * diry;
	}
	return false;
}

int* Bullet::updateSecurityMap(int maze[MSZ][MSZ]) {
	int* cell = this->coor2cell(this->x, this->y);
	if (maze[cell[1]][cell[0]] == WALL) {
		this->Fire(false);
		return nullptr;
	}
	this->x += SPEED * this->dirx;
	this->y += SPEED * this->diry;
	return cell;
}

int Bullet::calcDamage() {
	double distanceFactor = (sqrt(pow((this->initialX - this->x), 2) + pow((this->initialY - this->y), 2)));
	return int(BULLET_DAMAGE / distanceFactor + 1);
}

int* Bullet::coor2cell(double x, double y) {
	int col = int((MSZ * (x + 1) - 1) / 2);
	int row = int((MSZ * (y + 1) - 1) / 2);
	return new int[] { col, row };
}

double* Bullet::cell2coor(int col, int row) {
	double x = ((2.0 * col + 1.0) / MSZ) - 1.0;
	double y = ((2.0 * row + 1.0) / MSZ) - 1.0;
	return new double[] { x, y };
}

bool Bullet::didHit() {
	if (this->hit) {
		this->hit = false;
		return true;
	}
	return false;

}