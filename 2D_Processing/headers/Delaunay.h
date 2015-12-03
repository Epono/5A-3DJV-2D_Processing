#pragma once

#include <vector>
#include <climits>
#include <algorithm>
#include "QuadEdge.h"

class Delaunay {
private:
	QuadEdge* _startingEdge;
	std::vector<QuadEdge*> _quadEdges;
	float _minx;
	float _miny;
	float _maxx;
	float _maxy;
	Point* _a;
	Point* _b;
	Point* _c;
	Point* _d;

private:
	void updateBoundigBox(Point* p);
	QuadEdge* locate(Point* p);

public:
	Delaunay();
	void setBoundigBox(float minx, float miny, float maxx, float maxy);
	void insertPoint(Point* p);
	std::vector<std::vector<Point*>*> computeEdges();
	std::vector<std::vector<Point*>*> computeTriangles();
	std::vector<std::vector<Point*>*> computeVoronoi();
};