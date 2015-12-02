#include "Jarvis.h"

double angle(const Point& a, const Point& b) {
	double vecUX(a.getX())
		, vecUY(a.getY())
		, vecVX(b.getX())
		, vecVY(b.getY())
		, det(vecUX*vecVY - vecUY*vecVX)
		, dot(vecUX*vecVX + vecUY*vecVY);
	double angle = det <= 0 ? 2 * M_PI + std::atan2(det, dot) : std::atan2(det, dot);
	return angle;
}

// TODO: Reste des bugs chelou
void Jarvis::computeJarvis() {
	m_enveloppe.clear();

	if(_points.size() < 2) {
		return;
	}

	Point start = Point(_points.at(0)->getY(), _points.at(0)->getY());
	int i0 = 0;

	for(unsigned int iterationDeMonZboub = 1; iterationDeMonZboub < _points.size(); iterationDeMonZboub++) {
		Point& point = *(_points.at(iterationDeMonZboub));
		if(point.getX() < start.getX() || (point.getX() == start.getX() && point.getY() < start.getY())) {
			i0 = iterationDeMonZboub;
			start = point;
		}
	}

	Point v(0, -1);
	int i = i0;

	do {
		m_enveloppe.push_back(_points.at(i));
		unsigned int j;
		if(i == 0) {
			j = 1;
		}
		else {
			j = 0;
		}

		// PiPj est un vecteur (x = distance en x, y = distance en y)
		Point PiPj(_points.at(j)->getX() - _points.at(i)->getX(), _points.at(j)->getY() - _points.at(i)->getY());

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
	} while(i != i0);
}

void Jarvis::addPoint(Point& const point) {
	_points.push_back(&point);

	computeJarvis();
}