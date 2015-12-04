#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

class Line;

class Point {
private:
	float _x;
	float _y;
	Line* line;

public:
	Point() : _x(0.f), _y(0.f) {}
	Point(float x, float y) : _x(x), _y(y) {}
	Point(const Point& p) : _x(p._x), _y(p._y) {}
	Point& operator=(const Point& p) {
		if(this != &p) {
			_x = p._x;
			_y = p._y;
		}
		return *this;
	}
	Point operator-() const { return Point(-_x, -_y); }
	bool operator==(const Point& p)
	{
		return this->_x == p._x && this->_y == p._y;
	}

	double DistanceTo(const Point& p) {
		return std::sqrt(std::pow(this->_x - p._x, 2)
						 + std::pow(this->_y - p._y, 2));
	}
	double GetDistance() {
		return std::sqrt(std::pow(_x, 2) + std::pow(_y, 2));
	}

	float getX() const { return _x; }
	float getY() const { return _y; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	Line* getLine() const { return line; }
	void setLine(Line* val) { line = val; }

	friend std::ostream& operator<<(std::ostream& os, const Point& p) {
		os << "(" << p.getX() << ", " << p.getY() << ")";
		return os;
	}
};