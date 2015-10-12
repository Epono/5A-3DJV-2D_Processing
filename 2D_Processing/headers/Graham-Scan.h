#pragma once

#include <vector>
#include <iostream>

#include <math.h>

#include "Point.h"

class Graham_Scan {
private:
	Point* m_centroid;
	std::vector<Point> m_points;

public:
	Graham_Scan(std::vector<Point> points);

	void computeCentroid();
	void sortPoints();

	inline Point* getCentroid() const { return m_centroid; };
};