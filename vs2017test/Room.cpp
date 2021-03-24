#include "Room.h"
#include "glut.h"

using namespace std;

Room::Room() {
	width = 0;
	height = 0;
	centerX = 0;
	centerY = 0;
}

void Room::DrawMe() {
	glColor3d(1, 1, 1); // white
	glBegin(GL_POLYGON);
	glVertex2d(0.5, 0.2);
	glVertex2d(0.8, 0.2);
	glVertex2d(0.8, -0.1);
	glVertex2d(0.5, -0.1);
	glEnd();
}

void Room::FillMaze(int maze[MSZ][MSZ]) {
	int i, j;
	int obstacleCounter = 0;
	int maxObstacles = (height + width) / 2;
	for (i = centerY - height / 2; i < centerY + height / 2; i++) {
		for (j = centerX - width / 2; j < centerX + width / 2; j++) {
			if ((i > centerY - height / 2 && i < centerY + height / 2 - 1) &&
				(j > centerX - width / 2 && j < centerX + width / 2 - 1) &&
				(rand() % maxObstacles == 0) &&
				(obstacleCounter < maxObstacles)) {
				maze[i][j] = OBSTACLE;
				obstacleCounter++;
			}
			else {
				maze[i][j] = SPACE;
			}
		}
	}
	this->leftBottomCorner = new int[] {centerX - width / 2, centerY - height / 2};
	this->rightUpperCorner = new int[] {centerX + width / 2, centerY + height / 2};
}

void Room::initSafeCells(int mazeSecurityMap[MSZ][MSZ]) {
	vector<int> securityVector;
	for (int i = this->leftBottomCorner[1]; i < this->rightUpperCorner[1]; i++) {
		for (int j = this->leftBottomCorner[0]; j < this->rightUpperCorner[0]; j++) {
			securityVector.push_back(mazeSecurityMap[i][j]);
		}
	}
	std::sort(securityVector.begin(), securityVector.end());
	int threshold = securityVector[securityVector.size() / 10];
	for (int i = this->leftBottomCorner[1]; i < this->rightUpperCorner[1]; i++) {
		for (int j = this->leftBottomCorner[0]; j < this->rightUpperCorner[0]; j++) {
			if (mazeSecurityMap[i][j] <= threshold) {
				this->safeCells.push_back(new int[] {j, i}); // push col and then row as cell!
			}
		}
	}
}

int* Room::getSafeCell() {
	if (safeCells.size() == 0) {
		return nullptr;
	}
	return safeCells[rand() % safeCells.size()];
}

int Room::getAmountOf(int maze[MSZ][MSZ], int color) {
	int counter = 0;
	for (int i = centerY - height / 2; i < centerY + height / 2; i++) {
		for (int j = centerX - width / 2; j < centerX + width / 2; j++) {
			if (maze[i][j] == color) {
				counter++;
			}
		}
	}
	return counter;
}

bool Room::IsOverlap(int w, int h, int row, int col)
{
	int hdist, vdist; // horizontal and vertical distances between rooms centers

	hdist = abs(col - centerX);
	vdist = abs(row - centerY);
	return hdist < w / 2 + width / 2 + 2 && vdist < h / 2 + height / 2 + 2;
}

bool Room::isInside(int col, int row) {
	return col > this->leftBottomCorner[0] && col < this->rightUpperCorner[0] &&
		row > this->leftBottomCorner[1] && row < this->rightUpperCorner[1];
}

int Room::getHeight() {
	return this->height;
}

int Room::getWidth() {
	return this->width;
}
