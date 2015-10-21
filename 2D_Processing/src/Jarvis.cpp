#include "Jarvis.h"

Jarvis::Jarvis(std::vector<Point*> points) {
	_points = points;
}

//float length(Point PiPj) {
//	return sqrt((PiPj.getY() * PiPj.getY()) + (PiPj.getX() * PiPj.getX()));
//}

//float angle(Point v, Point PiPj) {
//	std::cout << "Angle de " << v.getX() << ", " << v.getY() << " avec " << PiPj.getX() << ", " << PiPj.getY() << " : ";
//	v.setX(v.getX() / length(v));
//	v.setY(v.getY() / length(v));
//	PiPj.setX(PiPj.getX() / length(PiPj));
//	PiPj.setY(PiPj.getY() / length(PiPj));
//
//	float dot = v.getX() * PiPj.getX() + v.getY() * PiPj.getY();
//	float det = v.getX() * PiPj.getX() - v.getY() * PiPj.getY();
//
//	//float angle = atan2(PiPj.getY(), PiPj.getX()) - atan2(v.getY(), v.getX());
//	//float angle = atan2(PiPj.getX(), PiPj.getY()) - atan2(v.getX(), v.getY());
//
//	float angle = atan2(det, dot);
//	if(angle < 0)
//		angle += 3.14;
//
//	std::cout << angle * (180 / 3.14) << std::endl;
//	// L'angle est dans le sens horaire
//	return angle;
//}

double angle(const Point& a, const Point& b) {
	double vecUX(a.getX())
		, vecUY(a.getY())
		, vecVX(b.getX())
		, vecVY(b.getY())
		, det(vecUX*vecVY - vecUY*vecVX)
		, dot(vecUX*vecVX + vecUY*vecVY);
	double angle = det < 0 ? 2 * M_PI + std::atan2(det, dot) : std::atan2(det, dot);
	std::cout << angle << std::endl;
	return angle;
}

void Jarvis::computeJarvis() {
	Point& start = *(_points.at(0));
	int index = 0;

	for(int i = 1; i < _points.size(); i++) {
		Point& point = *(_points.at(i));
		if(point.getX() < start.getX() || (point.getX() == start.getX() && point.getY() < start.getY())) {
			index = i;
			start = point;
		}
	}

	Point v(0, -1);
	int i = index;

	do {
		m_enveloppe.push_back(_points.at(i));
		int j;
		if(i == 0) {
			j = 1;
		}
		else {
			j = 0;
		}

		// PiPj est un vecteur (x = distance en x, y = distance en y)
		Point PiPj(_points.at(j)->getX() - _points.at(i)->getX(), _points.at(j)->getY() - _points.at(i)->getY());

		//float amin = angle(v, PiPj);
		float amin = angle(v, PiPj);
		float lmax = _points.at(i)->DistanceTo(*(_points.at(j)));
		int inew = j;

		//Recheche du point le plus proche (en angle) de la droite
		for(j = inew + 1; j < _points.size(); j++) {
			if(j != i) {
				Point PiPj2(_points.at(j)->getX() - _points.at(i)->getX(), _points.at(j)->getY() - _points.at(i)->getY());
				float a = angle(v, PiPj2);
				if(amin > a || (amin == a && lmax < _points.at(i)->DistanceTo(*(_points.at(j))))) {
					amin = a;
					lmax = _points.at(i)->DistanceTo(*(_points.at(j)));
					inew = j;
				}
			}
		}

		// Mise à jour du pivot directeur
		v.setX(_points.at(inew)->getX() - _points.at(i)->getX());
		v.setY(_points.at(inew)->getY() - _points.at(i)->getY());
		i = inew;
	} while(i != index);
}

void Jarvis::addPoint(Point& const point) {
	_points.push_back(&point);

	computeJarvis();
}