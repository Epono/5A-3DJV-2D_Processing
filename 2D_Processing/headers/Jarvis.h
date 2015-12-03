#pragma once

#include <vector>
#include <iostream>

#include "Point.h"
#include "LineStrip.h"

class Jarvis : public LineStrip {
private:
	std::vector<Point> m_enveloppe;

public:
	Jarvis() : LineStrip() {}
	Jarvis(std::vector<Point> points) { _points = points; }

	void computeJarvis();
	void addPoint(Point& const point);

	std::vector<Point> getEnveloppe() const { return m_enveloppe; }
};