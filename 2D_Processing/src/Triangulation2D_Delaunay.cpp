#include "Triangulation2D_Delaunay.h"
#include "Line.h"

bool Triangulation2D_Delaunay::Circumcircle(const Point& p0, const Point& p1, const Point& p2, Point& center, float& radius) {
	float dA, dB, dC, aux1, aux2, div;

	dA = p0.getX() * p0.getX() + p0.getY() * p0.getY();
	dB = p1.getX() * p1.getX() + p1.getY() * p1.getY();
	dC = p2.getX() * p2.getX() + p2.getY() * p2.getY();

	aux1 = (dA*(p2.getY() - p1.getY()) + dB*(p0.getY() - p2.getY()) + dC*(p1.getY() - p0.getY()));
	aux2 = -(dA*(p2.getX() - p1.getX()) + dB*(p0.getX() - p2.getX()) + dC*(p1.getX() - p0.getX()));
	div = (2 * (p0.getX()*(p2.getY() - p1.getY()) + p1.getX()*(p0.getY() - p2.getY()) + p2.getX()*(p1.getY() - p0.getY())));

	if(div == 0) {
		return false;
	}

	center.setX(aux1 / div);
	center.setY(aux2 / div);

	radius = sqrt((center.getX() - p0.getX())*(center.getX() - p0.getX()) + (center.getY() - p0.getY())*(center.getY() - p0.getY()));

	return true;
}

bool Triangulation2D_Delaunay::IsInsideCircle(Point& center, float radius, Point& p) {
	return std::pow(p.getX() - center.getX(), 2) + std::pow(p.getY() - center.getY(), 2) < std::pow(radius, 2);
}

void Triangulation2D_Delaunay::computeTriangulation() {

}