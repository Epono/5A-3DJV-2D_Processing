#pragma once

#include <vector>
#include <climits>
#include <algorithm>
#include "QuadEdge.h"
#include "Line.h"
#include "Triangle.h"
#include "LineStrip.h"
#include "utils.h"

class Delaunay {
private:
	QuadEdge* _startingEdge;
	std::vector<QuadEdge*> _quadEdges;
	Point *_bottomLeft, *_bottomRight, *_topRight, *_topLeft;

private:
	void updateBoundigBox(Point* p);
	QuadEdge* locate(Point* p);

public:
	Delaunay();
	void insertPoint(Point* p);
	std::vector<Line*> computeEdges();
	std::vector<Triangle*> computeTriangles();
	std::vector<LineStrip*> computeVoronoi();
};