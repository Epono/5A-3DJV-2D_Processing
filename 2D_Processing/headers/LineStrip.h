#pragma once

#include <vector>
#include "Point.h"
#include "utils.h"

class LineStrip {
protected:
	std::vector<Point> _points;
	color_rgb _color;
public:
	LineStrip() : 
		_points(std::vector<Point>()), _color(color_rgb(1.f, 1.f, 1.f)){}

	LineStrip(std::vector<Point> p) 
		: _points(p), _color(color_rgb(1.f,1.f,1.f)){}

	Point operator[](size_t i) const { return _points.at(i); }
	bool isEmpty() const { return _points.size() == 0; }

	std::vector<Point> getPoints() const { return _points; }
	void setPoints(std::vector<Point> points) {	_points = points; }
	color_rgb getColor()const{ return _color; }
	void setColor(float r, float g, float b)
	{
		_color._r = r; _color._g = g; _color._b = b;
	}
	void addPoint(Point p){ _points.push_back(p); }
};