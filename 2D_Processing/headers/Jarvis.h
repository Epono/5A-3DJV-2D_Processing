#pragma once

#include <vector>
#include <iostream>

#include "Point.h"
#include "LineStrip.h"

class Jarvis : public LineStrip {
private:
	//std::vector<Point> m_points;
	std::vector<Point*> m_enveloppe;

public:
	Jarvis() : LineStrip(){}
	Jarvis(std::vector<Point*> points);

	void computeJarvis();
	void addPoint(Point& const point);

	std::vector<Point*>& getEnveloppe() { return m_enveloppe; }
};