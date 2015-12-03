#pragma once

#include "Point.h"

class Line {
private:
	Point _startPoint;
	Point _endPoint;

public:
	Line() :
		_startPoint(), _endPoint() {}

	Line(Point startPoint, Point endPoint) :
		_startPoint(startPoint), _endPoint(endPoint) {}

	Point getEndPoint() const { return _endPoint; }
	void setEndPoint(const Point& endPoint) { _endPoint = endPoint; }

	Point getStartPoint()const { return _startPoint; }
	void setStartPoint(const Point& startPoint) { _startPoint = startPoint; }

	friend std::ostream& operator<<(std::ostream& os, const Line& l) {
		os << "[" << l.getStartPoint() << " - " << l.getEndPoint() << "]";
		return os;
	}
};