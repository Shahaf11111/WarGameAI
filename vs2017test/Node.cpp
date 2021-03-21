#include "Node.h"
#include <math.h>

Node::Node()
{
	row = 0;
	col = 0;
	g = 0;
	h = 0;
	f = 0;
	parent = nullptr;
}

Node::Node(int c, int r) {
	col = c;
	row = r;
	parent = nullptr;
	g = 0;
	h = 0;
	f = 0;
}

Node::Node(Node* node) {
	this->row = node->row;
	this->col = node->col;
	this->parent = node->parent;
	this->g = node->g;
	this->h = node->h;
	this->f = node->f;
}

// (r,c) are the coordinates of the new Node,
// (tr,tc) are the coordinates of the target. We'll need it to 
// compute Heuristics: h
Node::Node(int r, int c, int tr, int tc, double g, Node* p)
{
	row = r;
	col = c;
	this->g = g;
	parent = p;
	h = Distance(r, c, tr, tc); // we could use Manhattan Distance
	f = g + h;
}

double Node::Distance(int r1, int c1, int r2, int c2)
{
	return sqrt(pow((r1 - r2), 2) + pow((c1 - c2), 2));
}

double* Node::cell2coor(int col, int row) {
	double x = ((2.0 * col + 1.0) / MSZ) - 1.0;
	double y = ((2.0 * row + 1.0) / MSZ) - 1.0;
	return new double[] { x, y };
}

