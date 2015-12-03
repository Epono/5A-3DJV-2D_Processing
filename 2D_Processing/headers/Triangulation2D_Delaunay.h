#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "TriangleStrip.h"
#include "utils.h"

class Triangulation2D_Delaunay : public TriangleStrip {
private:
public:
	Triangulation2D_Delaunay() : TriangleStrip() {}
	Triangulation2D_Delaunay(std::vector<Point*> points)
		: TriangleStrip(points) {}

	bool Circumcircle(const Point& p0, const Point& p1, const Point& p2, Point& center, float& radius);
	bool IsInsideCircle(Point& center, float radius, Point& p);

	void computeTriangulation();
};