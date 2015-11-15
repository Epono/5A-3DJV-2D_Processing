#pragma once

#include <vector>
#include <iostream>

#include "TriangleStrip.h"

class Triangulation2D_qcq : TriangleStrip {
private:

public:
	Triangulation2D_qcq() : TriangleStrip() {}
	Triangulation2D_qcq(std::vector<Point*> points);

	void computeTriangulation();
	void addPoint(Point& const point);
};