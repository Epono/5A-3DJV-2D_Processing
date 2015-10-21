#include "Graham-Scan.h"

Graham_Scan::Graham_Scan(std::vector<Point> points) {
	m_points = points;
}

//
//
void Graham_Scan::computeCentroid() {
	float sumX = 0, sumY = 0;

	for(auto point : m_points) {
		sumX += point.getX();
		sumY += point.getY();
	}

	m_centroid = new Point(sumX / m_points.size(), sumY / m_points.size());
}

float angle2(Point o, Point a, Point b) {
	//float dot_product = dotProduct(o, a, b);
	//return acos();
	Point vector_oa(a.getX() - o.getX(), a.getY() - o.getY());
	Point vector_ob(b.getX() - o.getX(), b.getY() - o.getY());
	float angle = atan2(vector_ob.getY(), vector_ob.getX()) - atan2(vector_oa.getY(), vector_oa.getX());
	return angle;
}

void Graham_Scan::sortPoints() {
	std::vector<Point*> sortedPoints;

	for(auto& point : m_points) {
		float angle_rad = angle2(Point(1000000, m_centroid->getY()), Point(m_centroid->getX(), m_centroid->getY()), point);
		float angle_deg = angle_rad / 3.14 * 180;
		std::cout << point.getX() << ", " << point.getY() << " - " << angle_deg << std::endl;
	}
}

//float dotProduct(Point o, Point a, Point b) {
//	double a[] = {a->getX() - o.getX(), a->getY() - o.getY(), 0};
//	double b[] = {b->getX() - o.getX(), b->getY() - o.getY(), 0};
//	return std::inner_product(begin(a), end(a), begin(b), 0.0);
//}

//float angleWithOxB(Point p) {
//	//cos - 1(   u.v   )
//	//        ||u||||v||
//	//atan2(v_y, v_x) - atan2(u_y, u_x)
//	return atan2(p.getY(), p.getX());
//}

//float distance(Point a, Point b) {
//	float dx = a.getX() - b.getX(), dy = a.getY() - b.getY();
//	return sqrt(dx * dx + dy * dy);
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