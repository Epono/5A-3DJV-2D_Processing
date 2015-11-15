#pragma once

#include <vector>
#include "Point.h"
#include "utils.h"

class Triangle {
private:
	Point* _pointA;
	Point* _pointB;
	Point* _pointC;
	color_rgb _color;

public:
	Triangle(Point* pointA, Point* pointB, Point* pointC)
		: _pointA(pointA), _pointB(pointB), _pointC(pointC), _color(color_rgb(1.f, 1.f, 1.f)) {}

	Triangle(Point* pointA, Point* pointB, Point* pointC, color_rgb color)
		: _pointA(pointA), _pointB(pointB), _pointC(pointC), _color(color) {}

	color_rgb getColor() { return _color; }
	void setColor(float r, float g, float b) {
		_color._r = r; _color._g = g; _color._b = b;
	}

	Point* getPointA() const { return _pointA; }
	void setPointA(Point* point) { _pointA = point; }

	Point* getPointB() const { return _pointB; }
	void setPointB(Point* point) { _pointB = point; }

	Point* getPointC() const { return _pointC; }
	void setPointC(Point* point) { _pointC = point; }
};