#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "TriangleStrip.h"

class Triangulation2D_qcq : public TriangleStrip {
private:

public:
	Triangulation2D_qcq() : TriangleStrip() {}
	Triangulation2D_qcq(std::vector<Point*> points);


	void computeTriangulation();
	void flippingEdges();
};