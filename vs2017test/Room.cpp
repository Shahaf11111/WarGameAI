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

void Room::FillMaze(int maze[MSZ][MSZ])
{
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
}

bool Room::IsOverlap(int w, int h, int row, int col)
{
	int hdist, vdist; // horizontal and vertical distances between rooms centers

	hdist = abs(col - centerX);
	vdist = abs(row - centerY);
	return hdist < w / 2 + width / 2 + 2 && vdist < h / 2 + height / 2 + 2;
}

int Room::getHeight() {
	return this->height;
}

int Room::getWidth() {
	return this->height;
}