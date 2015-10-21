#pragma once

#include <vector>
#include <iostream>

#include <math.h>

#include "Point.h"

class Jarvis {
private:
	std::vector<Point> m_points;
	std::vector<Point> m_enveloppe;

public:
	Jarvis(std::vector<Point> const points);

	void computeJarvis();
	void addPoint(Point& const point);

	std::vector<Point>& getEnveloppe() { return m_enveloppe; }
};