#include "Kit.h"
#include "glut.h"


void drawRectangle(double* color, double centerX, double centerY,
	double width, double height, double rotateX, double rotateY, int angle) {
	glColor3d(color[0], color[1], color[2]);
	if (rotateX != NULL && rotateY != NULL && angle != NULL) {
		glPushMatrix();
		glTranslated(rotateX, rotateY, 0.0);
		glRotated(angle, 0.0, 0.0, 1.0);
		glTranslated(-rotateX, -rotateY, 0.0);
		glColor3d(color[0], color[1], color[2]);
	}
	glBegin(GL_POLYGON);
	glVertex2d(centerX - width, centerY - height);
	glVertex2d(centerX - width, centerY + height);
	glVertex2d(centerX + width, centerY + height);
	glVertex2d(centerX + width, centerY - height);
	glEnd();
	if (rotateX != NULL && rotateY != NULL && angle != NULL) {
		glPopMatrix();
	}
}

void drawTriangle(double* color, double centerX, double centerY,
	double width, double height, double rotateX, double rotateY, int angle) {
	glColor3d(color[0], color[1], color[2]);
	if (rotateX != NULL && rotateY != NULL && angle != NULL) {
		glPushMatrix();
		glTranslated(rotateX, rotateY, 0.0);
		glRotated(angle, 0.0, 0.0, 1.0);
		glTranslated(-rotateX, -rotateY, 0.0);
		glColor3d(color[0], color[1], color[2]);
	}
	glBegin(GL_POLYGON);
	glVertex2d(centerX, centerY - height);
	glVertex2d(centerX + width, centerY);
	glVertex2d(centerX, centerY + height);
	glEnd();
	if (rotateX != NULL && rotateY != NULL && angle != NULL) {
		glPopMatrix();
	}
}

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
	double size = 0.65 / MSZ;
	double* myCoors = this->cell2coor(this->getCol(), this->getRow());
	double x = myCoors[0];
	double y = myCoors[1];
	drawRectangle(new double[] {0, 0, 0}, x, y, size, size * 2, NULL, NULL, NULL);
	drawRectangle(new double[] {0, 0, 0}, x, y, size * 2, size, NULL, NULL, NULL);
	size *= 0.75;
	drawRectangle(this->color, x, y, size, size * 2, NULL, NULL, NULL);
	drawRectangle(this->color, x, y, size * 2, size, NULL, NULL, NULL);
}
