#pragma once
#include "Constants.h"

class Node {

private:
	int row, col;
	Node* parent;
	double g; // Accumulative cost
	double h; // Personal cost
	double f; // Total cost (accumulative + personal)

public:
	Node();
	Node(int r, int c);
	Node(Node* node);
	Node(int r, int c, int tr, int tc, double g, Node* p);
	
	double distance(int r1, int c1, int r2, int c2);
	double* cell2coor(int col, int row);
	
	// Getters & Setters:
	void setRow(int r) { row = r; };
	void setCol(int c) { col = c; };
	virtual int getRow() { return row; };
	virtual int getCol() { return col; };
	Node* getParent() { return parent; };
	double getF() { return f; };
	double getG() { return g; };
	double getH() { return h; };
	bool operator == (const Node &other)  {
		return row == other.row && col == other.col;
	}

};

