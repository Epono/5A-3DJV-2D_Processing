#include "Graham-Scan.h"
#include <algorithm>

Graham_Scan::Graham_Scan(std::vector<Point*> points) : m_points(points)
{ }

void Graham_Scan::calculEnveloppe()
{
	computeCentroid();
	//Sort the Points following GrahamSort rule
	std::sort(m_points.begin(), m_points.end(), GrahamSort(m_centroid));
}

//
//
void Graham_Scan::computeCentroid() 
{
	float sumX(0), sumY(0);

	for(auto point : m_points) 
	{
		sumX += point->getX();
		sumY += point->getY();
	}

	m_centroid = new Point(sumX / m_points.size(), sumY / m_points.size());
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