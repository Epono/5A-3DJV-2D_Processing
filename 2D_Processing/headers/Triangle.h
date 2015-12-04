#pragma once

#include <vector>
#include "Point.h"
#include "utils.h"
#include "Line.h"

class Triangle {
private:
	Point* _pointA;
	Point* _pointB;
	Point* _pointC;
	color_rgb _color;
	Line* _lineA;
	Line* _lineB;
	Line* _lineC;

public:
	Triangle(Point* pointA, Point* pointB, Point* pointC)
		: _pointA(pointA), _pointB(pointB), _pointC(pointC), _color(color_rgb(1.f, 1.f, 1.f)) {}

	Triangle(Line* lineA, Line* lineB, Line* lineC)
		: _lineA(lineA), _lineB(lineB), _lineC(lineC), _color(color_rgb(1.f, 1.f, 1.f)) {
		_pointA = lineA->getStartPoint();
		_pointB = lineB->getStartPoint();
		_pointC = lineC->getStartPoint();
	}

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

	Line* LineA() const { return _lineA; }
	void LineA(Line* val) { _lineA = val; }
	Line* LineB() const { return _lineB; }
	void LineB(Line* val) { _lineB = val; }
	Line* LineC() const { return _lineC; }
	void LineC(Line* val) { _lineC = val; }
};