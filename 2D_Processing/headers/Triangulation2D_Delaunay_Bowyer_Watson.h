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

	bool Circumcircle(const Point& p0, const Point& p1, const Point& p2, Point& center, float& radius);
	bool IsInsideCircle(Point& center, float radius, Point& p);

	void computeTriangulation();
};