#include "HpKit.h"
#include "glut.h"

HpKit::HpKit(int c, int r, int hp) : Node(c, r) {
	this->hp = hp;
}

int HpKit::getHp()
{
	return this->hp;
}

void HpKit::drawMe() {
	double size = 0.01;
	double* myCoors = this->cell2coor(this->getCol(), this->getRow());
	double x = myCoors[0];
	double y = myCoors[1];
	glColor3d(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2d(x - size, y - size);
	glVertex2d(x - size, y + size);
	glVertex2d(x + size, y + size);
	glVertex2d(x + size, y - size);
	glEnd();
}
