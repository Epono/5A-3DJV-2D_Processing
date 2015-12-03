#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "TriangleStrip.h"
#include "utils.h"

class Triangulation2D_Delaunay_Bowyer_Watson : public TriangleStrip {
private:
public:
	Triangulation2D_Delaunay_Bowyer_Watson() : TriangleStrip() {}
	Triangulation2D_Delaunay_Bowyer_Watson(std::vector<Point*> points)
		: TriangleStrip(points) {}

	bool removeIf(Triangle* triangle);


	void computeTriangulation();
};