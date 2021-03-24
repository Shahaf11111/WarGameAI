#pragma once
#include "Node.h"

void drawRectangle(double* color, double x, double y,
	double width, double height, double rotateX, double rotateY, int angle);

void drawTriangle(double* color, double centerX, double centerY,
	double width, double height, double rotateX, double rotateY, int angle);

class Kit : public Node {

private:
	int amount;
	int type;
	double color[3];

public:
	Kit(int c, int r, int amount, int type, double* color);
	int getAmount();
	void drawMe();
	int getType() { return this->type; }

};