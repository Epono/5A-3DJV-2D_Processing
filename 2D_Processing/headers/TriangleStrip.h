#pragma once

#include <vector>
#include "Triangle.h"

class TriangleStrip {
protected:
	std::vector<Point*> _points;
	std::vector<Triangle*> _triangles;
	color_rgb _color;

public:
	TriangleStrip() :
		_points(std::vector<Point*>()), _triangles(std::vector<Triangle*>()), _color(color_rgb(1.f, 1.f, 1.f)) {}

	TriangleStrip(std::vector<Point*> points) :
		_points(points), _triangles(std::vector<Triangle*>()), _color(color_rgb(1.f, 1.f, 1.f)) {}

	TriangleStrip(std::vector<Point*> points, color_rgb color) :
		_points(points), _triangles(std::vector<Triangle*>()), _color(color) {}

	TriangleStrip(std::vector<Point*> points, std::vector<Triangle*> triangles) :
		_points(std::vector<Point*>(points)), _triangles(triangles), _color(color_rgb(1.f, 1.f, 1.f)) {}

	std::vector<Point*> getPoints() const { return _points; }
	void setPoints(std::vector<Point*> points) { _points = points; }

	std::vector<Triangle*>& getTriangles() { return _triangles; }
	void setTriangles(std::vector<Triangle*> triangles) { _triangles = triangles; }

	color_rgb getColor() { return _color; }
	void setColor(float r, float g, float b) { _color._r = r; _color._g = g; _color._b = b; }

	void addTriangle(Triangle* t) { _triangles.push_back(t); }
};