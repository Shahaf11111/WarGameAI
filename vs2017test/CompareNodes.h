#pragma once
#include "Node.h"

class CompareNodes
{
public:
	bool operator() (Node n1, Node n2)
	{
		return n1.GetF() > n2.GetF();
	}

};

