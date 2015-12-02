#pragma once

#include "Point.h"

class Line {
private:
	Point* _startPoint;
	Point* _endPoint;

public:
	Line() :
		_startPoint(nullptr), _endPoint(nullptr) {}

	Line(Point* startPoint, Point* endPoint) :
		_startPoint(startPoint), _endPoint(endPoint) {}

	//float getLength() const;

	// TODO: remettre du const
	Point* getEndPoint() { return _endPoint; }
	void setEndPoint(Point* endPoint) { _endPoint = endPoint; }

	Point* getStartPoint() { return _startPoint; }
	void setStartPoint(Point* startPoint) { _startPoint = startPoint; }

	friend std::ostream& operator<<(std::ostream& os, Line& l) {
		os << "[" << *(l.getStartPoint()) << " - " << *(l.getEndPoint()) << "]";
		return os;
	}

	~Line();
};