#include "Graham-Scan.h"
#include <algorithm>


void Graham_Scan::calculEnveloppe()
{
	computeCentroid();
	//Sort the Points following GrahamSort rule
	enveloppe_ = _points;
	std::sort(this->enveloppe_.begin(), enveloppe_.end(), GrahamSort(centroid_));
	std::vector<Point*>::iterator sInit = enveloppe_.begin(), pivot = sInit;
	bool avance(false);
	do
	{
		std::vector<Point*>::iterator suiv = pivot , prec = pivot;
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
		if (Graham_Scan::OrientedAngle(**prec, **pivot, **suiv) <= M_PI) //Si le pivot est convexe
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
		sumX += point->getX();
		sumY += point->getY();
	}

	centroid_ = new Point(sumX / _points.size(), sumY / _points.size());
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


float angle2(Point o, Point a, Point b) {
	//float dot_product = dotProduct(o, a, b);
	//return acos();
	Point vector_oa(a.getX() - o.getX(), a.getY() - o.getY());
	Point vector_ob(b.getX() - o.getX(), b.getY() - o.getY());
	float angle = atan2(vector_ob.getY(), vector_ob.getX()) - atan2(vector_oa.getY(), vector_oa.getX());
	return angle;
}

//float dotProduct(Point o, Point a, Point b) {
//	double a[] = {a->getX() - o.getX(), a->getY() - o.getY(), 0};
//	double b[] = {b->getX() - o.getX(), b->getY() - o.getY(), 0};
//	return std::inner_product(begin(a), end(a), begin(b), 0.0);
//}






// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
//int orientation(Point b, Point q, Point r) {
//	int val = (q.getY() - b.getY()) * (r.getX() - q.getX()) -
//		(q.getX() - b.getX()) * (r.getY() - q.getY());
//
//	if(val == 0) return 0;  // colinear
//	return (val > 0) ? 1 : 2; // clock or counterclock wise
//}