#pragma once
#include "Node.h"

class HpKit : public Node {

private:
	int hp;

public:
	HpKit(int c, int r, int hp);
	int getHp();
	void drawMe();
};