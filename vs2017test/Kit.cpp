#include "Kit.h"
#include "glut.h"

Kit::Kit(int c, int r, int amount, int type, double* color) : Node(c, r) {
	this->amount = amount;
	this->type = type;
	memcpy(this->color, color, 3 * sizeof(double));
}

int Kit::getAmount()
{
	return this->amount;
}

void Kit::drawMe() {
	double size = 0.01;
	double* myCoors = this->cell2coor(this->getCol(), this->getRow());
	double x = myCoors[0];
	double y = myCoors[1];
	glColor3d(this->color[0], this->color[1], this->color[2]);
	// Diamond shape:
	glBegin(GL_POLYGON);
	glVertex2d(x, y + size);
	glVertex2d(x + size, y);
	glVertex2d(x, y - size);
	glVertex2d(x - size, y);
	glEnd();
}
