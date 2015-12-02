#pragma once
#include "Edge.h"
#include "Point.h"
#include "LineStrip.h"#include <vector>
#include <list>

class Delaunay
{
public:
	Delaunay();
	~Delaunay();

	void TriangulationDelaunay(std::vector<Edge>);

	bool DelaunayCriteria();
	void addTop(Point);
	void Voronoi();

};

