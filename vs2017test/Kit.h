#pragma once
#include "Node.h"

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