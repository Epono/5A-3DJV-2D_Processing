#pragma once

#include <vector>
#include <iostream>

#include "Point.h"
#include "LineStrip.h"

class Graham_Scan : public LineStrip
{
private:
	Point* centroid_;
	std::vector<Point*> enveloppe_;

public:
	Graham_Scan(std::vector<Point*> points) : LineStrip(points) {}

	void calculEnveloppe();
	void computeCentroid();
	static double OrientedAngle(const Point&, const Point&, const Point&);

	Point* getCentroid() { return centroid_; }
	std::vector<Point*> getEnveloppe() { return enveloppe_; }
};

typedef struct GrahamSort
{
	GrahamSort(Point* b) : barycentre_(b){}
	
	bool operator()(Point* Pi, Point* Pj)
	{
		Point* X(new Point(barycentre_->getX(), barycentre_->getY() + 1));
		double PiAngle(Graham_Scan::OrientedAngle(*X, *barycentre_, *Pi))
			, PjAngle(Graham_Scan::OrientedAngle(*X, *barycentre_, *Pj));
		
		if (PiAngle == PjAngle)
			return Pi->DistanceTo(*barycentre_) < Pj->DistanceTo(*barycentre_);
		else
			return PiAngle < PjAngle;
	}

	Point* barycentre_;
} GrahamSort;
