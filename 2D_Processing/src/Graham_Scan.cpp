#include "Graham_Scan.h"
#include <algorithm>


void Graham_Scan::calculEnveloppe()
{
	computeCentroid();
	//Sort the Points following GrahamSort rule
	enveloppe_ = _points;
	std::sort(this->enveloppe_.begin(), enveloppe_.end(), GrahamSort(centroid_));
	std::vector<Point>::iterator sInit = enveloppe_.begin(), pivot = sInit;
	bool avance(false);
	do
	{
		std::vector<Point>::iterator suiv = pivot , prec = pivot;
		if (pivot == enveloppe_.begin())
		{
			prec = enveloppe_.end() - 1;
			suiv = pivot + 1;
		}
		else if (pivot == enveloppe_.end() - 1)
		{
			suiv = enveloppe_.begin();
			prec = pivot - 1;
		}
		else
		{
			prec = pivot - 1;
			suiv = pivot + 1;
		}
		if (Graham_Scan::OrientedAngle(*suiv, *pivot, *prec) <= M_PI) //Si le pivot est convexe
		{
			pivot = suiv;
			avance = true;
		}
		else
		{
			sInit = prec;
			enveloppe_.erase(pivot);
			pivot = sInit;
			avance = false;
		}
	} while (pivot != sInit || !avance);
}

//
//
void Graham_Scan::computeCentroid() 
{
	float sumX(0), sumY(0);

	for(auto& point : _points) 
	{
		sumX += point.getX();
		sumY += point.getY();
	}
	centroid_.setX(sumX / _points.size());
	centroid_.setY(sumY / _points.size());
}

double Graham_Scan::OrientedAngle(const Point& a, const Point& b, const Point& c)
{
	double vecUX(a.getX() - b.getX())
		, vecUY(a.getY() - b.getY())
		, vecVX(c.getX() - b.getX())
		, vecVY(c.getY() - b.getY())
		, det(vecUX*vecVY - vecUY*vecVX)
		, dot(vecUX*vecVX + vecUY*vecVY);
	return det < 0 ? 2 * M_PI + std::atan2(det, dot) : std::atan2(det, dot);
}
