#pragma once
#include "Constants.h"

class Node {

private:
	int row, col;
	Node* parent;
	double g, h, f;

public:
	Node();
	Node(int r, int c);
	Node(Node* node);
	Node(int r, int c, int tr, int tc, double g, Node* p);
	double Distance(int r1, int c1, int r2, int c2);
	double* cell2coor(int col, int row);
	void SetRow(int r) { row = r; };
	void SetCol(int c) { col = c; };
	virtual int getRow() { return row; };
	virtual int getCol() { return col; };
	Node* GetParent() { return parent; };
	double GetF() { return f; };
	double GetG() { return g; };
	double GetH() { return h; };
	bool operator == (const Node &other) 
	{
		return row == other.row && col == other.col;
	}

};

